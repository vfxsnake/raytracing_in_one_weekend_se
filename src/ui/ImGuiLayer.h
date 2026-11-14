#pragma once

#include <vulkan/vulkan_raii.hpp>

class VulkanContext;
class SwapChain;
struct GLFWwindow;

class ImGuiLayer
{
public:
    ImGuiLayer(GLFWwindow* window, VulkanContext& context, const SwapChain& swap_chain);
    ~ImGuiLayer();

    ImGuiLayer(const ImGuiLayer&) = delete;
    ImGuiLayer& operator=(const ImGuiLayer&) = delete;

    ImGuiLayer(ImGuiLayer&&) = delete;
    ImGuiLayer& operator =(ImGuiLayer&&) = delete;

    void beginFrame();
    void recordDrawData(const vk::raii::CommandBuffer& command_buffer); 

private:
    static constexpr uint32_t DESCRIPTOR_POOL_SIZE = 16;
};
