/**
 * @file
 */

#pragma once

namespace voxel {

class Mesh;
class RawVolume;
class Region;
class Palette;

void extractDualContouringMesh(voxel::RawVolume *volData, const Palette &palette, const Region &region, Mesh *mesh);

} // namespace voxel
