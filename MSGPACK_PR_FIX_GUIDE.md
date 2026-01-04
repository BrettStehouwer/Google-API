# msgpack-c PR #1147 - Fix Implementation Guide

## Files to Update

### 1. Create/Update: `cmake/pkg-config.pc.in`

```pc
Name: msgpack
Description: MessagePack C/C++ serialization library - efficient binary serialization format
Version: 3.3.0
prefix=@CMAKE_INSTALL_PREFIX@
includedir=${prefix}/include

# msgpack-c is a header-only C++ library
# No external libraries required for basic usage
# Boost is optional for enhanced adaptors (std::optional, std::string_view, etc.)
Cflags: -I${includedir}
Libs:
Requires:
```

### 2. Update: `CMakeLists.txt`

Add this at the **end** of your CMakeLists.txt file (after all other INSTALL directives):

```cmake
# Generate pkg-config file for msgpack-c
# This allows installations to be detected via pkg-config
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/msgpack.pc"
  @ONLY
)

INSTALL(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/msgpack.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
  COMPONENT msgpack-cxx
)
```

## Key Changes from Your Original PR

### ❌ What Was Wrong
1. Used `${CMAKE_BINARY_DIR}` instead of `${CMAKE_CURRENT_BINARY_DIR}`
2. Added unnecessary CMake variables for `-I` and `-L` flags
3. Tried to handle Boost compilation flags (header-only doesn't need them)
4. Missing project metadata in .pc file

### ✅ What's Fixed
1. Uses correct CMake variables
2. Hardcoded `-I` flag directly in template
3. No compilation flags needed (header-only library)
4. Includes proper project name, description, and version
5. Simplified CMakeLists.txt code

## How It Works

When installed, the pkg-config file will be at:
```
/usr/local/lib/pkgconfig/msgpack.pc
```
(or wherever CMAKE_INSTALL_LIBDIR points)

Users can then detect msgpack-c with:
```bash
pkg-config --cflags msgpack
# Output: -I/usr/local/include

pkg-config --libs msgpack
# Output: (empty - header-only library)

pkg-config msgpack
# Output: msgpack - MessagePack C/C++ serialization library
```

## Build & Test

```bash
# Configure with standard CMake
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -B build

# Build
cmake --build build

# Install (requires sudo or proper permissions)
sudo cmake --install build

# Verify installation
pkg-config --list-all | grep msgpack
pkg-config --cflags --libs msgpack
```

## C++ Usage After Installation

```cpp
#include <msgpack.hpp>

int main() {
    // msgpack-c is now discoverable via pkg-config
    // Users can compile with: g++ $(pkg-config --cflags msgpack) main.cpp
    return 0;
}
```

## Addressing Maintainer Feedback

✅ **Use standard CMake install variables**
- `@CMAKE_INSTALL_PREFIX@` → `${prefix}`
- `@CMAKE_INSTALL_INCLUDEDIR@` → `${prefix}/include`

✅ **Simplify CMakeLists.txt**
- Remove `PKG_CONFIG_LIBS` and `PKG_CONFIG_CFLAGS` variables
- Use `@ONLY` for template substitution

✅ **No Boost complexity**
- Boost is optional - users enable it independently
- Don't export Boost flags in .pc file

✅ **Header-only library**
- No `Libs:` line needed
- Empty `Requires:` field

This solution follows pkg-config best practices and aligns with what the maintainer requested!
