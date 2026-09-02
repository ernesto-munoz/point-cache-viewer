#pragma once

#include "layer.h"
#include "event.h"
#include "input_events.h"
#include "event_bus.h"

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <ranges>
#include <raylib.h>

#include <iostream>


namespace core {
	struct WindowSpecification {
		std::string title;
		uint32_t width = 1280;
		uint32_t height = 720;
		bool is_resizeable = true;
		bool vsync = true;
	};

	struct ApplicationSpecification {
		std::string name = "Application";
		WindowSpecification window_specification;
	};

	class Application {
	public:
		Application(const ApplicationSpecification& specification = ApplicationSpecification());
		~Application();

		void Run();
		void Stop();
		void PollEvents();

		void RaiseEvent(Event& event);

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer() {
			std::unique_ptr<TLayer> layer = std::make_unique<TLayer>();
			layer->Attach(event_bus_);
			layer_stack_.push_back(std::move(layer)); // cast to rvalue to allow the move of vector
		}

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		TLayer* GetLayer() {
			for (const auto& layer : layer_stack_) {
				if (auto casted = dynamic_cast	<TLayer*>(layer.get()))
					return casted;
			}
			return nullptr;
		}

		static Application& Get();
		static float GetFrameTime();

	private:
		ApplicationSpecification application_specification_;
		bool is_running_ = false;
		static inline Application* instance_ = nullptr;

		EventBus event_bus_;

		std::vector<std::unique_ptr<Layer>> layer_stack_;

		std::shared_ptr<Scheduler> scheduler_ptr_;
		std::thread scheduler_thread_;

		

		friend class Layer;
	};
}