#pragma once

#include <cstdint>

struct ViewSettings
{
    float zoom = 1.0f;
    float zoomRatio = 1.25f;
    float zoomMinValue = 0.05f;
};


struct RenderSettings
{
    uint32_t width = 800;
    uint32_t height = 600;
};
