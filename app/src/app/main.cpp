#include "core/application.h"
#include "debug_layer.h"
#include "pc_viewer_layer.h"
#include "ui_control_layer.h"

int main() {
	SetTraceLogLevel(LOG_WARNING);

	core::ApplicationSpecification app_spec;
	app_spec.window_specification.width = 1920;
	app_spec.window_specification.height = 1080;
	app_spec.name = "Duplication";
	app_spec.window_specification.title = "Duplication Window";

	core::Application application(app_spec);
	application.PushLayer<PointCloudViewerLayer>();
	application.PushLayer<UIControlLayer>();
	application.PushLayer<DebugLayer>();
	application.Run();
}