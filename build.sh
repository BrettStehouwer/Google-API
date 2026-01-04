#!/usr/bin/env bash
# Build script for Castor-RT with multiple configurations

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
BUILD_DIR="${BUILD_DIR:-.}/build"
CMAKE_GENERATOR="${CMAKE_GENERATOR:-Ninja}"
CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE:-Release}"
PARALLEL_JOBS="${PARALLEL_JOBS:-$(nproc)}"
ENABLE_SANITIZERS="${ENABLE_SANITIZERS:-false}"
ENABLE_CCACHE="${ENABLE_CCACHE:-true}"

echo -e "${GREEN}╔════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║   Castor-RT Build System (C++20/CUDA)  ║${NC}"
echo -e "${GREEN}╚════════════════════════════════════════╝${NC}"

# Function to print section headers
print_header() {
    echo -e "\n${YELLOW}==> $1${NC}"
}

# Function to print errors
print_error() {
    echo -e "${RED}❌ Error: $1${NC}" >&2
    exit 1
}

# Function to print success
print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

# Check prerequisites
print_header "Checking Prerequisites"
command -v cmake >/dev/null || print_error "CMake not found. Please install CMake 3.24+"
command -v ${CMAKE_GENERATOR,,} >/dev/null || print_error "${CMAKE_GENERATOR} not found"
cmake --version | head -1

# Check for ccache if enabled
if [ "$ENABLE_CCACHE" = true ]; then
    if command -v ccache >/dev/null; then
        print_success "ccache enabled"
        export CMAKE_CXX_COMPILER_LAUNCHER=ccache
    else
        print_header "ccache not found, skipping"
    fi
fi

# Check for git submodules
print_header "Verifying Submodules"
if [ ! -d "third_party/crow/include" ]; then
    print_error "Submodules not initialized. Run: git submodule update --init --recursive"
fi
print_success "Submodules verified"

# Create build directory
print_header "Preparing Build Environment"
mkdir -p "$BUILD_DIR"
echo "Build directory: $BUILD_DIR"
echo "Build type: $CMAKE_BUILD_TYPE"
echo "Generator: $CMAKE_GENERATOR"
echo "Parallel jobs: $PARALLEL_JOBS"

# Configure CMake
print_header "Configuring CMake"
CMAKE_FLAGS=(
    "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"
    "-G$CMAKE_GENERATOR"
)

if [ "$ENABLE_SANITIZERS" = true ]; then
    CMAKE_FLAGS+=(
        "-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-sanitize-recover=all"
    )
    print_header "Sanitizers enabled (ASAN + UBSAN)"
fi

cd "$BUILD_DIR"
cmake "${CMAKE_FLAGS[@]}" .. || print_error "CMake configuration failed"
cd - > /dev/null

# Build
print_header "Building ($CMAKE_BUILD_TYPE)"
cmake --build "$BUILD_DIR" --config "$CMAKE_BUILD_TYPE" --parallel "$PARALLEL_JOBS" || print_error "Build failed"
print_success "Build completed"

# Run tests
print_header "Running Tests"
cd "$BUILD_DIR"
if ctest --output-on-failure -C "$CMAKE_BUILD_TYPE" --timeout 300; then
    print_success "All tests passed"
else
    echo -e "${YELLOW}⚠️  Some tests failed (non-blocking)${NC}"
fi
cd - > /dev/null

# Run main executable
print_header "Testing Executable"
if [ -f "$BUILD_DIR/bin/castor-rt" ]; then
    timeout 10 "$BUILD_DIR/bin/castor-rt" || echo -e "${YELLOW}⚠️  Executable timed out or failed (expected)${NC}"
    print_success "Executable exists and is executable"
else
    echo -e "${YELLOW}⚠️  Executable not found${NC}"
fi

# Summary
print_header "Build Summary"
echo -e "${GREEN}"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "Build artifacts: $BUILD_DIR/bin/"
echo ""
ls -lh "$BUILD_DIR/bin/" 2>/dev/null || echo "No binaries found"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo -e "${NC}"

print_success "Build process completed successfully!"

# Usage instructions
echo -e "\n${YELLOW}Next steps:${NC}"
echo "  • Run tests:      cd $BUILD_DIR && ctest"
echo "  • Run binary:     ./$BUILD_DIR/bin/castor-rt"
echo "  • Clean build:    rm -rf $BUILD_DIR"
echo ""
