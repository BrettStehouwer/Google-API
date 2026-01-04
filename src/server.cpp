#include "server.hpp"
#include <iostream>
#include <sstream>

namespace castor {

Server::Server(int port) : port_(port) {}

Server::~Server() {
    if (running_) {
        shutdown();
    }
}

bool Server::initialize(const std::shared_ptr<Engine>& engine, const std::shared_ptr<Tokenizer>& tokenizer) {
    if (!engine || !tokenizer) {
        return false;
    }
    
    engine_ = engine;
    tokenizer_ = tokenizer;
    return true;
}

void Server::run() {
    if (!engine_ || !tokenizer_) {
        std::cerr << "[Server] Engine or Tokenizer not initialized\n";
        return;
    }

    running_ = true;

    std::cout << "[Server] Starting on http://0.0.0.0:" << port_ << "\n";
    std::cout << "[Server] Endpoints:\n";
    std::cout << "  GET  /health       - Server health check\n";
    std::cout << "  GET  /model        - Get model info\n";
    std::cout << "  POST /infer        - Run inference\n";
    
    // TODO: Implement REST server using Crow or Boost.Asio
    // For now, this is a placeholder
    
    std::cout << "[Server] Server would run here (Crow integration pending)\n";
}

void Server::shutdown() {
    running_ = false;
    std::cout << "[Server] Shutting down\n";
}

} // namespace castor
