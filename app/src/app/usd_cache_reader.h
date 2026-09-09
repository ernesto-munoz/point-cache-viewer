#pragma once

#include <memory>
#include "cache_reader.h"

class USDCacheReader : public CacheReaderInterface {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	USDCacheReader();
	~USDCacheReader() override;

	bool Open(const std::string& path) override;
	virtual int GetFrameCount() const override;
	virtual int GetPointCount(size_t frame_index) const override;
	virtual FrameData ReadFrame(size_t frame_index) override;
};