#include "ServerComModule.h"
#include <algorithm>
#include <stdexcept>
#ifndef NDEBUG
#include <iostream>
#endif


ServerComModule* ServerComModule::serverComModule{};


ServerComModule::ServerComModule() : bufferSize{ 500 }, serialPort(USBTX, USBRX) {
    serialPort.set_baud(9600);
    serialPort.set_format(8, BufferedSerial::None, 1);
    serialPort.set_blocking(true);

    parser.register_request_callback("testConnection", [](const jsonrpcpp::Id& id,
    	const jsonrpcpp::Parameter& params) {
    		return std::make_shared<jsonrpcpp::Response>(id, true);
    	});
}


ServerComModule& ServerComModule::getAccess() {
    return *serverComModule;
}


void ServerComModule::initializeComModule() {
    if (serverComModule == nullptr) {
        serverComModule = new ServerComModule();
    } else {
        throw std::logic_error("Communication module already initialized!");
    }
}


ServerComModule::~ServerComModule() {
    delete serverComModule;
}


void ServerComModule::runServer() {
    while (true) {
	   	char buffer[bufferSize];
	   	
	   	int cnt{};
	   	while (true) {
			serialPort.read(buffer + cnt, sizeof(char));
			if (buffer[cnt] == '\n') {
				buffer[cnt] = '\0';
				break;	
			}
			cnt++;
	   	}
	
	   	jsonrpcpp::entity_ptr entity{ parser.parse(buffer) };	   	
	   
	   	if (entity->is_response()) {
	   		std::string response{ entity->to_json().dump() };
	   		response += std::string("\n");
	   		
	   		serialPort.write(response.c_str(), response.length());
	   	}
    }
}

void ServerComModule::registerFunction(const std::string& name, jsonrpcpp::notification_callback function) {
	parser.register_notification_callback(name, function);
}
    
    
void ServerComModule::registerFunction(const std::string& name, jsonrpcpp::request_callback function) {
	parser.register_request_callback(name, function);
}

