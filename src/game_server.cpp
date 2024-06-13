#include "game_server.h"
#include <iostream>
#include <cstring>

Server::Server(unsigned short port)
    : m_acceptor(m_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), m_timer(m_io_context) {
}

void Server::run() {
    accept();
    cast();
    m_io_context.run();
}

void Server::accept() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_io_context);
    m_acceptor.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec) {
            m_client_set.insert(socket);
            std::cout << "Client connected" << std::endl;
            handle_client(socket);
        }
        accept();
    });
}

void Server::cast() {
    auto message = serialize(m_game_state);
    broadcast(message);

    m_timer.expires_after(std::chrono::milliseconds(16));
    m_timer.async_wait([this](boost::system::error_code error_code) {
        if(!error_code) {
            cast();
        }
    });
}

void Server::handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    auto buffer = std::make_shared<std::vector<char>>(sizeof(GameState));
    socket->async_read_some(boost::asio::buffer(*buffer), [this, socket, buffer](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            buffer->resize(length);
            auto client_state = deserialize(*buffer);
            std::cout << "Received GameState: Ball(" << client_state.ball.x << ", " << client_state.ball.y << "), "
                        << "Platform1(" << client_state.platform1.x << ", " << client_state.platform1.y << "), "
                        << "Platform2(" << client_state.platform2.x << ", " << client_state.platform2.y << "), "
                        << "Score(" << client_state.score1 << " - " << client_state.score2 << ")" << std::endl;
            m_game_state.ball = client_state.ball;
            m_game_state.platform1 = client_state.platform1;
            m_game_state.platform2 = client_state.platform2;
            m_game_state.score1 = client_state.score1;
            m_game_state.score2 = client_state.score2;
            handle_client(socket);
        } else {
            m_client_set.erase(socket);
        }
    });
}

void Server::broadcast(const std::vector<char>& message) {
    for (auto& client : m_client_set) {
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