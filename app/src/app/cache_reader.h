#pragma once
#include <vector>
#include <string>

struct FrameData {
	std::vector<float> positions;
	std::vector<float> normals;
};

class CacheReaderInterface {
public:
	virtual ~CacheReaderInterface() = default;
	virtual bool Open(const std::string& path) = 0;
	virtual int GetFrameCount() const = 0;
	virtual int GetPointCount(size_t frame_index) const = 0;
	virtual std::unique_ptr<FrameData> ReadFrame(size_t frame_index) = 0;
};
