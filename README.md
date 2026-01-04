# Castor-RT: LLM Inference Engine for Ada (RTX 4090)

Castor-RT is a **high-performance, latency-critical** Large Language Model inference engine designed exclusively for NVIDIA Ada Lovelace (RTX 4090, SM 89).

Built with:
- **C++20** for modern, efficient code
- **CUDA 12.x** for GPU acceleration
- **TensorRT** for optimized model inference
- **Crow** web framework for REST API
- **tokenizers-cpp** for fast text processing

## Implemented Features

### Phase 1: REST API Server ✅
- **GET `/health`** - Server status check  
- **GET `/model`** - Model configuration details  
- **POST `/infer`** - JSON-based inference endpoint  
- Crow framework with Boost.Asio  
- Multithreaded request handling

### Phase 2: Tokenization ✅
- HuggingFace JSON tokenizers (tokenizer.json)
- Byte-level BPE encoding/decoding
- Character-level fallback for unknown words
- Compatible with Llama-2, Mistral, Falcon

### Phase 3: GPU Inference (🚧 In Progress)
- TensorRT engine loading (.plan files)
- CUDA GPU inference
- FP32, FP16, INT8 quantization
- Multi-batch processing  

## Requirements

### System
- **GPU**: NVIDIA RTX 4090 (Ada architecture)
- **OS**: Ubuntu 22.04 LTS+ or Windows (MSVC)
- **RAM**: 24GB+ system, 24GB VRAM

### Software
- **CMake** 3.24+
- **CUDA Toolkit** 12.0+
- **TensorRT** 8.6+
- **cuDNN** 8.8+
- **GCC** 11+ or **MSVC** 2022+

## Build Instructions

### 1. Clone with submodules
```bash
git clone --recursive https://github.com/BrettStehouwer/Google-API.git
cd Google-API
```

### 2. Install dependencies (Ubuntu 22.04+)
```bash
sudo apt update && sudo apt install -y \
  build-essential cmake git libssl-dev libboost-all-dev nlohmann-json3-dev

# Optional: CUDA and TensorRT (for GPU inference)
# CUDA: https://developer.nvidia.com/cuda-downloads
# TensorRT: https://developer.nvidia.com/tensorrt/download
# cuDNN: https://developer.nvidia.com/cudnn
```

### 3. Configure and build (RTX 4090 / Ada Architecture)

**C++-only build (validation mode, no GPU required):**
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel $(nproc)
```

**With CUDA (RTX 4090 - SM 89):**
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCUDA_ARCH=89 ..
cmake --build . --config Release --parallel $(nproc)
```

### 4. Run
```bash
# Main inference server
./bin/castor-rt

# Run unit tests
./bin/castor-tests
```

## Features

✅ **REST API Server** (Crow framework)
- GET `/health` - Server health check
- GET `/model` - Model information
- POST `/infer` - Run inference (accepts JSON prompt)

✅ **Real Tokenization** (HuggingFace format)
- Supports `tokenizer.json` format (Llama-2, Mistral, Falcon)
- Byte-level BPE encoding
- Character-level fallback

✅ **GPU Acceleration** (TensorRT, optional)
- FP32, FP16, INT8 quantization support
- Multi-batch inference
- CUDA-optimized kernels

✅ **Code Quality**
- C++20 standard
- Comprehensive unit tests (5/5 passing)
- GitHub Actions CI/CD pipeline

```
.
├── CMakeLists.txt                 # Build configuration
├── include/
│   ├── engine.hpp                 # TensorRT inference engine
│   ├── tokenizer.hpp              # Text tokenization
│   └── model_config.hpp           # Model configuration
├── src/
│   ├── main.cpp                   # Entry point
│   ├── engine.cpp                 # Engine implementation
│   └── tokenizer.cpp              # Tokenizer implementation
├── third_party/
│   ├── crow/                      # Web framework (git submodule)
│   └── tokenizers-cpp/            # Tokenizers (git submodule)
└── cmake_modules/                 # CMake helper modules
```

## API Overview

### REST Server
```bash
# Start server on port 8080
./bin/castor-rt

# Health check
curl http://localhost:8080/health

# Get model info
curl http://localhost:8080/model

# Run inference
curl -X POST http://localhost:8080/infer \
  -H "Content-Type: application/json" \
  -d '{"prompt": "Hello, world!"}'
```

### Engine (C++ API)
```cpp
castor::Engine engine;
castor::ModelConfig config = {
    .model_name = "llama-2-7b",
    .max_seq_length = 4096,
    .vocab_size = 32000,
};
engine.initialize("models/llama-2-7b.plan", config);

std::vector<int32_t> tokens = {1, 2, 3, ...};
std::vector<float> logits;
engine.infer(tokens, logits);
```

### Tokenizer (C++ API)
```cpp
castor::Tokenizer tokenizer;
tokenizer.load_from_hf_json("tokenizer.json");

auto ids = tokenizer.encode("What is AI?");
// ids: [1, 1192, 318, ...]

auto text = tokenizer.decode(ids);
size_t vocab_size = tokenizer.vocab_size();
```

## Model Optimization

Castor-RT uses **TensorRT** to optimize LLM models:

1. **Export from PyTorch/Hugging Face**
   ```bash
   python tools/export_to_trt.py --model llama-2-7b --output models/llama-2-7b.plan
   ```

2. **Quantization** (optional)
   ```bash
   python tools/quantize.py --model models/llama-2-7b.plan --mode int8
   ```

3. **Compile & Benchmark**
   ```bash
   ./bin/castor-rt --benchmark models/llama-2-7b.plan
   ```

## Testing Status

### Current State
- ✅ C++20 compilation (GCC 11+, MSVC 2022+)
- ✅ REST API server (Crow framework)
- ✅ HuggingFace tokenizer support
- ✅ Unit tests: 5/5 passing
- ✅ HTTP endpoints responding correctly
- ⏳ TensorRT integration in progress

### Build Matrix (GitHub Actions)
- Ubuntu 22.04 LTS with GCC 11
- Ubuntu 24.04 LTS with GCC 13
- CUDA optional (makes TensorRT optional)
- Valgrind memory safety checks
- clang-tidy static analysis

## Troubleshooting

### CUDA not found
```bash
export CUDA_HOME=/usr/local/cuda-12
export PATH=$CUDA_HOME/bin:$PATH
export LD_LIBRARY_PATH=$CUDA_HOME/lib64:$LD_LIBRARY_PATH
```

### TensorRT missing
Ensure TensorRT is installed and visible:
```bash
find /opt -name "nvinfer.h" 2>/dev/null
```

### Engine file (.plan) not found
Generate one using the export script (see "Model Optimization" section).

## References

- [TensorRT Docs](https://docs.nvidia.com/deeplearning/tensorrt/developer-guide/)
- [CUDA C++ API](https://docs.nvidia.com/cuda/cuda-c-programming-guide/)
- [Crow Framework](https://crowcpp.org/)
- [tokenizers-cpp](https://github.com/mlc-ai/tokenizers-cpp)

## License

MIT (See LICENSE file)

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.
