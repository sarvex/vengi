/**
 * @file
 */

#pragma once

#include "app/CommandlineApp.h"
#include "scenegraph/SceneGraph.h"

/**
 * @brief This tool is able to convert voxel volumes between different formats
 *
 * @ingroup Tools
 */
class VoxConvert: public app::CommandlineApp {
private:
	using Super = app::CommandlineApp;
	core::VarPtr _mergeQuads;
	core::VarPtr _reuseVertices;
	core::VarPtr _ambientOcclusion;
	core::VarPtr _scale;
	core::VarPtr _scaleX;
	core::VarPtr _scaleY;
	core::VarPtr _scaleZ;
	core::VarPtr _quads;
	core::VarPtr _withColor;
	core::VarPtr _withTexCoords;

	bool _mergeVolumes = false;
	bool _scaleVolumes = false;
	bool _mirrorVolumes = false;
	bool _rotateVolumes = false;
	bool _translateVolumes = false;
	bool _exportPalette = false;
	bool _exportLayers = false;
	bool _cropVolumes = false;
	bool _splitVolumes = false;
	bool _dumpSceneGraph = false;
	bool _resizeVolumes = false;

protected:
	glm::ivec3 getArgIvec3(const core::String &name);
	core::String getFilenameForLayerName(const core::String& inputfile, const core::String &layerName, int id);
	bool handleInputFile(const core::String &infile, scenegraph::SceneGraph &sceneGraph, bool multipleInputs);

	void usage() const override;
	void mirror(const core::String& axisStr, scenegraph::SceneGraph& sceneGraph);
	void rotate(const core::String& axisStr, scenegraph::SceneGraph& sceneGraph);
	void scale(scenegraph::SceneGraph& sceneGraph);
	void resize(const glm::ivec3 &size, scenegraph::SceneGraph& sceneGraph);
	void script(const core::String &scriptParameters, scenegraph::SceneGraph& sceneGraph, uint8_t color);
	void translate(const glm::ivec3& pos, scenegraph::SceneGraph& sceneGraph);
	void crop(scenegraph::SceneGraph& sceneGraph);
	int dumpNode_r(const scenegraph::SceneGraph& sceneGraph, int nodeId, int indent);
	void dump(const scenegraph::SceneGraph& sceneGraph);
	void filterVolumes(scenegraph::SceneGraph& sceneGraph);
	void exportLayersIntoSingleObjects(scenegraph::SceneGraph& sceneGraph, const core::String &inputfile);
	void split(const glm::ivec3 &size, scenegraph::SceneGraph& sceneGraph);
public:
	VoxConvert(const io::FilesystemPtr& filesystem, const core::TimeProviderPtr& timeProvider);

	app::AppState onConstruct() override;
	app::AppState onInit() override;
};
