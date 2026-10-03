#include "alembic_cache_reader.h"

#include <filesystem>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreAbstract/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/Abc/ErrorHandler.h>


struct AlembicCacheReader::Impl {
	Alembic::Abc::IArchive archive;
	std::map<std::string, Alembic::AbcGeom::IPolyMesh> meshes_by_path;

	void CollectMeshes(Alembic::Abc::IObject top_object) {
		auto num_children = top_object.getNumChildren();
		for (auto i = 0; i < num_children; ++i)
		{
			Alembic::Abc::IObject child = top_object.getChild(i);
			if (Alembic::AbcGeom::IPolyMesh::matches(child.getHeader())) {
				meshes_by_path[child.getFullName()] = Alembic::AbcGeom::IPolyMesh(child);
			}
			CollectMeshes(child);
		}
	}
};

AlembicCacheReader::AlembicCacheReader() : impl(std::make_unique<Impl>()) {}

AlembicCacheReader::~AlembicCacheReader() = default;

bool AlembicCacheReader::Open(const std::string& path)
{
	if (std::filesystem::exists(path) == false) {
		std::cout << std::format("The file {} doesn't exists. Exiting...", path) << std::endl;
		return false;
	}
		
	impl->archive = Alembic::Abc::IArchive(Alembic::AbcCoreOgawa::ReadArchive(), path);
	impl->CollectMeshes(impl->archive.getTop());
	return true;
}

int AlembicCacheReader::GetFrameCount() const
{
	if (!impl->archive) return 0;
	auto numTimeSamplings = impl->archive.getNumTimeSamplings();
	return static_cast<int>(impl->archive.getMaxNumSamplesForTimeSamplingIndex(1));
}

int AlembicCacheReader::GetPointCount(size_t frame_index) const
{
	for (const auto& a : impl->meshes_by_path)
	{
		std::cout << a.first << std::endl;
	}
	return 0;
}

std::unique_ptr<FrameData> AlembicCacheReader::ReadFrame(size_t frame_index)
{
	std::unique_ptr<FrameData> fd = std::make_unique<FrameData>();
	for (const auto& a : impl->meshes_by_path)
	{
		Alembic::AbcCoreAbstract::index_t frame_index_t = frame_index;
		Alembic::Abc::ISampleSelector selector(frame_index_t);
		Alembic::AbcGeom::IPolyMeshSchema::Sample sample;
		a.second.getSchema().get(sample, selector);
		auto positions = sample.getPositions();
		for (size_t i = 0; i < positions->size(); ++i) {
			const Imath::V3f& p = (*positions)[i];
			fd->positions.emplace_back(p.x * 10);
			fd->positions.emplace_back(p.y * 10);
			fd->positions.emplace_back(p.z * 10);
		}
	}
	return fd;
}
