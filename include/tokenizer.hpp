#pragma once

#include <vector>
#include <string>
#include <memory>

namespace castor {

/**
 * @brief Tokenizer for converting text <-> token IDs
 * 
 * Uses tokenizers-cpp library for efficient BPE tokenization
 */
class Tokenizer {
public:
    Tokenizer();
    ~Tokenizer();

    /**
     * @brief Load tokenizer model from file
     * @param model_path Path to tokenizer model (e.g., vocab.json, merges.txt)
     * @return true if loaded successfully
     */
    bool load(const std::string& model_path);

    /**
     * @brief Encode text to token IDs
     * @param text Input text
     * @return Vector of token IDs
     */
    std::vector<int32_t> encode(const std::string& text);

    /**
     * @brief Decode token IDs back to text
     * @param tokens Vector of token IDs
     * @return Decoded text
     */
    std::string decode(const std::vector<int32_t>& tokens);

    /**
     * @brief Get vocabulary size
     */
    size_t vocab_size() const { return vocab_size_; }

    /**
     * @brief Check if tokenizer is loaded
     */
    bool is_loaded() const { return loaded_; }

private:
    bool loaded_ = false;
    size_t vocab_size_ = 0;
    
    // Tokenizer instance will be held here
    // Placeholder for actual tokenizers-cpp integration
    void* tokenizer_ = nullptr;
};

} // namespace castor
