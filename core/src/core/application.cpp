#include "application.h"

namespace core {
	Application::Application(const ApplicationSpecification& specification)
		: application_specification_(specification)
	{
		instance_ = this;

		InitWindow(
			specification.window_specification.width,
			specification.window_specification.height,
			specification.window_specification.title.c_str()
		);
		SetTargetFPS(60);

	}
	Application::~Application()
	{
		CloseWindow();
		instance_ = nullptr;

	}
	void Application::Run()
	{
		is_running_ = true;

		while (is_running_) {

			// check with raylib context if is should stop the application
			if (WindowShouldClose()) {
				Stop();
				break;
			}
			PollEvents();
			
			// update each layer
			for (const std::unique_ptr<Layer>& layer : layer_stack_) {
				layer->OnUpdate(this->GetFrameTime());
			}

			// render each layer
			BeginDrawing();
			ClearBackground(DARKGRAY); // default background color
			for (const std::unique_ptr<Layer>& layer : layer_stack_) {
				layer->OnRender();
			}
			EndDrawing();
		}


	}
	void Application::Stop()
	{
		is_running_ = false;
	}

	void Application::PollEvents()
	{
		// check for movement event
		const Vector2 &pos = GetMouseDelta();
		if (pos.x != 0 || pos.y != 0) {
			MouseMovedEvent event(GetMouseX(), GetMouseY());
			RaiseEvent(event);
		}

		if (IsMouseButtonReleased(0)) {
			MouseButtonReleasedEvent event(0, GetMouseX(), GetMouseY());
			RaiseEvent(event);
		}
	}

	void Application::RaiseEvent(Event& event)
	{
		for (auto& layer : std::views::reverse(layer_stack_)) {
			layer->GetEventSystem().Emit(event);
			if (event.handled)
				break;
		}
	}

	Application& core::Application::Get()
	{
		if (!instance_) {
			throw std::runtime_error("ERROR: Application has not been initialized.");
		}
		return *instance_;
	}
	float core::Application::GetFrameTime()
	{
		return ::GetFrameTime();
	}
}