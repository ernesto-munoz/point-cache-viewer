#include "ui_control_layer.h"
#include "ui_control_layer.h"

void UIControLayer::OnRender()
{
	rlImGuiSetup(true); // true = dark theme
    rlImGuiBegin();

    
    ImGui::Begin("Control Window");

    ImGui::Text("Hola desde ImGui + Raylib!");
    ImGui::Separator();

    if (ImGui::Button("Click me")) {
    }
    ImGui::SameLine();


    static float color[3] = { 0.1f, 0.1f, 0.1f };
    if (ImGui::ColorEdit3("Color de fondo", color)) {
        //clear_color = Color{
        //    (unsigned char)(color[0] * 255),
        //    (unsigned char)(color[1] * 255),
        //    (unsigned char)(color[2] * 255),
        //    255
        //};
    }
    DrawVideoControlBar(GetScreenWidth(), GetScreenHeight());

    ImGui::End();

    rlImGuiEnd();
}

void UIControLayer::DrawVideoControlBar(float screen_width, float screen_height)
{
    const float bar_height = 40.0f;
    ImGui::SetNextWindowPos(ImVec2(0, screen_height - bar_height));
    ImGui::SetNextWindowSize(ImVec2(screen_width, screen_height));


    // this flags for the desired visualization of the window
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;


    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.6f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Video Controls", nullptr, flags);
    if (ImGui::Button(is_playing_ ? "Pause" : "Play")) {
        is_playing_ = !is_playing_;
    }
    ImGui::SameLine();

    ImGui::SetNextItemWidth(-100); // space for the time
    if (ImGui::SliderFloat("##progress", &progress_, 0.0f, 1.0f, "")) {
        GetEventBus()->Emit(PlaybackSeekEvent(progress_));

    }

    ImGui::SameLine();
    ImGui::Text("%02d:%02d / %02d:%02d",
        (int)(progress_ * duration_frames_) / 60, (int)(progress_ * duration_frames_) % 60,
        (int)duration_frames_ / 60, (int)duration_frames_ % 60
    );

    ImGui::End();

    ImGui::PopStyleVar(2); // window padding and window border size
    ImGui::PopStyleColor();


}
