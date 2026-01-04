#pragma once

#include <vector>
#include <string>
#include <memory>
#include "model_config.hpp"

namespace castor {

class Tokenizer; // Forward declaration

/**
 * @brief Main inference engine for LLM models using TensorRT
 * 
 * Manages model loading, CUDA memory, and high-performance inference
 * optimized for NVIDIA Ada Lovelace (RTX 4090, sm_89).
 */
class Engine {
public:
    Engine();
    ~Engine();

    /**
     * @brief Initialize the engine with a TensorRT engine plan
     * @param engine_path Path to .plan or .engine file
     * @param config Model configuration
     * @return true if initialization successful
     */
    bool initialize(const std::string& engine_path, const ModelConfig& config);

    /**
     * @brief Shutdown and cleanup resources
     */
    void shutdown();

    /**
     * @brief Run inference on input tokens
     * @param input_ids Token IDs to process
     * @param output_logits Output logits (vocab_size predictions)
     * @return true if inference successful
     */
    bool infer(const std::vector<int32_t>& input_ids, std::vector<float>& output_logits);

    /**
     * @brief Get current model configuration
     */
    const ModelConfig& get_config() const { return config_; }

    /**
     * @brief Check if engine is initialized
     */
    bool is_initialized() const { return initialized_; }

    /**
     * @brief Get associated tokenizer
     */
    std::shared_ptr<Tokenizer> get_tokenizer() const { return tokenizer_; }

    /**
     * @brief Set associated tokenizer
     */
    void set_tokenizer(std::shared_ptr<Tokenizer> tokenizer) { tokenizer_ = tokenizer; }

private:
    ModelConfig config_;
    bool initialized_ = false;
    std::shared_ptr<Tokenizer> tokenizer_;
    
    // TensorRT runtime and engine will be held here
    // Placeholder for actual TensorRT integration
    void* cuda_context_ = nullptr;
};

} // namespace castor
