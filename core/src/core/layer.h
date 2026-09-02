#pragma once

#include <memory>
#include "event.h"
#include "scheduler.h"
#include "event_bus.h"

namespace core {

	class Layer
	{
	public:
		virtual ~Layer() = default;

		//virtual void OnEvent(Event& event) {}

		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}
		void Attach(EventBus& event_bus) { event_bus_ = &event_bus; OnAttach(); }
		virtual void OnAttach() {}

		template<std::derived_from<Layer> T, typename... Args>
		void TransitionTo(Args&&... args)
		{
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}
		EventSystem& GetEventSystem() { return event_system_; }
		EventBus* GetEventBus() { return event_bus_; }

		std::shared_ptr<Scheduler> GetScheduler() { return Scheduler::Instance(); }

	private:
		void QueueTransition(std::unique_ptr<Layer> layer);
		EventSystem event_system_;
		EventBus* event_bus_;
	};

}