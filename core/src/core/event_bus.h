#pragma once

#include <functional>
#include <typeindex>
#include <vector>
#include <unordered_map>

namespace core {
	class EventBus {
	public:
		using SubscriptionID = size_t;

		template<typename T>
		using EventCallback = std::function<bool(const T&)>;

		template<typename T>
		SubscriptionID Subscribe(EventCallback<T> callback) {
			SubscriptionID id = next_id_++;
			listeners_[std::type_index(typeid(T))].push_back(
				{
					id,
					[callback](const void* e) {
						return callback(*static_cast<const T*>(e));
					}
				}
			);
			return id;
		}

		template<typename T>
		void Unsubscribe(SubscriptionID id) {
			auto it = listeners_.find(std::type_index(typeid(T)));
			if (it == listeners_.end()) return;

			auto& vec = it->second;
			vec.erase(std::remove_if(vec.begin(), vec.end(),
				[id](const Listener& l) { return l.id == id; }), vec.end()
			);
		}

		template<typename T>
		void Emit(const T& event) {
			auto it = listeners_.find(std::type_index(typeid(T)));
			if (it == listeners_.end()) return;

			auto listeners_copy = it->second;
			for (auto& listener : listeners_copy) {
				if (listener.callback(&event)) break;
			}
		}

	private:
		struct Listener {
			SubscriptionID id;
			std::function<bool(const void*)> callback;
		};

		std::unordered_map<std::type_index, std::vector<Listener>> listeners_;
		SubscriptionID next_id_ = 0;
	};
}
