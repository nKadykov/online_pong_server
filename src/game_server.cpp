#include "game_server.h"
#include <iostream>
#include <cstring>

Server::Server(unsigned short port)
    : m_acceptor(m_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
}

void Server::run() {
    accept();
    m_io_context.run();
}

void Server::accept() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_io_context);
    m_acceptor.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec) {
            m_clients.insert(socket);
            std::cout << "Client connected" << std::endl;

            auto buffer = std::make_shared<std::vector<char>>(256);
            socket->async_read_some(boost::asio::buffer(*buffer), [this, socket, buffer](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    buffer->resize(length);
                    auto gameState = deserialize(*buffer);
                    std::cout << "Received GameState: Ball(" << gameState.ball.x << ", " << gameState.ball.y << "), "
                              << "Platform1(" << gameState.platform1.x << ", " << gameState.platform1.y << "), "
                              << "Platform2(" << gameState.platform2.x << ", " << gameState.platform2.y << "), "
                              << "Score(" << gameState.score1 << " - " << gameState.score2 << ")" << std::endl;
                    broadcast(gameState);
                }
                m_clients.erase(socket);
            });
        }
        accept();
    });
}

void Server::broadcast(const GameState& gameState) {
    auto message = serialize(gameState);
    for (auto& client : m_clients) {
        boost::asio::async_write(*client, boost::asio::buffer(message), [](boost::system::error_code, std::size_t) {});
    }
}

std::vector<char> Server::serialize(const GameState& gameState) {
    std::vector<char> data(sizeof(GameState));
    std::memcpy(data.data(), &gameState, sizeof(GameState));
    return data;
}

GameState Server::deserialize(const std::vector<char>& data) {
    GameState gameState;
    std::memcpy(&gameState, data.data(), sizeof(GameState));
    return gameState;
}