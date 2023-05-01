#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using ip::tcp;

// Class for handling multiple connections using shared_ptr and socket
// Will route async reads and writes to their corresponding handler functions
class con_handler : public boost::enable_shared_from_this<con_handler>
{
private :
    tcp::socket sock;
    const std::string message {"Hello from your lovely Server!\n"};
    const static int MAX_LENGTH {1024};
    char data[MAX_LENGTH];

public:
    // Type alias for ease of use
    using pointer = boost::shared_ptr<con_handler>;

    // Constructor initializes the socket
    con_handler(io_service& io_service_) : sock{io_service_} {};

    // Create a shared pointer of con_handler
    static pointer create(io_service& io_service_)
    {
        return pointer(new con_handler(io_service_));
    }

    // Get the socket object
    tcp::socket& socket()
    {
        return sock;
    }

    // Route async reads and writes to their corresponding handler functions
    void start()
    {
        auto self(shared_from_this());
        sock.async_read_some(buffer(data, MAX_LENGTH),
                            boost::bind(&con_handler::read_handler,
                            shared_from_this(),
                            placeholders::error,
                            placeholders::bytes_transferred));

        sock.async_write_some(buffer(message, MAX_LENGTH),
                            boost::bind(&con_handler::write_handler,
                            shared_from_this(),
                            placeholders::error,
                            placeholders::bytes_transferred));
    }

    // Read and Write Handlers ('const' is important before boost::system::error_code& err, otherwise it does not compile)

    void read_handler(const boost::system::error_code& err, size_t bytes_transferred)
    {
        if (!err)
        {
            std::cout << "Received from client: " << data;
        }
        else
        {
            std::cerr << "Error: " << err.message() << std::endl;
            sock.close();
        }

    }

    void write_handler(const boost::system::error_code& err, size_t bytes_transferred)
    {
        if (!err)
        {
            std::cout << "Sent Hello Message to client!" << std::endl;
        }
        else
        {
            std::cerr << "Error: " << err.message() << std::endl;
            sock.close();
        }
        
    }

};

// Class for instantiating a Server 
// For handling async accepts
class Server
{
private:
    tcp::acceptor acceptor_;

    // Function for creating con_handler objects and instantiating acceptors
    void start_accept()
    {
        std::cout << "Waiting for clients to connect..." << std::endl;

        // a workaround as acceptor.get_io_service() is deprecated in boost::asio's new versions
        con_handler::pointer connection = con_handler::create((io_service&)acceptor_.get_executor().context());

        // Route the asynchronous accepts to accept handler function 
        acceptor_.async_accept(connection->socket(), 
                                boost::bind(&Server::accept_handler,
                                this,
                                connection,
                                placeholders::error));
    }

public:
    // Constructor initializes the acceptor for localhost ipv4 address (127.0.0.1) and port 1234
    Server(io_service& io_service_) : acceptor_{io_service_, tcp::endpoint(tcp::v4(), 1234)} 
    {
        start_accept();
    } 

    // Accept handler function
    void accept_handler(con_handler::pointer connection, const boost::system::error_code& err)
    {
        if (!err)
        {
            std::cout << "Client connected!" << std::endl;
            connection->start();
        }
        start_accept(); // To keep accepting new connections
    }       
};

int main()
{
    std::cout << "Starting server..." << std::endl;

    // Initialize the object for core ASIO functionality
    io_service io_service_;

    // Rest of the logic resides under try catch blocks
    try 
    {
        Server server(io_service_);
        io_service_.run(); // to keep the server running asynchronously
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}