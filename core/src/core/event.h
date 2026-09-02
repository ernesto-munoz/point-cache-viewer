#pragma once

#include <string>
#include <functional>
#include <unordered_map>

namespace core {
	enum class EventType {
		kNone = 0,
		kWindowClose, kWindowReize,
		kKeyPressed, kKeyReleased,
		kMouseButtonPressed, kMouseButtonReleased, kMouseMoved, kMouseScrolled,
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
	class Event {
	public:
		bool handled = false;

		virtual ~Event() {}
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
	};

	class EventSystem {
	public:
		template<typename T>
		using EventFn = std::function<bool(const T&)>;

		template<typename T>
		void Subscribe(EventFn<T> func) {
			listeners_[T::GetStaticType()].push_back(
				[func](const Event& e) { return func(static_cast<const T&>(e)); }
			);
		}

		void Emit(const Event& e) {
			auto it = listeners_.find(e.GetEventType());
			if (it != listeners_.end()) {
				for (auto& cb : it->second) cb(e);
			}
		}

	private:
		std::unordered_map<EventType, std::vector<EventFn<Event>>> listeners_;
	};

}