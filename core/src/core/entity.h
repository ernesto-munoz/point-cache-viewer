#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace core {

	struct RendererInterface {
		virtual void Render(const class Entity& entity) const = 0;
		virtual ~RendererInterface() = default;
	};

	struct UpdaterInterface {
		virtual void Update(class Entity& entity, float dt) = 0;
		virtual ~UpdaterInterface() = default;
	};

	class Entity {
	public:
		Entity(std::string name,
			std::unique_ptr<RendererInterface> renderer,
			std::unique_ptr<UpdaterInterface> updater) :
			name_(std::move(name)),
			renderer_(std::move(renderer)),
			updater_(std::move(updater)) {
		}

		void Render() const {
			if (renderer_) renderer_->Render(const_cast<Entity&>(*this));
		}

		void Update(float dt) {
			if (updater_) updater_->Update(*this, dt);
		}

	private:
		std::string name_;
		std::unique_ptr<RendererInterface> renderer_;
		std::unique_ptr<UpdaterInterface> updater_;
	};

}