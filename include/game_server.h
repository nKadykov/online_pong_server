#pragma once
#include <boost/asio.hpp>
#include <set>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

struct GameState {
    sf::Vector2f ball;
    sf::Vector2f platform1;
    sf::Vector2f platform2;
    int score1;
    int score2;
};

class Server {
public:
    Server(unsigned short port);
    void run();

private:
    void accept();
    void broadcast(const GameState& gameState);
    std::vector<char> serialize(const GameState& gameState);
    GameState deserialize(const std::vector<char>& data);

    boost::asio::io_context m_io_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::set<std::shared_ptr<boost::asio::ip::tcp::socket>> m_clients;
};