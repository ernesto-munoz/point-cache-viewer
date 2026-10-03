#include "debug_layer.h"

void DebugLayer::OnRender()
{
	DrawText(TextFormat("FPS: %i", GetFPS()), GetScreenWidth() - 150, 30, 30, GREEN);
}
