#include "ControlPanel.h"

#include <imgui.h>

#include "settings/Settings.h"


ControlPanel::ControlPanel(RenderSettings& render_settings, ViewSettings& view_settings):
    renderSettings_(render_settings),
    viewSettings_(view_settings)
{

}

void ControlPanel::draw()
{
    ImGui::Begin(
        "Control Panel", // window name
        nullptr, // bool pointer if not null the closing button will appear and the state will be stored in the pointer.
        0  // no flags
    );

    
    
    if (ImGui::Button("Reset Zoom"))
    {
        resetZoom();
    }


    ImGui::End();
}

void ControlPanel::resetZoom()
{
    viewSettings_.zoom = 1.0f;
}