# Ray Tracing in One Weekend SE

My implementation of [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
(v4.0.2) with a twist of interactivity: instead of writing a `.ppm` file, the CPU tracer renders
progressively into a live Vulkan window provided by the [Snake Engine](https://github.com/vfxsnake/vk_tutorial_se),
each bucket appearing as soon as it is finished. No hardware acceleration yet.

This is a learning project with two goals:

1. Write the ray tracer from first principles following the book — the math layer, camera, sphere
   intersection, antialiasing, diffuse / metal / dielectric materials, defocus blur — all on the CPU.
2. Make it interactive: the tracer runs off the main thread, the window stays responsive, and parameters
   change live (Dear ImGui) and restart the render.

Hardware ray tracing (Vulkan RT / RTX) is **a separate, next step** in the Snake Engine; this CPU tracer is
the reference it will be validated against. *The Next Week* and *The Rest of Your Life* come after that.

---

## Current status

**As of 2026-11-12 — Phase 0 in progress: Dear ImGui (docking) builds and links; render-view integration next.**

- **Setup: complete.** Template applied; Snake Engine added as a submodule; display seed (Vulkan
  framebuffer upload + fullscreen quad, gradient test pattern) copied from
  [tiny_render_se](https://github.com/vfxsnake/tiny_render_se) at its Phase 0.4 commit. Builds and shows the gradient on WSL and Windows.
- **Phase 0 — Progressive display: in progress.** Milestone 0.1 (the framebuffer shown in a docked
  Dear ImGui render-view window with dockable control panels) started: ImGui `v1.93.0-docking` fetched and
  built as a library on WSL and Windows; not yet wired into the frame loop.
- **Chapters 2–13: not started.**

Known gaps: none yet.

---

## Architecture

```
raytracing_in_one_weekend_se/
├── src/
│   ├── main.cpp
│   ├── Application.h/.cpp     window lifecycle, owns display + tracer
│   ├── framebuffer/           Framebuffer + Color: the tracer/display boundary
│   ├── display/               Vulkan pipeline: uploads the framebuffer to a texture each frame
│   └── utils/Timer.h
├── shaders/display.slang
├── tests/                     Catch2 unit tests
├── assets/
├── docs/lessons/
└── engine/vk_tutorial_se/     git submodule — Snake Engine (Vulkan context + swap chain)
```

---

## Dependencies

| Dependency | How acquired |
|------------|-------------|
| Vulkan SDK (incl. `slangc`) | System install (LunarG) |
| GLFW 3.4 | CMake FetchContent |
| Vulkan-Hpp | CMake FetchContent |
| Catch2 | CMake FetchContent |
| Dear ImGui (`v1.93.0-docking`) | CMake FetchContent |
| Snake Engine | Git submodule (`engine/vk_tutorial_se/`) |

---

## Building

```bash
git clone --recurse-submodules https://github.com/vfxsnake/raytracing_in_one_weekend_se.git
```

- **WSL** (Debug, IntelliSense + tests; integrated GPU only — not for timings):

  ```bash
  cmake -S . -B build_wsl -G Ninja -DCMAKE_BUILD_TYPE=Debug
  cmake --build build_wsl
  ctest --test-dir build_wsl
  cd build_wsl && ./RayTracingInOneWeekendSE
  ```

- **Windows** (target platform, RTX 2070):

  ```bat
  cmake -S . -B build -G "Visual Studio 17 2022" -A x64
  cmake --build build --config Release
  ctest --test-dir build -C Release
  cd build && Release\RayTracingInOneWeekendSE.exe
  ```

Run the executable from the build directory — `shaders/` and `assets/` are resolved against the working
directory.

---

## Lessons

| # | Lesson | Status |
|---|--------|--------|
| 0 | Progressive display (tracer thread, bucket upload, ImGui, restart on change) | In progress |
| 2 | Output an Image | Not started |
| 3 | The vec3 Class | Not started |
| 4 | Rays, a Simple Camera, and Background | Not started |
| 5 | Adding a Sphere | Not started |
| 6 | Surface Normals and Multiple Objects | Not started |
| 7 | Moving Camera Code Into Its Own Class | Not started |
| 8 | Antialiasing | Not started |
| 9 | Diffuse Materials | Not started |
| 10 | Metal | Not started |
| 11 | Dielectrics | Not started |
| 12 | Positionable Camera | Not started |
| 13 | Defocus Blur | Not started |

> Numbering follows the book's chapters (1 is the overview, 14 "Where Next?").

---

## License

MIT
