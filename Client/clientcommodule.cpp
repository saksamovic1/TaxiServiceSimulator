#include "clientcommodule.h"
#include <algorithm>
#include <stdexcept>
#include "jsonrpcpp.hpp"
#include <iostream>
#if defined (_WIN32) || defined(_WIN64)
#define SERIAL_PORT "\\\\.\\COM6"
#endif
#if defined (__linux__)
#define SERIAL_PORT "/dev/ttyACM0"
#endif
#define MAX_BUFFER_SIZE 500


ClientComModule* ClientComModule::clientComModule{};


ClientComModule::ClientComModule(std::string const &devicePort, unsigned int baudRate) {
    char errorOpening = serialPort.openDevice(devicePort.c_str(), baudRate);

    if (errorOpening != 1) {
        throw std::runtime_error("Unable to open serial port!");
}


ClientComModule& ClientComModule::getAccess() {
    return *clientComModule;
}


void ClientComModule::initializeComModule() {
    if (clientComModule == nullptr) {
        clientComModule = new ClientComModule(SERIAL_PORT, 9600);
    } else {
        throw std::logic_error("Communication module already initialized!");
    }
}


int ClientComModule::pickCab(std::pair<int, int> entry, std::pair<int, int> exit) {

    serialPort.writeString((jsonrpcpp::Request
                            (1, "pickCab", Json({{"entryLocationRow", entry.first}, {"entryLocationCol", entry.second},
                                                {"exitLocationRow", exit.first}, {"exitLocationCol", exit.second}}))
                                .to_json().dump() + std::string("\n")).c_str());

    char buffer[MAX_BUFFER_SIZE];
    serialPort.readString(buffer, '\n', MAX_BUFFER_SIZE, 1000);

    return std::dynamic_pointer_cast<jsonrpcpp::Response>
        (jsonrpcpp::Parser::do_parse(buffer))->result().get<int>(); //okej nam je int, jer ce to biti samo rezultat koji je cab u pitanju
}

ClientComModule::~ClientComModule() {
    serialPort.closeDevice();
    delete clientComModule;
}


bool ClientComModule::testConnection() {
    if (clientComModule == nullptr) {
        throw std::logic_error("Communication module not initialized!");
    }

    serialPort.writeString((jsonrpcpp::Request(1, "testConnection")
                                .to_json().dump() + std::string("\n")).c_str());

    char buffer[MAX_BUFFER_SIZE];
    if (serialPort.readString(buffer, '\n', MAX_BUFFER_SIZE, 1000) <= 0) {
        throw std::runtime_error("Connection could not be established!..");
    }

    if (!std::dynamic_pointer_cast<jsonrpcpp::Response>
         (jsonrpcpp::Parser::do_parse(buffer))->result().get<bool>()) {
        throw std::runtime_error("Connection could not be established!");
    }
    //std::cout << buffer << "\n";
    //std::cout << "sve ok";
    return true;
}


void ClientComModule::freeBusyCab(int taxiID) { //radi
    serialPort.writeString((jsonrpcpp::Notification
                            ("freeBusyCab", Json({{"taxiID", taxiID}}))
                                .to_json().dump() + std::string("\n")).c_str());
}


std::vector<std::pair<int, int>> ClientComModule::setObstacleLocations() //radi
{
    std::vector<std::pair<int, int>> obstacleLocations;
    auto request = jsonrpcpp::Request(1, "setObstacleLocations");
    serialPort.writeString((request.to_json().dump() + std::string("\n")).c_str());

    char buffer[MAX_BUFFER_SIZE];
    serialPort.readString(buffer, '\n', MAX_BUFFER_SIZE, 1000);

    auto response = std::dynamic_pointer_cast<jsonrpcpp::Response>
        (jsonrpcpp::Parser::do_parse(buffer));

    std::pair<int, int> currentObstacle;

    if(response->result().is_array()) {
        for(int i=0; i<20; i=i+2) {
            currentObstacle.first = response->result().at(i).get<int>();
            currentObstacle.second = response->result().at(i+1).get<int>();
            obstacleLocations.push_back(currentObstacle);
        }
    }

    return obstacleLocations;

}

std::pair<int, int> ClientComModule::updateCabState(std::reference_wrapper<Cab> cab)
{
    auto request = jsonrpcpp::Request(2, "updateCabState", Json({{"taxiID", cab.get().getCabId()}}));
    if(cab.get().getCurrentRow() == -1 && cab.get().getCurrentCol() == -1) {
        request = jsonrpcpp::Request(2, "initializeCabState", Json({{"taxiID", cab.get().getCabId()}}));
    }

    serialPort.writeString((request.to_json().dump() + std::string("\n")).c_str());

    char buffer[MAX_BUFFER_SIZE];
    serialPort.readString(buffer, '\n', MAX_BUFFER_SIZE, 1000);
    std::cout << buffer << "\n";

    std::pair<int, int> currentCabLocation = std::pair<int, int>();

    auto response = std::dynamic_pointer_cast<jsonrpcpp::Response>
        (jsonrpcpp::Parser::do_parse(buffer));

    if(response->result().is_array()) {
        currentCabLocation.first = response->result().at(0).get<int>();
        std::cout << "x: " << response->result().at(0).get<int>() << std::endl;
        currentCabLocation.second = response->result().at(1).get<int>();
        std::cout << "y: " << response->result().at(1).get<int>() << std::endl;
    }

    return currentCabLocation;
}

