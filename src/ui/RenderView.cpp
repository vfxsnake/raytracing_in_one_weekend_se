#include "RenderView.h"

#include <algorithm>
#include <cmath>

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
    
    image_size.x *= zoom_;
    image_size.y *= zoom_;


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


float RenderView::getZoom() const
{
    return zoom_;
}


float RenderView::getZoomRatio() const
{
    return zoomRatio_;
}


float RenderView::getZoomMinValue() const
{
    return zoomMinValue_;
}


void RenderView::setZoom(float zoom_value)
{
    zoom_ = std::max(zoomMinValue_, zoom_value);
}


void RenderView::setZoomRatio(float zoom_ratio)
{
    zoomRatio_ = zoom_ratio;
}


void RenderView::setZoomMinValue(float zoom_min_value)
{
    zoomMinValue_ = zoom_min_value;
}


void RenderView::doZoom()
{
    float wheel_delta = ImGui::GetIO().MouseWheel;
    float view_zoom = zoom_ * std::pow(zoomRatio_, wheel_delta);
    setZoom(view_zoom);
}


