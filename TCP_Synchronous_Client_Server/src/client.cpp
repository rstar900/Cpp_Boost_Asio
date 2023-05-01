#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

int main()
{
    std::cout << "Starting Client..." << std::endl;

    // io_service object needed for core io functionality of asio
    io_service io_service_;

    // Create a socket
    tcp::socket socket_{io_service_};

    std::cout << "Connecting to server..." << std::endl;

    // Connect the client to server endpoint on local host ipv4 on port 1234
    socket_.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 1234));

    std::cout << "Connected to server!" << std::endl;

    // Defining this for storing error codes
    boost::system::error_code error;

    // Send message to server
    std::string msg;
    std::cout << "Type the message you wanna send." << std::endl;
    std::getline(std::cin, msg);
    msg += "\n";
    write(socket_, buffer(msg), error);
    if (!error)
    {
        std::cout << "Message sent to server successfully!" << std::endl;
    }
    else
    {
        std::cout << "Message could not be sent! " << error.message() << std::endl;
    }

    // Getting the response message back from the server
    streambuf buf;
    read(socket_, buf, transfer_all(), error);
    if (error && error != error::eof)
    {
        std::cout << "Could not receive message from the server! " << error.message() << std::endl;
    }
    else
    {
        std::cout << "Received message from the server: " << buffer_cast<const char*>(buf.data());
    }

    return 0;
}