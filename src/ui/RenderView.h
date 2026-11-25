#pragma once

#include <imgui.h>

// forward declaration
struct ViewSettings;

class RenderView
{
public:
    explicit RenderView(ViewSettings& view_settings);

    void draw(ImVec2 image_size, ImTextureID texture_id);

private:
    
    void doZoom();
    ViewSettings& viewSettings_;

};