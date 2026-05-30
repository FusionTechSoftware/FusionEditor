#ifndef __BOUNDS_CREATION_HEADER__
#define __BOUNDS_CREATION_HEADER__

#include <vector>
#include "../../../Resources/glm/glm.hpp"

struct BoundingBox
{
	glm::vec3 MinimumBounds = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 MaximumBounds = glm::vec3(0.0f, 0.0f, 0.0f);
};

class BoundsSystem
{
	public:
						BoundsSystem(void);
		BoundingBox		GenerateNewBounds(const std::vector<glm::vec3>& Vertices, const glm::vec3& Position);
		BoundingBox		UpdateBounds(const glm::vec3& NewModelPosition, BoundingBox NewBoundingBox);
	private:
		glm::vec3		StartModelPosition;

};

#endif