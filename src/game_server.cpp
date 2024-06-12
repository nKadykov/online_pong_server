#include "game_server.h"
#include "game_session.h"

GameServer::GameServer(boost::asio::io_context& io_context, short port) :
    m_acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), m_socket(io_context) {
        accept();
}

void GameServer::accept() {
    m_acceptor.async_accept(m_socket, [this](boost::system::error_code error) {
        if(!error) {
            std::make_shared<GameSession>(std::move(m_socket))->start();
        }
        accept();
    });
}