# Castor-RT: LLM Inference Engine for Ada (RTX 4090)

Castor-RT is a **high-performance, latency-critical** Large Language Model inference engine designed exclusively for NVIDIA Ada Lovelace (RTX 4090, SM 89).

Built with:
- **C++20** for modern, efficient code
- **CUDA 12.x** for GPU acceleration
- **TensorRT** for optimized model inference
- **Crow** web framework for REST API
- **tokenizers-cpp** for fast text processing

## Features
✅ FP32, FP16, INT8 quantization support  
✅ Multi-batch inference  
✅ CUDA-optimized kernels  
✅ REST API server  
✅ Model caching  

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

### 2. Install dependencies (Ubuntu)
```bash
# CUDA Toolkit (https://developer.nvidia.com/cuda-downloads)
# TensorRT (https://developer.nvidia.com/tensorrt/download)
# cuDNN (https://developer.nvidia.com/cudnn)

sudo apt update && sudo apt install -y \
  build-essential cmake git libssl-dev
```

### 3. Configure and build
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release \
       -DCUDA_ARCH=89 \
       ..
cmake --build . --config Release --parallel $(nproc)
```

### 4. Run
```bash
./bin/castor-rt
```

## Project Structure

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

### Engine (High-Performance Inference)
```cpp
castor::Engine engine;
castor::ModelConfig config = {...};
engine.initialize("models/llama-2-7b.plan", config);

std::vector<int32_t> tokens = {1, 2, 3, ...};
std::vector<float> logits;
engine.infer(tokens, logits);
```

### Tokenizer (Text Processing)
```cpp
castor::Tokenizer tokenizer;
tokenizer.load("models/tokenizer.json");

auto ids = tokenizer.encode("Hello, world!");
auto text = tokenizer.decode(ids);
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

## Performance Targets

| Model | Batch | Precision | Throughput |
|-------|-------|-----------|-----------|
| Llama 2 7B | 1 | FP32 | ~200 tok/s |
| Llama 2 7B | 1 | FP16 | ~400 tok/s |
| Llama 2 13B | 1 | FP16 | ~250 tok/s |
| Mistral 7B | 1 | INT8 | ~500 tok/s |

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
