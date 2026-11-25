#pragma once

// forward declarations
struct RenderSettings;
struct ViewSettings;


class ControlPanel
{
public:
    explicit ControlPanel(RenderSettings& render_settings, ViewSettings& view_settings);

    void draw();

    
private:
    RenderSettings& renderSettings_;
    ViewSettings& viewSettings_;
    void resetZoom();
};
