#pragma once

#include <string>

struct SeekEvent {
	int frame;
};

struct PauseEvent {};
struct StopEvent {};
struct ResumeEvent {};

struct FileOpenedEvent {
	int duration_frames;
};

struct OpenFileRequestEvent {
	std::string file_path;
};

struct NextFrameEvent {
	int next_frame;
};