#pragma once

#include <iostream>
#include "core/layer.h"
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "events.h"

class UIControLayer : public core::Layer {
public:
	void OnRender() override;

	float GetDuration() { return duration_frames_; }
	float SetDuration(float duration_frames) { duration_frames_ = duration_frames; }
	
	float GetProgress() { return progress_; }
	void SetProgress(float progress) { progress_ = progress; }

	bool IsPlaying() { return is_playing_; }
	void Play() { is_playing_ = true; }
	void Pause() { is_playing_ = false; }
	void Reset() { progress_ = 0.0f; }
	void Stop() { is_playing_ = false; progress_ = 0.0f; }

private:
	void DrawVideoControlBar(float screen_width, float screen_height);

	bool is_playing_ = false;
	float progress_ = 0.0f;
	float duration_frames_ = 250;

};