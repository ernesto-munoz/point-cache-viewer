#include "usd_cache_reader.h"
#include <iostream>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usd/primRange.h>

#include "core/utils.h"

PXR_NAMESPACE_USING_DIRECTIVE

struct USDCacheReader::Impl {
	UsdStageRefPtr stage;
	std::vector<double> timeSamples;
	std::map<SdfPath, UsdGeomMesh> meshes_by_path;

	void CollectMeshes() {
		for (UsdPrim prim : stage->Traverse()) {
			if (prim.IsA<UsdGeomMesh>()) {
				UsdGeomMesh mesh(prim);
				meshes_by_path[prim.GetPath()] = mesh;
			}
		}
	}
};

USDCacheReader::USDCacheReader() : impl(std::make_unique<Impl>()) {}

USDCacheReader::~USDCacheReader() = default;

bool USDCacheReader::Open(const std::string& path)
{
	{
		core::ElapsedTime e("Open USD Cache Reader {} ms");
		impl->stage = UsdStage::Open(path);
		if (!impl->stage) return false;
		impl->CollectMeshes();
		std::cout << "Opened: " << path << std::endl;
	
	}
	
	return true;
}

int USDCacheReader::GetFrameCount() const
{
	return (int)impl->timeSamples.size();
}

int USDCacheReader::GetPointCount(size_t frame_index) const
{
	return 0;
}

FrameData USDCacheReader::ReadFrame(size_t frame_index)
{
	return FrameData();
}
