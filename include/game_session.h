#pragma once
#include <boost/asio.hpp>
#include <memory>

class GameSession : public std::enable_shared_from_this<GameSession> {
public:
    GameSession(boost::asio::ip::tcp::socket socket);
    void start();
private:
    void read();
    void write(std::size_t length);

    boost::asio::ip::tcp::socket m_socket;
    std::array<char, 128> m_data;
};