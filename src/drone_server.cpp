#include <drone_server.h>
#include <drone.h>

#include "server_ws.hpp"
#include <future>
#include <iostream>

#include <string>
#include <thread>

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
WsServer server;

std::thread server_thread;

void drone::init_server(){
    server.config.port = 8080;
    auto &drone_endpoint = server.endpoint["^/drone/?$"];

    drone_endpoint.on_message = [](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::InMessage> in_message) {
        std::string response;
        std::string msg = in_message->string();
        run_command(msg, response);

        std::cout << "Server: Message received: \"" << msg << "\" from " << connection.get() << std::endl;
        std::cout << "Server: Sending message \"" << response << "\" to " << connection.get() << std::endl;

        // connection->send is an asynchronous function
        connection->send(response, [](const SimpleWeb::error_code &ec) {
            if(ec) {
                cout << "Server: Error sending message. " <<
                    // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
                    "Error: " << ec << ", error message: " << ec.message() << endl;
            }
        });
    };

    

    drone_endpoint.on_open = [](shared_ptr<WsServer::Connection> connection) {
        std::cout << "Server: Opened connection " << connection.get() << std::endl;
    };

    // See RFC 6455 7.4.1. for status codes
    drone_endpoint.on_close = [](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
        std::cout << "Server: Closed connection " << connection.get() << " with status code " << status << std::endl;
    };

    // Can modify handshake response headers here if needed
    drone_endpoint.on_handshake = [](shared_ptr<WsServer::Connection> /*connection*/, SimpleWeb::CaseInsensitiveMultimap & /*response_header*/) {
        return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
    };

    // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    drone_endpoint.on_error = [](shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
        std::cout << "Server: Error in connection " << connection.get() << ". "
            << "Error: " << ec << ", error message: " << ec.message() << std::endl;
    };

    
    promise<unsigned short> server_port;
    server_thread = std::thread([&server, &server_port]() {
        // Start server
        server.start([&server_port](unsigned short port) {
            server_port.set_value(port);
        });
    });
    std::cout << "Server listening on port " << server_port.get_future().get() << std::endl << std::endl;

}

void drone::close_server(){
    server.stop();
    server_thread.join();

}

/**
 * 
 * 
 */