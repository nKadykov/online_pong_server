#pragma once
#include <boost/asio.hpp>
#include <memory>

class GameServer {
public:
    GameServer(boost::asio::io_context& io_context, short port);
private:
    void accept();

    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
};