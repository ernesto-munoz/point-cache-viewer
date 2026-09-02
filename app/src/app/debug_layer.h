#pragma once

#include "core/layer.h"
#include "raylib.h"

class DebugLayer : public core::Layer {
	void OnRender() override;
};