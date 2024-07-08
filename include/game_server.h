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
    bool if_on;
};

class Server {
public:
    Server(unsigned short);
    void run();

private:
    void accept();
    void cast();
    void handle_client(std::shared_ptr<boost::asio::ip::tcp::socket>);
    void broadcast(const std::vector<char>&);
    std::vector<char> serialize(const GameState&);
    GameState deserialize(const std::vector<char>&);

    boost::asio::io_context m_io_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::set<std::shared_ptr<boost::asio::ip::tcp::socket>> m_client_set;
    boost::asio::steady_timer m_timer;
    GameState m_game_state;
};