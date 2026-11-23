#include "Application.h"

#include <stdexcept>

#include <core/VulkanContext.h>
#include <core/SwapChain.h>

#include <imgui.h>

#include "ui/ImGuiLayer.h"
#include "ui/RenderView.h"
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
    while (!glfwWindowShouldClose(window_))
    {   
        glfwPollEvents();
        imguiLayer_->beginFrame();
        
         // converting the display pipeline texture into imTextureID for ImGui
        // the static cast unwraps the vk::DescriptorSet into VkDescriptorSet (raw c handle)
        VkDescriptorSet descriptor_set = static_cast<VkDescriptorSet>(displayPipeline_->getImGuiDescriptorSet());
        ImTextureID texture_id = reinterpret_cast<ImTextureID>(descriptor_set);
        ImVec2 image_size = ImVec2(
            static_cast<float>(framebuffer_.getWidth()), 
            static_cast<float>(framebuffer_.getHeight())
        );
        
        // Render view class calls goes here
        renderView_.draw(image_size, texture_id);
        
        // temporary call for demo window, this will be discared later
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
                .g = static_cast<uint8_t>(255* y/ (framebuffer_.getHeight() -1)),
                .b = 0,
                .a = 255
            };
            framebuffer_.setPixel(x, y, color);
        }
    }
}
