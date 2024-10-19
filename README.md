# vk.c
A vulkan wrapper designed to simplify compute operations in pure C.

## Dependencies

```sh
sudo pacman -S gcc gdb cmake doxygen
```

## Build Steps

Build documentation:

```sh
cmake --build build --target doc_doxygen
```

Build shared library and examples:

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug 
cmake --build build --config Debug -j "$(nproc)"
```

Automate build steps:

```sh
chmod +x compile.sh
./compile.sh
```

Can use -Debug or -Release:

```sh
./compile.sh Release # Defaults to Debug
```

Control documentation generation:

```sh
./compile.sh Release OFF  # Builds without documentation
./compile.sh Debug ON    # Builds with documentation
```

## References

See Arch Linux installation documentation for latest instructions:

- [Vulkan Wiki](https://wiki.archlinux.org/title/Vulkan)

See Kronos documentation for latest API:

- [Vulkan Documentation](https://docs.vulkan.org/spec/latest/index.html)
- [Vulkan Specification](https://registry.khronos.org/vulkan/specs/1.3/html/index.html)
