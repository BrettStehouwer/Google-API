#pragma once

#include <memory>
#include <string>
#include "engine.hpp"
#include "tokenizer.hpp"

namespace castor {

/**
 * @brief REST API server for LLM inference using Crow framework
 */
class Server {
public:
    Server(int port = 8080);
    ~Server();

    /**
     * @brief Initialize server with engine and tokenizer
     */
    bool initialize(const std::shared_ptr<Engine>& engine, const std::shared_ptr<Tokenizer>& tokenizer);

    /**
     * @brief Start the HTTP server (blocking)
     */
    void run();

    /**
     * @brief Stop the server
     */
    void shutdown();

    /**
     * @brief Check if server is running
     */
    bool is_running() const { return running_; }

private:
    int port_;
    bool running_ = false;
    std::shared_ptr<Engine> engine_;
    std::shared_ptr<Tokenizer> tokenizer_;
};

} // namespace castor
