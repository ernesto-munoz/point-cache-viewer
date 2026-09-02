#pragma once

#include <iostream>
#include <memory>
#include "core/layer.h"
#include "core/utils.h"
#include "core/event_bus.h"
#include "raylib.h"
#include "rlgl.h"
#include "alembic_cache_reader.h"
#include "events.h"

#define POINT_SIZE 22.0f

class PointCloudViewerLayer : public core::Layer {
	AlembicCacheReader cache_reader_;
	FrameData frame_data_;
	Camera3D camera_ = { 0 };
	Model model_;
	Mesh mesh_;
	Shader point_shader_;
	int uPointSize_loc_;
	int vColor_loc_;
	int uLightDir_loc_;
	unsigned int current_frame = 0;
	bool dirty_mesh_ = true;


	void LoadCurrentFrame();

public:
	PointCloudViewerLayer();
	~PointCloudViewerLayer();
	void OnRender() override;
	void OnUpdate(float ts) override;
	void OnAttach() override;

	void NextFrame();
};