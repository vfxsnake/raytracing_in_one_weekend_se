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


    // Render Resolution:
    if (ImGui::CollapsingHeader("Render Settings"))
    {

        int _resolution_pair[2] = {
            static_cast<int>(renderSettings_.width), 
            static_cast<int>(renderSettings_.height)
        };

        // Resolution widgets
        if (
            ImGui::InputInt2(
                "Render Resolution", // label 
                _resolution_pair  // value reference
            )
        )
        {
            renderSettings_.width = static_cast<uint32_t>(_resolution_pair[0]);
            renderSettings_.height = static_cast<uint32_t>(_resolution_pair[1]);
        }
    }
    
    if (ImGui::CollapsingHeader("Render View Settings", ImGuiTreeNodeFlags_DefaultOpen ))
    {
        int _zoom = static_cast<int>(viewSettings_.zoom * 100);
        ImGui::SetNextItemWidth(80.0f);
        if(
            ImGui::DragInt(
                "zoom", // label
                &_zoom, // reference value
                1.0f, // velocity
                1, // min value
                500, // max value
                "%d%%" // format
            )
        )
        {
             viewSettings_.zoom = static_cast<float>(_zoom) * 0.01f;
        }
        ImGui::SameLine();
        // zoom ratio
        ImGui::SetNextItemWidth(80.0f);
        ImGui::InputFloat(
            "zoom ratio", // label
            &viewSettings_.zoomRatio // reference value
        );
        
        if (ImGui::Button("Reset Zoom"))
        {
            resetZoom();
        }
    }


    ImGui::End();
}

void ControlPanel::resetZoom()
{
    viewSettings_.zoom = 1.0f;
}