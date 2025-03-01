/**
 * @file
 */

#pragma once

#include "Modifier.h"
#include "ModifierRenderer.h"
#include "core/IComponent.h"

namespace voxedit {

class ModifierFacade : public Modifier {
private:
	using Super = Modifier;
	ModifierRenderer _modifierRenderer;

public:
	bool init() override;
	void shutdown() override;

	bool select(const glm::ivec3& mins, const glm::ivec3& maxs) override;
	void unselect() override;
	void invert(const voxel::Region &region) override;
	void setReferencePosition(const glm::ivec3& pos) override;
	bool setMirrorAxis(math::Axis axis, const glm::ivec3 &mirrorPos) override;
	void render(const video::Camera& camera);
};

}
