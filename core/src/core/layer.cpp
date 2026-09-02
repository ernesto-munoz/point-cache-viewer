#include "layer.h"
#include "application.h"

namespace core {
	void Layer::QueueTransition(std::unique_ptr<Layer> toLayer)
	{
		// TODO: don't do this
		auto& layer_stack = Application::Get().layer_stack_;
		for (auto& layer : layer_stack)
		{
			if (layer.get() == this)
			{
				layer = std::move(toLayer);
				return;
			}
		}
	}
}