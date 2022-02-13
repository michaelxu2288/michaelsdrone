#include <drone_server.h>
#include <drone.h>

#include "server_ws.hpp"
#include <future>
#include <iostream>

#include <string>
#include <thread>

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
using namespace std;
WsServer server;

thread server_thread;

void drone::init_server(){
    server.config.port = 8080;
    auto &drone_endpoint = server.endpoint["^/drone/?$"];

    drone_endpoint.on_message = [](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::InMessage> in_message) {
        string response;
        string msg = in_message->string();
        run_command(msg, response);

        cout << "Server: Message received: \"" << msg << "\" from " << connection.get() << endl;
        cout << "Server: Sending message \"" << response << "\" to " << connection.get() << endl;

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
        cout << "Server: Opened connection " << connection.get() << endl;
    };

    // See RFC 6455 7.4.1. for status codes
    drone_endpoint.on_close = [](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
        cout << "Server: Closed connection " << connection.get() << " with status code " << status << endl;
    };

    // Can modify handshake response headers here if needed
    drone_endpoint.on_handshake = [](shared_ptr<WsServer::Connection> /*connection*/, SimpleWeb::CaseInsensitiveMultimap & /*response_header*/) {
        return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
    };

    // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    drone_endpoint.on_error = [](shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
        cout << "Server: Error in connection " << connection.get() << ". "
            << "Error: " << ec << ", error message: " << ec.message() << endl;
    };

    
    promise<unsigned short> server_port;
    server_thread = thread([&server, &server_port]() {
        // Start server
        server.start([&server_port](unsigned short port) {
            server_port.set_value(port);
        });
    });
    cout << "Server listening on port " << server_port.get_future().get() << endl << endl;

}

void drone::close_server(){
    server.stop();
    server_thread.join();

}

/**
 * 
 * 
 */