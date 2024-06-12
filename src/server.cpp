#include "game_session.h"
#include "game_server.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        if(argc != 2) {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }
        boost::asio::io_context io_context;
        GameServer server(io_context, std::atoi(argv[1]));
        io_context.run();
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}