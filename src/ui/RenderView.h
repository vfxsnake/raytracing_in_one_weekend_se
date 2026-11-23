#pragma once

#include <imgui.h>


class RenderView
{
public:
    void draw(ImVec2 image_size, ImTextureID texture_id);
    
    float getZoom() const;
    float getZoomRatio() const;
    float getZoomMinValue() const;
    
    void setZoom(float zoom_value);
    void setZoomRatio(float zoom_ratio);
    void setZoomMinValue(float zoom_min_value);
    
    
    
private:
    
    void doZoom();
    
    float zoom_ = 1.0f;
    float zoomRatio_ = 1.25f;
    float zoomMinValue_ = 0.05f;
};