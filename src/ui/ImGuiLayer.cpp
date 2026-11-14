#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <core/VulkanContext.h>
#include <core/SwapChain.h>


ImGuiLayer::ImGuiLayer(GLFWwindow* window, VulkanContext& context, const SwapChain& swap_chain)
{
    ImGui::CreateContext();
    
    // Setup Dear ImGui style
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    
    vk::Format color_format = swap_chain.getFormat();
    vk::PipelineRenderingCreateInfo pipeline_rendering_create_info{
        .colorAttachmentCount = 1,
        .pColorAttachmentFormats = &color_format 
    };

    // Setup Platform/Renderer backends
    // srating ImplGlfw_
    ImGui_ImplGlfw_InitForVulkan(window, true);
    
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.ApiVersion = VK_API_VERSION_1_4;              // Pass in your value of VkApplicationInfo::apiVersion, otherwise will default to header version.
    init_info.Instance = *context.getInstance();
    init_info.PhysicalDevice = *context.getPhysicalDevice();
    init_info.Device = *context.getLogicalDevice();
    init_info.QueueFamily = context.getQueueFamilyIndex();
    init_info.Queue = *context.getQueue();
    init_info.DescriptorPoolSize = DESCRIPTOR_POOL_SIZE;
    init_info.MinImageCount = swap_chain.getImageCount();
    init_info.ImageCount = swap_chain.getImageCount();
    init_info.PipelineInfoMain.PipelineRenderingCreateInfo = pipeline_rendering_create_info;
    init_info.UseDynamicRendering = true;
    init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    
    // starting implVulkan
    ImGui_ImplVulkan_Init(&init_info);
}


ImGuiLayer::~ImGuiLayer()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void ImGuiLayer::beginFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();   
}


void ImGuiLayer::recordDrawData(const vk::raii::CommandBuffer& command_buffer)
{
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *command_buffer);
}