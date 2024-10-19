# vk.c
A Vulkan wrapper designed to simplify compute operations in pure C.

## Dependencies

```sh
sudo pacman -S gcc gdb cmake
```

## Build Steps

Clone the repository and enter the directory:

```sh
git clone https://github.com/teleprint-me/vk.c.git vk
cd vk
```

Build the shared library and examples:

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug 
cmake --build build --config Debug -j "$(nproc)"
```

Add execution bit and run to automate the build steps:

```sh
chmod +x compile.sh
./compile.sh # Defaults to Debug
```

You can specify the build type:

```sh
./compile.sh Release
```

## References

- [**Arch Wiki:**](https://wiki.archlinux.org/title/Vulkan) Comprehensive guide for installing and configuring Vulkan on Arch.
- [**Vulkan Documentation:**](https://docs.vulkan.org/spec/latest/index.html) Official Vulkan API documentation.
- [**Vulkan Specification:**](https://registry.khronos.org/vulkan/specs/1.3/html/index.html) Detailed specifications of the Vulkan API.
- [**GLSL Specification:**](https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.60.html) Specification for the OpenGL Shading Language.

## License

See the [**AGPL LICENSE**](LICENSE) file for more details.
