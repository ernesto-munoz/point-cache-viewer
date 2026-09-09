#pragma once

#include <iostream>
#include <memory>
#include <filesystem>
#include "core/layer.h"
#include "core/utils.h"
#include "core/event_bus.h"
#include "raylib.h"
#include "rlgl.h"
#include "alembic_cache_reader.h"
//#include "usd_cache_reader.h"
#include "events.h"

#define POINT_SIZE 22.0f

class PointCloudViewerLayer : public core::Layer {
	std::unique_ptr<CacheReaderInterface> cache_reader_;
	bool is_playing_ = false;
	unsigned int current_frame = 0;

	FrameData frame_data_;
	Camera3D camera_ = { 0 };
	Model model_;
	Mesh mesh_;
	Shader point_shader_;
	int point_size_loc_;
	int color_loc_;
	int light_dir_loc_;
	bool dirty_mesh_ = true;


	void LoadCurrentFrame();

public:
	PointCloudViewerLayer();
	~PointCloudViewerLayer();
	void OnRender() override;
	void OnUpdate(float ts) override;
	void OnAttach() override;

	void NextFrame();
	void LoadFile(std::string file_path);
};