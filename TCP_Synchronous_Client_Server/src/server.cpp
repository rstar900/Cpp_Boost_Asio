#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

std::string receive_(tcp::socket& socket) 
{
    // read until a newline character is found and return the contents as a C++ string
    streambuf buf;
    read_until(socket, buf, "\n");
    std::string data = buffer_cast<const char*>(buf.data());
    return data;
}

void send_(tcp::socket& socket, const std::string& message)
{
    // Append a newline character to the message and send on the socket
    const std::string msg = message + "\n";
    write(socket, buffer(msg));
}

int main()
{
    std::cout << "Starting Server..." << std::endl;

    // io_service object needed for core io functionality of asio
    io_service io_service_;
    
    // Create accepter object to listen for incoming connection on ipv4 localhost address (127.0.0.1)
    tcp::acceptor acceptor_(io_service_, tcp::endpoint(tcp::v4(), 1234));

    // Create a socket object
    tcp::socket socket_{io_service_};

    std::cout << "Waiting for a client to connect..." << std::endl;

    // Waiting for client to connect
    acceptor_.accept(socket_);

    std::cout << "Client connected!" << std::endl;

    // Read data from client once it is connected
    std::string data = receive_(socket_);
    std::cout << "Received message from client: " << data;

    // Write data
    send_(socket_, "Hello from your lovely Server!");
    std::cout << "Hello Message sent to client!" << std::endl;
    return 0;
}