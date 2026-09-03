#include "ui_control_layer.h"

UIControlLayer::UIControlLayer()
{
    rlImGuiSetup(true); // true = dark theme
    ImGui::GetStyle().ScaleAllSizes(1.5f);
}

void UIControlLayer::OnRender()
{   
    rlImGuiBegin();
    ImGui::Begin("Control Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    if (ImGui::Button("Select...")) {
        const char* filter_patterns[] = { "*.abc", "*.usd" };
        const char* selected = tinyfd_openFileDialog(
            "Select file...",
            "C:/Users/aokuma/code/raylib-projects/point-cache-viewer/app/data/point_cloud.abc",
            2, filter_patterns, "Data file (.abc, .usd)", 0);
        if (selected) {
            selected_path_ = selected;
        }
    }
    ImGui::SameLine();
    ImGui::TextUnformatted(selected_path_.c_str());

    ImGui::BeginDisabled(selected_path_.empty());
    if (ImGui::Button("Load")) {
        GetEventBus()->Emit(OpenFileRequestEvent(selected_path_));
    }
    ImGui::EndDisabled();
    ImGui::End();

    DrawVideoControlBar(GetScreenWidth(), GetScreenHeight());

    rlImGuiEnd();
}

void UIControlLayer::OnAttach()
{
    GetEventBus()->Subscribe<FileOpenedEvent>(
        [this](const FileOpenedEvent& e) {
            set_duration(e.duration_frames);
            return false;
        });

    GetEventBus()->Subscribe<NextFrameEvent>(
        [this](const NextFrameEvent& e) {
            set_progress(e.next_frame / duration_frames_);
            return false;
        }
    );
}

void UIControlLayer::DrawVideoControlBar(float screen_width, float screen_height)
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
        is_playing_ ? GetEventBus()->Emit(ResumeEvent()) : GetEventBus()->Emit(PauseEvent());
    }
    ImGui::SameLine();

    ImGui::SetNextItemWidth(-100); // space for the time
    if (ImGui::SliderFloat("##progress", &progress_, 0.0f, 1.0f, "")) {
        //static_cast<int>(cache_reader_.GetFrameCount() * e.progress)
        GetEventBus()->Emit(SeekEvent(duration_frames_ * progress_));
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
