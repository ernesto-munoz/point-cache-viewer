#include "pc_viewer_layer.h"
#include "core/utils.h"

void PointCloudViewerLayer::LoadFrame(unsigned int frame)
{
	if (!cache_reader_) return;
	frame_data_ = cache_reader_->ReadFrame(frame);

	int count = frame_data_->positions.size() / 3;
	if(IsModelValid(model_)) UnloadModel(model_);

	mesh_ = {
		.vertexCount = count,
		.triangleCount = 1,
		.vertices = (float*)MemAlloc(count * 3 * sizeof(float)),
		//.colors = (unsigned char*)MemAlloc(count * 4 * sizeof(unsigned char)),
	};
	std::memcpy(mesh_.vertices, frame_data_->positions.data(), count * 3 * sizeof(float));
	UploadMesh(&mesh_, false);
	model_ = LoadModelFromMesh(mesh_);
	model_.materials[0].shader = point_shader_;
}

PointCloudViewerLayer::PointCloudViewerLayer() : Layer()
{
	camera_.position = Vector3 { 25.0f, 25.0f, 25.0f }; // position of the camera
	camera_.target = Vector3 { 0.0f, 0.0f, 0.0f }; // look at
	camera_.up = Vector3 { 0.0f, 1.0f, 0.0f }; // up in the world
	camera_.fovy = 45.0f;
	camera_.projection = CAMERA_PERSPECTIVE;
	
	point_shader_ = LoadShader(
		"data/shaders/point2.vs",
		"data/shaders/point2.fs"
	);
	point_size_loc_ = GetShaderLocation(point_shader_, "uPointSize");
	color_loc_ = GetShaderLocation(point_shader_, "vColor");
	light_dir_loc_ = GetShaderLocation(point_shader_, "uLightDir");

	// overkill but i wanted to do it this way
	GetScheduler()->Schedule([this]() {
		if (!is_playing_) return;
		NextFrame();
		dirty_mesh_ = true;
	},
	std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(1.0f / 25.0f))
	);
}

PointCloudViewerLayer::~PointCloudViewerLayer()
{
	UnloadShader(point_shader_);
}

void PointCloudViewerLayer::OnRender()
{
	DrawText(TextFormat("PC Viewer Layer"), 30, 60, 30, GREEN);
	UpdateCamera(&camera_, CAMERA_ORBITAL);

	float point_size = POINT_SIZE;
	Vector4 tint = { 1.0f, 0.0f, 0.0f, 1.0f };
	Vector3 lightDir = { 1.0f, 1.0f, 1.0f }; // light direction
	SetShaderValue(point_shader_, point_size_loc_, &point_size, SHADER_UNIFORM_FLOAT);
	SetShaderValue(point_shader_, color_loc_, &tint, SHADER_UNIFORM_VEC4);
	SetShaderValue(point_shader_, light_dir_loc_, &lightDir, SHADER_UNIFORM_VEC3);
	
	BeginMode3D(camera_);
		rlEnablePointMode();
		rlDisableBackfaceCulling();

		DrawModel(model_, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);

		rlEnableBackfaceCulling();
		rlDisablePointMode();

		DrawGrid(20, 1.0f);
	EndMode3D();
}

void PointCloudViewerLayer::OnUpdate(float ts)
{
	if (dirty_mesh_) {
		LoadFrame(current_frame);
		dirty_mesh_ = false;
	}
}

void PointCloudViewerLayer::OnAttach()
{
	GetEventBus()->Subscribe<SeekEvent>(
		[this](const SeekEvent& e) {
			current_frame = e.frame;
			LoadFrame(current_frame);
			return false;
		});

	GetEventBus()->Subscribe<OpenFileRequestEvent>(
		[this](const OpenFileRequestEvent& e) {
			LoadFile(e.file_path);
			return false;
		});

	GetEventBus()->Subscribe<PauseEvent>(
		[this](const PauseEvent& e) {
			is_playing_ = false;
			return false;
		}
	);

	GetEventBus()->Subscribe<ResumeEvent>(
		[this](const ResumeEvent& e) {
			is_playing_ = true;
			return false;
		}
	);
}

void PointCloudViewerLayer::NextFrame()
{
	current_frame = (current_frame + 1) % cache_reader_->GetFrameCount();
	GetEventBus()->Emit(NextFrameEvent(current_frame));
}

void PointCloudViewerLayer::LoadFile(std::string file_path)
{
	std::string ext = std::filesystem::path(file_path).extension().string();

	if (ext == ".abc") {
		cache_reader_ = std::make_unique<AlembicCacheReader>();
	}
	//if (ext == ".usd" || ext == ".usda" || ext == ".usdc") {
	//	cache_reader_ = std::make_unique<USDCacheReader>();
	//}

	cache_reader_->Open(file_path);
	// emit the event of a file opened
	GetEventBus()->Emit(FileOpenedEvent(cache_reader_->GetFrameCount()));
	is_playing_ = true;  // begin playing right away

	std::cout << cache_reader_->GetFrameCount() << std::endl;
}
