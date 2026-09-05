# McClone

> A Minecraft clone written in C++20.

**Version:** 1.0.2
**Author:** [SleepyFish](https://github.com/SleepyFish-YT)

---

## Supported platforms:
- Windows (x64) by Microsoft
###### Maybe someday, i will add linux support

---

## About

McClone is a from-scratch Minecraft-inspired voxel game written in modern C++20.
It aims to replicate core Minecraft mechanics while being built entirely in C++ using
OpenGL for rendering and OpenAL for spatial audio.

---

## Building

### Requirements

- CMake 3.31+
- C++20-compatible compiler (MSVC recommended on Windows)
- [vcpkg](https://github.com/microsoft/vcpkg)

### Dependencies (via vcpkg)

| Library | Purpose |
|---|---|
| [OpenGL](https://www.opengl.org/) | Graphics API |
| [GLFW](https://github.com/glfw/glfw) | Window & input |
| [GLAD](https://github.com/Dav1dde/glad) | OpenGL loader |
| [GLM](https://github.com/g-truc/glm) | Math library |
| [OpenAL Soft](https://github.com/kcat/openal-soft) | Spatial audio |
| [STB](https://github.com/nothings/stb) | Image loading |
| [nlohmann/json](https://github.com/nlohmann/json) | JSON parsing |
| winmm | Windows multimedia |

### Steps

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

---

## Usage

Run the executable directly or with optional arguments:

| Argument                  | Description |
|---------------------------|---|
| `--debug`                 | Show console window and enable debug logging |
| `--demo`                  | Run in demo mode |
| `--ignoreGlErrors`        | Suppress OpenGL error output |
| `--resolution <w> <h>`    | Set window size (e.g. `--resolution 1920 1080`) |
| `--resolution fullscreen` | Run at native screen resolution |

---

## Download

[Latest McClone Release](https://github.com/SleepyFish-YT/McClone/releases/latest) (./releases/latest)

---

## Discord Server
<a href="https://discord.gg/7JXXvkufJK"><img src="https://invidget.switchblade.xyz/7JXXvkufJK" alt="https://discord.gg/7JXXvkufJK"/></a>

---

## Licenses

Third-party library licenses are located in [`./licenses/`](./licenses/).

---

###### Signed by SleepyFish