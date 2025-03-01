/**
 * @file
 */

#include "TestHelper.h"
#include "core/Common.h"
#include "math/Random.h"
#include "voxel/MaterialColor.h"
#include "voxel/Palette.h"
#include "voxel/RawVolume.h"
#include "voxel/Voxel.h"
#include "voxel/tests/VoxelPrinter.h"
#include "voxelformat/MeshFormat.h"
#include "scenegraph/SceneGraph.h"
#include "scenegraph/SceneGraphNode.h"
#include "voxelutil/VolumeVisitor.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace glm {
::std::ostream &operator<<(::std::ostream &os, const mat4x4 &matrix) {
	os << to_string(matrix);
	return os;
}
::std::ostream &operator<<(::std::ostream &os, const mat3x3 &matrix) {
	os << to_string(matrix);
	return os;
}
::std::ostream &operator<<(::std::ostream &os, const mat4x3 &matrix) {
	os << to_string(matrix);
	return os;
}
::std::ostream &operator<<(::std::ostream &os, const vec2 &v) {
	os << to_string(v);
	return os;
}
::std::ostream &operator<<(::std::ostream &os, const vec3 &v) {
	os << to_string(v);
	return os;
}
::std::ostream &operator<<(::std::ostream &os, const vec4 &v) {
	os << to_string(v);
	return os;
}
::std::ostream &operator<<(::std::ostream &os, const ivec2 &v) {
	os << to_string(v);
	return os;
}
::std::ostream &operator<<(::std::ostream &os, const ivec3 &v) {
	os << to_string(v);
	return os;
}
::std::ostream &operator<<(::std::ostream &os, const ivec4 &v) {
	os << to_string(v);
	return os;
}
}

namespace voxel {

::std::ostream &operator<<(::std::ostream &os, const voxel::Palette &palette) {
	return os << voxel::Palette::print(palette).c_str();
}

int countVoxels(const voxel::RawVolume& volume, const voxel::Voxel &voxel) {
	int cnt = 0;
	voxelutil::visitVolume(volume, [&](int, int, int, const voxel::Voxel &v) {
		if (v == voxel) {
			++cnt;
		}
	}, voxelutil::VisitAll());
	return cnt;
}

void paletteComparator(const voxel::Palette &pal1, const voxel::Palette &pal2, float maxDelta) {
	ASSERT_EQ(pal1.colorCount(), pal2.colorCount());
	for (int i = 0; i < pal1.colorCount(); ++i) {
		const core::RGBA& c1 = pal1.color(i);
		const core::RGBA& c2 = pal2.color(i);
		const float delta = core::Color::getDistance(c1, c2);
		ASSERT_LT(delta, maxDelta) << "Palette color differs at " << i << ", color1[" << core::Color::print(c1)
								   << "], color2[" << core::Color::print(c2) << "], delta[" << delta << "]"
								   << "\nPalette 1:\n"
								   << voxel::Palette::print(pal1)
								   << "\nPalette 2:\n"
								   << voxel::Palette::print(pal2);
	}
}

void keyFrameComparator(const scenegraph::SceneGraphKeyFrames &keyframes1, const scenegraph::SceneGraphKeyFrames &keyframes2, ValidateFlags flags) {
	if ((flags & ValidateFlags::Animations) == ValidateFlags::Animations) {
		ASSERT_EQ(keyframes1.size(), keyframes2.size());
		for (size_t i = 0; i < keyframes1.size(); ++i) {
			ASSERT_EQ(keyframes1[i].frameIdx, keyframes2[i].frameIdx);
			ASSERT_EQ(keyframes1[i].longRotation, keyframes2[i].longRotation);
			ASSERT_EQ(keyframes1[i].interpolation, keyframes2[i].interpolation);
			const scenegraph::SceneGraphTransform &t1 = keyframes1[i].transform();
			const scenegraph::SceneGraphTransform &t2 = keyframes2[i].transform();
			ASSERT_FALSE(t1.dirty()) << "Key frame " << i << " is not yet updated";
			ASSERT_FALSE(t2.dirty()) << "Key frame " << i << " is not yet updated";
			if ((flags & ValidateFlags::Translation) == ValidateFlags::Translation) {
				ASSERT_EQ(t1.worldTranslation(), t2.worldTranslation()) << "Translation failed for frame " << i;
				ASSERT_EQ(t1.localTranslation(), t2.localTranslation()) << "Translation failed for frame " << i;
				ASSERT_EQ(t1.worldMatrix(), t2.worldMatrix()) << "Matrix failed for frame " << i;
				ASSERT_EQ(t1.localMatrix(), t2.localMatrix()) << "Matrix failed for frame " << i;
			} else {
				const glm::mat3x3 wrot1 = t1.worldMatrix();
				const glm::mat3x3 wrot2 = t2.worldMatrix();
				const glm::mat3x3 lrot1 = t1.localMatrix();
				const glm::mat3x3 lrot2 = t2.localMatrix();
				ASSERT_EQ(wrot1, wrot2) << "Matrix failed for frame " << i;
				ASSERT_EQ(lrot1, lrot2) << "Matrix failed for frame " << i;

			}
			if ((flags & ValidateFlags::Scale) == ValidateFlags::Scale) {
				ASSERT_EQ(t1.worldScale(), t2.worldScale()) << "Scale failed for frame " << i;
				ASSERT_EQ(t1.localScale(), t2.localScale()) << "Scale failed for frame " << i;
			}
		}
	} else {
		ASSERT_GE(keyframes1.size(), 1u) << "keyframes 1 doesn't have any entry";
		ASSERT_GE(keyframes2.size(), 1u) << "keyframes 2 doesn't have any entry";
		ASSERT_EQ(keyframes1[0].frameIdx, keyframes2[0].frameIdx);
		ASSERT_EQ(keyframes1[0].longRotation, keyframes2[0].longRotation);
		ASSERT_EQ(keyframes1[0].interpolation, keyframes2[0].interpolation);
		const scenegraph::SceneGraphTransform &t1 = keyframes1[0].transform();
		const scenegraph::SceneGraphTransform &t2 = keyframes2[0].transform();
		ASSERT_FALSE(t1.dirty()) << "Key frame 0 is not yet updated";
		ASSERT_FALSE(t2.dirty()) << "Key frame 0 is not yet updated";
		if ((flags & ValidateFlags::Translation) == ValidateFlags::Translation) {
			ASSERT_EQ(t1.worldTranslation(), t2.worldTranslation()) << "Translation failed for frame 0";
		}
	}
}

void volumeComparator(const voxel::RawVolume& volume1, const voxel::Palette &pal1, const voxel::RawVolume& volume2, const voxel::Palette &pal2, ValidateFlags flags, float maxDelta) {
	const Region& r1 = volume1.region();
	const Region& r2 = volume2.region();
	if ((flags & ValidateFlags::Region) == ValidateFlags::Region) {
		ASSERT_EQ(r1, r2) << "regions differ: " << r1.toString() << " vs " << r2.toString();
	}

	const int32_t lowerX = r1.getLowerX();
	const int32_t lowerY = r1.getLowerY();
	const int32_t lowerZ = r1.getLowerZ();
	const int32_t upperX = r1.getUpperX();
	const int32_t upperY = r1.getUpperY();
	const int32_t upperZ = r1.getUpperZ();
	const int32_t lower2X = r2.getLowerX();
	const int32_t lower2Y = r2.getLowerY();
	const int32_t lower2Z = r2.getLowerZ();
	const int32_t upper2X = r2.getUpperX();
	const int32_t upper2Y = r2.getUpperY();
	const int32_t upper2Z = r2.getUpperZ();

	voxel::RawVolume::Sampler s1(volume1);
	voxel::RawVolume::Sampler s2(volume2);
	for (int32_t z1 = lowerZ, z2 = lower2Z; z1 <= upperZ && z2 <= upper2Z; ++z1, ++z2) {
		for (int32_t y1 = lowerY, y2 = lower2Y; y1 <= upperY && y2 <= upper2Y; ++y1, ++y2) {
			for (int32_t x1 = lowerX, x2 = lower2X; x1 <= upperX && x2 <= upper2X; ++x1, ++x2) {
				s1.setPosition(x1, y1, z1);
				s2.setPosition(x2, y2, z2);
				const voxel::Voxel& voxel1 = s1.voxel();
				const voxel::Voxel& voxel2 = s2.voxel();
				ASSERT_EQ(voxel1.getMaterial(), voxel2.getMaterial())
					<< "Voxel differs at " << x1 << ":" << y1 << ":" << z1 << " in material - voxel1["
					<< voxel::VoxelTypeStr[(int)voxel1.getMaterial()] << ", " << (int)voxel1.getColor() << "], voxel2["
					<< voxel::VoxelTypeStr[(int)voxel2.getMaterial()] << ", " << (int)voxel2.getColor() << "], color1["
					<< core::Color::print(voxel1.getColor()) << "], color2[" << core::Color::print(voxel2.getColor())
					<< "]";
				if (voxel::isAir(voxel1.getMaterial())) {
					continue;
				}
				if ((flags & ValidateFlags::Color) != ValidateFlags::Color) {
					continue;
				}

				// TODO: could get improved by checking if the current voxel is surrounded by others on all sides
				if ((flags & ValidateFlags::IgnoreHollow) == ValidateFlags::IgnoreHollow &&
					voxel2.getColor() == voxelformat::MeshFormat::FillColorIndex &&
					voxel1.getColor() != voxelformat::MeshFormat::FillColorIndex) {
					continue;
				}

				const core::RGBA& c1 = pal1.color(voxel1.getColor());
				const core::RGBA& c2 = pal2.color(voxel2.getColor());
				const float delta = core::Color::getDistance(c1, c2);
				ASSERT_LT(delta, maxDelta)
					<< "Voxel differs at " << x1 << ":" << y1 << ":" << z1 << " in material - voxel1["
					<< voxel::VoxelTypeStr[(int)voxel1.getMaterial()] << ", " << (int)voxel1.getColor() << "], voxel2["
					<< voxel::VoxelTypeStr[(int)voxel2.getMaterial()] << ", " << (int)voxel2.getColor() << "], color1["
					<< core::Color::print(c1) << "], color2[" << core::Color::print(c2) << "], delta[" << delta << "]\n"
					<< voxel::Palette::print(pal1) << "\n"
					<< voxel::Palette::print(pal2);
			}
		}
	}
}

void sceneGraphComparator(const scenegraph::SceneGraph &graph1, const scenegraph::SceneGraph &graph2, ValidateFlags flags, float maxDelta) {
	ASSERT_EQ(graph1.size(), graph2.size());
	const int n = (int)graph1.size();
	for (int i = 0; i < n; ++i) {
		const scenegraph::SceneGraphNode *node1 = graph1[i];
		ASSERT_NE(nullptr, node1);
		const scenegraph::SceneGraphNode *node2 = graph2[i];
		ASSERT_NE(nullptr, node2);
		if ((flags & ValidateFlags::Palette) == ValidateFlags::Palette) {
			paletteComparator(node1->palette(), node2->palette(), maxDelta);
		}
		// it's intended that includingRegion is false here!
		volumeComparator(*node1->volume(), node1->palette(), *node2->volume(), node2->palette(), flags, maxDelta);
		if ((flags & ValidateFlags::Pivot) == ValidateFlags::Pivot) {
			ASSERT_EQ(node1->pivot(), node2->pivot()) << "Pivot failed";
		}
		keyFrameComparator(node1->keyFrames(), node2->keyFrames(), flags);
	}
}

}
