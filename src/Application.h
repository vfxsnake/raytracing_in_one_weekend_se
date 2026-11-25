#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <memory>

#include "framebuffer/Framebuffer.h"
#include "ui/RenderView.h"
#include "ui/ControlPanel.h"
#include "settings/Settings.h"

// forward declarations
class VulkanContext;
class SwapChain;
class DisplayPipeline;
class ImGuiLayer;

class Application
{
public:
    Application();
    ~Application();

    void run();
    
private:
    void initWindow();
    void mainLoop();

    void initVulkan();

    void drawTextPattern();

    static constexpr uint32_t WINDOW_WIDTH = 1600;
    static constexpr uint32_t WINDOW_HEIGHT = 900;

    ViewSettings viewSettings_;
    RenderSettings renderSettings_;

    GLFWwindow* window_ = nullptr;

    Framebuffer framebuffer_;
    RenderView renderView_;
    ControlPanel controlPanel_;

    // vulkan unique pointer objects
    std::unique_ptr<VulkanContext> context_;
    std::unique_ptr<SwapChain> swapChain_;
    std::unique_ptr<ImGuiLayer> imguiLayer_;
    std::unique_ptr<DisplayPipeline> displayPipeline_;
};