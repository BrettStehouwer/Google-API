#pragma once

#include <vector>
#include <string>
#include <memory>

namespace castor {

// Forward declarations for internal use
class SimpleBPETokenizer;

/**
 * @brief Tokenizer for converting text <-> token IDs
 * 
 * Supports multiple tokenizer types:
 * - HuggingFace JSON tokenizers (tokenizer.json) ✅
 * - Byte-level BPE (vocab.json + merges.txt) ✅
 * - SentencePiece (requires Rust build)
 * - RWKV World tokenizer (requires Rust build)
 * 
 * Works with:
 * - Llama-2, Mistral, Falcon tokenizers
 * - Any HuggingFace tokenizer in JSON format
 */
class Tokenizer {
public:
    Tokenizer();
    ~Tokenizer();

    /**
     * @brief Load tokenizer from a HuggingFace JSON file
     * @param model_path Path to tokenizer.json
     * @return true if loaded successfully
     */
    bool load_from_hf_json(const std::string& model_path);

    /**
     * @brief Load tokenizer from a SentencePiece model file
     * @param model_path Path to tokenizer.model
     * @return true if loaded successfully (requires Rust build)
     */
    bool load_from_sentencepiece(const std::string& model_path);

    /**
     * @brief Load tokenizer from BPE vocab and merges files
     * @param vocab_path Path to vocab.json
     * @param merges_path Path to merges.txt
     * @return true if loaded successfully
     */
    bool load_from_bpe(const std::string& vocab_path, const std::string& merges_path);

    /**
     * @brief Load tokenizer from a JSON file (auto-detect type)
     * @param model_path Path to tokenizer model (e.g., tokenizer.json)
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

    /**
     * @brief Get the type of tokenizer (HF, SentencePiece, BPE, etc.)
     */
    std::string get_tokenizer_type() const { return tokenizer_type_; }

private:
    bool loaded_ = false;
    size_t vocab_size_ = 0;
    std::string tokenizer_type_ = "unknown";
    
    // Tokenizer instance (SimpleBPETokenizer)
    std::unique_ptr<SimpleBPETokenizer> tokenizer_;
};

} // namespace castor
