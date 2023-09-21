#pragma once
#include "jsonrpcpp.hpp"
#include "mbed.h"
#include <string>
#include <functional>


class ServerComModule {
	const int bufferSize;

    jsonrpcpp::Parser parser;
	BufferedSerial serialPort;
	
    ServerComModule();
    static ServerComModule* serverComModule;

public:
    static void initializeComModule();
    static ServerComModule& getAccess();
    ~ServerComModule();
    void runServer();
    void registerFunction(const std::string& name, jsonrpcpp::notification_callback function);
    void registerFunction(const std::string& name, jsonrpcpp::request_callback function); 	
};

