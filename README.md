# pbrlib

pbrlib is a modern C++ rendering library built on Vulkan API. The project aims to provide high-performance rendering with support for modern techniques.

This is a hobby project that I work on in my spare time. The main goal is to learn and practice modern graphics programming techniques, explore Vulkan API capabilities, and build a solid foundation for future graphics projects.

## 🎯 Why this project?

The main goal of pbrlib is to provide a clean and efficient foundation for developing graphics applications on Vulkan. The library strives for:

- **Modern rendering**: Support for current rendering techniques
- **Performance**: Optimized architecture
- **Ease of use**: High-level API for development
- **Modularity**: Clear component separation

### ⚠️ Disclaimer

**This is a project in active development. API may change without prior notice. Do not expect any support or help. Pull requests will likely be ignored or dismissed.**

## 📋 Project tasks

Current tasks and development plans can be viewed in [GitHub Projects](https://github.com/users/ASIF1998/projects/1/views/1).

## 📄 License

The code is distributed under the GNU General Public License v3.0.

## 💻 Platforms

- **Windows** (SDL3)
- **Vulkan 1.3** required

## 🔨 Building

### Requirements

- **CMake 3.27+**
- **C++23** compiler
- **Vulkan SDK**
- **Python 3**

### Build options

- `PBRLIB_ENABLE_TESTS=ON` - Enable tests
- `PBRLIB_TEST_CONSOLE=ON` - Build test console
- `PBRLIB_SETUP_CI=OFF` - CI settings

### Building
```bash
cd third_party\glslang
python update_glslang_sources.py
cd "../.."
mkdir build 
cd build
cmake ..
```

### Dependencies
The project uses the following libraries:

- **Vulkan SDK** - Vulkan API
- **SDL3** - Window system and input
- **Assimp** - 3D model import
- **EnTT** - ECS system
- **spdlog** - Logging
- **VulkanMemoryAllocator** - Vulkan memory management
- **Tracy** - Profiling
- **STB** - Image loading
- **glslang** - Shader compilation
- **MoltenVK** - Implementation of Vulkan API graphics and compute functionality for macOS
- **cpptrace** - Сapturing and printing call stacks

## 🚀 Usage

### Simple example

```cpp
#include <pbrlib/engine.hpp>

int main() {
    pbrlib::Config config;
    config.width = 1920;
    config.height = 1080;
    config.title = "My App";
    config.draw_in_window = true;

    pbrlib::Engine engine(config);
    
    engine.setup([](pbrlib::Scene& scene) {
        // Scene initialization
    });
    
    engine.update([](pbrlib::Engine& engine, const pbrlib::InputStay& input, float delta) {
        // Logic update
    });
    
    engine.run();
    return 0;
}
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests
5. Create a Pull Request
