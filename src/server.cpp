#include "server.hpp"
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

// Include Crow for REST API
#include "crow/app.h"
#include "crow/json.h"

using json = nlohmann::json;

namespace castor {

// Global server app (needed for request handling)
static std::shared_ptr<crow::SimpleApp> g_app;
static std::shared_ptr<Engine> g_engine;
static std::shared_ptr<Tokenizer> g_tokenizer;

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
    g_engine = engine;
    g_tokenizer = tokenizer;
    return true;
}

void Server::run() {
    if (!engine_ || !tokenizer_) {
        std::cerr << "[Server] Engine or Tokenizer not initialized\n";
        return;
    }

    running_ = true;
    
    // Create Crow app
    auto app = std::make_shared<crow::SimpleApp>();
    g_app = app;

    // GET /health - Server health check
    CROW_ROUTE((*app), "/health").methods("GET"_method)
    ([](const crow::request&) {
        auto response = crow::response(200);
        response.set_header("Content-Type", "application/json");
        json j;
        j["status"] = "healthy";
        j["model"] = g_engine->get_config().model_name;
        j["port"] = 8080;
        response.body = j.dump();
        return response;
    });

    // GET /model - Get model info
    CROW_ROUTE((*app), "/model").methods("GET"_method)
    ([](const crow::request&) {
        auto response = crow::response(200);
        response.set_header("Content-Type", "application/json");
        
        const auto& config = g_engine->get_config();
        json j;
        j["model_name"] = config.model_name;
        j["max_batch_size"] = config.max_batch_size;
        j["max_seq_length"] = config.max_seq_length;
        j["vocab_size"] = config.vocab_size;
        j["hidden_dim"] = config.hidden_dim;
        j["num_layers"] = config.num_layers;
        j["initialized"] = g_engine->is_initialized();
        j["tokenizer_loaded"] = g_tokenizer->is_loaded();
        
        response.body = j.dump(2);
        return response;
    });

    // POST /infer - Run inference
    CROW_ROUTE((*app), "/infer").methods("POST"_method)
    ([](const crow::request& req) {
        auto response = crow::response(200);
        response.set_header("Content-Type", "application/json");
        
        try {
            auto data = json::parse(req.body);
            
            if (!data.contains("prompt")) {
                response.code = 400;
                json error;
                error["error"] = "Missing 'prompt' field in request";
                response.body = error.dump();
                return response;
            }

            std::string prompt = data["prompt"].get<std::string>();
            
            // Encode prompt to tokens
            auto tokens = g_tokenizer->encode(prompt);
            
            // Run inference
            std::vector<float> logits;
            bool success = g_engine->infer(tokens, logits);
            
            if (!success) {
                response.code = 500;
                json error;
                error["error"] = "Inference failed (TensorRT not fully integrated yet)";
                response.body = error.dump();
                return response;
            }
            
            json result;
            result["prompt"] = prompt;
            result["input_tokens"] = tokens.size();
            result["input_token_ids"] = tokens;
            result["output_logits_count"] = logits.size();
            result["status"] = "success";
            result["message"] = "Inference completed (stub - TensorRT integration pending)";
            
            response.body = result.dump(2);
            return response;
        } catch (const json::exception& e) {
            response.code = 400;
            json error;
            error["error"] = std::string("JSON parse error: ") + e.what();
            response.body = error.dump();
            return response;
        }
    });

    std::cout << "[Server] 🚀 Starting REST API on http://0.0.0.0:" << port_ << "\n";
    std::cout << "[Server] Available endpoints:\n";
    std::cout << "  GET  http://localhost:" << port_ << "/health  - Server health check\n";
    std::cout << "  GET  http://localhost:" << port_ << "/model   - Get model information\n";
    std::cout << "  POST http://localhost:" << port_ << "/infer   - Run inference\n";
    std::cout << "\n[Server] Example inference:\n";
    std::cout << "  curl -X POST http://localhost:" << port_ << "/infer \\\n";
    std::cout << "    -H \"Content-Type: application/json\" \\\n";
    std::cout << "    -d '{\"prompt\": \"Hello, world!\"}'\n";
    std::cout << "\n[Server] Running... (press Ctrl+C to stop)\n\n";

    // Start the server (blocking)
    app->port(port_).multithreaded().run();
}

void Server::shutdown() {
    running_ = false;
    std::cout << "\n[Server] Shutting down...\n";
}

} // namespace castor
