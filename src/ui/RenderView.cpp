#include "RenderView.h"

#include <algorithm>
#include <cmath>

#include "settings/Settings.h"

RenderView::RenderView(ViewSettings& view_settings):
    viewSettings_(view_settings)
{

}

void RenderView::draw(ImVec2 image_size, ImTextureID texture_id)
{
    // ImGui panel begin (render texture window) 
    ImGui::Begin(
        "Render Viewport", // window name
        nullptr, // bool pointer if not null the closing button will appear and the state will be stored in the pointer.
        ImGuiWindowFlags_HorizontalScrollbar
    );
    
    // get current window rendering available surface
    ImVec2 region_available = ImGui::GetContentRegionAvail();
    // getting the offset to center the image if the frame buffer is smaller than the available space
    
    image_size.x *= viewSettings_.zoom;
    image_size.y *= viewSettings_.zoom;


    ImVec2 offset  = ImVec2(
        std::max(0.0f, (region_available.x - image_size.x) * 0.5f ),
        std::max(0.0f, (region_available.y - image_size.y) * 0.5f )
    );

    ImVec2 cursor_position = ImGui::GetCursorScreenPos();
    offset.x += cursor_position.x;
    offset.y += cursor_position.y;
    ImGui::SetCursorScreenPos(offset);

    
    ImGui::Image(texture_id, image_size);
    
    // zoom action
    bool is_image_hovered = ImGui::IsItemHovered(0);
    if (is_image_hovered)
    {
        doZoom();
    }

    ImGui::End();
}


void RenderView::doZoom()
{
    float wheel_delta = ImGui::GetIO().MouseWheel;
    float view_zoom = viewSettings_.zoom * std::pow(viewSettings_.zoomRatio, wheel_delta);
    viewSettings_.zoom = std::max(viewSettings_.zoomMinValue, view_zoom);
}


