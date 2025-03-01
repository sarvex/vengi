/**
 * @file
 */

#include "app/tests/AbstractTest.h"
#include "core/ScopedPtr.h"
#include "voxel/MaterialColor.h"
#include "voxelutil/VolumeRotator.h"
#include "voxel/tests/VoxelPrinter.h"

namespace voxelutil {

class VolumeRotatorTest: public app::AbstractTest {
protected:
	inline core::String str(const voxel::Region& region) const {
		return region.toString();
	}
};

TEST_F(VolumeRotatorTest, DISABLED_testRotateAxisZ) {
	const voxel::Region region(-1, 1);
	voxel::RawVolume smallVolume(region);
	EXPECT_TRUE(smallVolume.setVoxel(0, 0, 0, voxel::createVoxel(voxel::VoxelType::Generic, 1)));
	EXPECT_TRUE(smallVolume.setVoxel(0, 1, 0, voxel::createVoxel(voxel::VoxelType::Generic, 1)));
	EXPECT_TRUE(smallVolume.setVoxel(1, 0, 0, voxel::createVoxel(voxel::VoxelType::Generic, 1)));
	core::ScopedPtr<voxel::RawVolume> rotated(voxelutil::rotateAxis(&smallVolume, math::Axis::Y));
	ASSERT_NE(nullptr, rotated) << "No new volume was returned for the desired rotation";
	ASSERT_EQ(voxel::VoxelType::Generic, rotated->voxel(0, 0, 0).getMaterial()) << smallVolume << "rotated: " << *rotated;
	ASSERT_EQ(voxel::VoxelType::Generic, rotated->voxel(-1, 0, 0).getMaterial()) << smallVolume << "rotated: " << *rotated;
	ASSERT_EQ(voxel::VoxelType::Generic, rotated->voxel(0, 1, 0).getMaterial()) << smallVolume << "rotated: " << *rotated;
}

TEST_F(VolumeRotatorTest, testRotateAxisY) {
	const voxel::Region region(-1, 1);
	voxel::RawVolume smallVolume(region);
	EXPECT_TRUE(smallVolume.setVoxel(0, 0, 0, voxel::createVoxel(voxel::VoxelType::Generic, 1)));
	EXPECT_TRUE(smallVolume.setVoxel(0, 1, 0, voxel::createVoxel(voxel::VoxelType::Generic, 1)));
	EXPECT_TRUE(smallVolume.setVoxel(1, 0, 0, voxel::createVoxel(voxel::VoxelType::Generic, 1)));
	core::ScopedPtr<voxel::RawVolume> rotated(voxelutil::rotateAxis(&smallVolume, math::Axis::Y));
	ASSERT_NE(nullptr, rotated) << "No new volume was returned for the desired rotation";
	ASSERT_EQ(voxel::VoxelType::Generic, rotated->voxel(0, 0, 0).getMaterial()) << smallVolume << "rotated: " << *rotated;
	ASSERT_EQ(voxel::VoxelType::Generic, rotated->voxel(0, 1, 0).getMaterial()) << smallVolume << "rotated: " << *rotated;
	ASSERT_EQ(voxel::VoxelType::Generic, rotated->voxel(0, 0, -1).getMaterial()) << smallVolume << "rotated: " << *rotated;
}

TEST_F(VolumeRotatorTest, DISABLED_testRotateAxisX) {
	const voxel::Region region(-1, 1);
	voxel::RawVolume smallVolume(region);
	EXPECT_TRUE(smallVolume.setVoxel(0, 0, 0, voxel::createVoxel(voxel::VoxelType::Generic, 1)));
	EXPECT_TRUE(smallVolume.setVoxel(0, 1, 0, voxel::createVoxel(voxel::VoxelType::Generic, 1)));
	EXPECT_TRUE(smallVolume.setVoxel(1, 0, 0, voxel::createVoxel(voxel::VoxelType::Generic, 1)));
	core::ScopedPtr<voxel::RawVolume> rotated(voxelutil::rotateAxis(&smallVolume, math::Axis::Y));
	ASSERT_NE(nullptr, rotated) << "No new volume was returned for the desired rotation";
	ASSERT_EQ(voxel::VoxelType::Generic, rotated->voxel(0, 0, 0).getMaterial()) << smallVolume << "rotated: " << *rotated;
	ASSERT_EQ(voxel::VoxelType::Generic, rotated->voxel(0, 0, 1).getMaterial()) << smallVolume << "rotated: " << *rotated;
	ASSERT_EQ(voxel::VoxelType::Generic, rotated->voxel(1, 0, 0).getMaterial()) << smallVolume << "rotated: " << *rotated;
}

TEST_F(VolumeRotatorTest, DISABLED_testRotate45Y) {
	const voxel::Region region(0, 10);
	voxel::RawVolume smallVolume(region);
	glm::ivec3 pos = region.getCenter();
	EXPECT_TRUE(smallVolume.setVoxel(pos.x, pos.y++, pos.z, voxel::createVoxel(voxel::VoxelType::Generic, 0)));
	EXPECT_TRUE(smallVolume.setVoxel(pos.x, pos.y++, pos.z, voxel::createVoxel(voxel::VoxelType::Generic, 0)));
	EXPECT_TRUE(smallVolume.setVoxel(pos.x, pos.y++, pos.z, voxel::createVoxel(voxel::VoxelType::Generic, 0)));

	core::ScopedPtr<voxel::RawVolume> rotated(voxelutil::rotateAxis(&smallVolume, math::Axis::Y));
	ASSERT_NE(nullptr, rotated) << "No new volume was returned for the desired rotation";
	const voxel::Region& rotatedRegion = rotated->region();
	EXPECT_NE(rotatedRegion, region) << "Rotating by 45 degree should increase the size of the volume "
			<< str(rotatedRegion) << " " << str(region);
}

}
