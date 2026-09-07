#pragma once

#include <iostream>
#include "core/layer.h"
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "events.h"
#include "tinyfiledialogs.h"

class UIControlLayer : public core::Layer {
public:
	UIControlLayer();
	void OnRender() override;
	void OnAttach() override;

	float duration() { return duration_frames_; }
	void set_duration(float duration_frames) { duration_frames_ = duration_frames; }
	
	float progress() { return progress_; }
	void set_progress(float progress) { progress_ = progress; }

	bool IsPlaying() { return is_playing_; }
	void Play() { is_playing_ = true; }
	void Pause() { is_playing_ = false; }
	void Reset() { progress_ = 0.0f; }
	void Stop() { is_playing_ = false; progress_ = 0.0f; }

private:
	void DrawVideoControlBar(float screen_width, float screen_height);

	bool is_playing_ = true;
	float progress_ = 0.0f;
	float duration_frames_ = 250;

	std::string selected_path_ = "data/point_cloud.abc";

};