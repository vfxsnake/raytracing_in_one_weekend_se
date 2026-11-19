#include "Application.h"

#include <stdexcept>
#include <algorithm>
#include <cmath>

#include <core/VulkanContext.h>
#include <core/SwapChain.h>

#include <imgui.h>

#include "ui/ImGuiLayer.h"
#include "display/DisplayPipeline.h"
#include "framebuffer/Color.h"

Application::Application() :
    framebuffer_(frameBufferWidth_, frameBufferHeight_)
{
    initWindow();
    initVulkan();
}


Application::~Application()
{
    context_->getLogicalDevice().waitIdle();
    displayPipeline_.reset();
    imguiLayer_.reset();
    swapChain_.reset();
    context_.reset();
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Application::run()
{
    drawTextPattern();
    mainLoop();
}


void Application::initWindow()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Unable to initialize GlFW");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // the size is going to be constant for now.

    window_ = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ray Tracing in One Weekend SE", nullptr, nullptr);
    if (!window_)
    {
        throw std::runtime_error("unable to create GLFW window");
    }
}


void Application::initVulkan()
{
    context_ = std::make_unique<VulkanContext>(window_);
    swapChain_ = std::make_unique<SwapChain>(*context_, window_);
    imguiLayer_ = std::make_unique<ImGuiLayer>(window_, *context_, *swapChain_);
    displayPipeline_ = std::make_unique<DisplayPipeline>(
        *context_, 
        *swapChain_,
        *imguiLayer_,
        frameBufferWidth_,
        frameBufferHeight_
    );
}


void Application::mainLoop()
{
    // persistent variable for this loop, used for tracking the render view zoom, only used by ImGui.
    float _render_view_zoom = 1.0f;
    
    while (!glfwWindowShouldClose(window_))
    {   
        glfwPollEvents();
        imguiLayer_->beginFrame();
        
        // ImGui panel begin (render texture window) 
        ImGui::Begin(
            "Render Viewport", // window name
            nullptr, // bool pointer if not null the closing button will appear and the state will be stored in the pointer.
            ImGuiWindowFlags_HorizontalScrollbar
        );
        
        // get current window rendering available surface
        ImVec2 region_available = ImGui::GetContentRegionAvail();
        // getting the offset to center the image if the frame buffer is smaller than the available space
        
        ImVec2 image_size = ImVec2(
            framebuffer_.getWidth() * _render_view_zoom, 
            framebuffer_.getHeight() * _render_view_zoom
        );

        ImVec2 offset  = ImVec2(
            std::max(0.0f, (region_available.x - image_size.x) * 0.5f ),
            std::max(0.0f, (region_available.y - image_size.y) * 0.5f )
        );

        ImVec2 cursor_position = ImGui::GetCursorScreenPos();
        offset.x += cursor_position.x;
        offset.y += cursor_position.y;
        ImGui::SetCursorScreenPos(offset);

        // converting the display pipeline texture into imTextureID for ImGui
        // the static cast unwraps the vk::DescriptorSet into VkDescriptorSet (raw c handle)
        VkDescriptorSet descriptor_set = static_cast<VkDescriptorSet>(displayPipeline_->getImGuiDescriptorSet());
        ImTextureID texture_id = reinterpret_cast<ImTextureID>(descriptor_set);


        ImGui::Image(texture_id, image_size);
        
        // zoom action
        bool is_image_hovered = ImGui::IsItemHovered(0);
        if (is_image_hovered)
        {
            float wheel_delta = ImGui::GetIO().MouseWheel;
            float zoom = _render_view_zoom * std::pow(1.25f, wheel_delta);
            _render_view_zoom = std::max(0.05f, zoom);
        }


        ImGui::End();
        ImGui::ShowDemoWindow();
        displayPipeline_->drawFrame(framebuffer_);
    }
}


void Application::drawTextPattern()
{
    for (int y = 0; y < framebuffer_.getHeight(); y++)
    {
        for (int x = 0; x < framebuffer_.getWidth(); x++)
        {
            Color color{
                .r = static_cast<uint8_t>(255* x/ (framebuffer_.getWidth() -1)),
                .g = static_cast<uint8_t>(255* x/ (framebuffer_.getWidth() -1)),
                .b = 0,
                .a = 255
            };
            framebuffer_.setPixel(x, y, color);
        }
    }
}