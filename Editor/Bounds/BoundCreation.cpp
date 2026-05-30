#include "BoundCreation.h"

BoundsSystem::BoundsSystem(void) {}

BoundingBox BoundsSystem::GenerateNewBounds(const std::vector<glm::vec3>& Vertices, const glm::vec3& Position)
{
	BoundingBox NewBoundingBox;
	NewBoundingBox.MaximumBounds	= Vertices[0];
	NewBoundingBox.MinimumBounds	= Vertices[0];

	this->StartModelPosition		= Position;

	for (int Index = 0; Index < Vertices.size(); Index++)
	{
		if (Vertices[Index].x < NewBoundingBox.MinimumBounds.x) { NewBoundingBox.MinimumBounds.x = Vertices[Index].x; }
		if (Vertices[Index].x > NewBoundingBox.MaximumBounds.x) { NewBoundingBox.MaximumBounds.x = Vertices[Index].x; }
		if (Vertices[Index].y < NewBoundingBox.MinimumBounds.y) { NewBoundingBox.MinimumBounds.y = Vertices[Index].y; }
		if (Vertices[Index].y > NewBoundingBox.MaximumBounds.y) { NewBoundingBox.MaximumBounds.y = Vertices[Index].y; }
		if (Vertices[Index].z < NewBoundingBox.MinimumBounds.z) { NewBoundingBox.MinimumBounds.z = Vertices[Index].z; }
		if (Vertices[Index].z > NewBoundingBox.MaximumBounds.z) { NewBoundingBox.MaximumBounds.z = Vertices[Index].z; }
	}

	return NewBoundingBox;
}

BoundingBox BoundsSystem::UpdateBounds(const glm::vec3& NewModelPosition, BoundingBox NewBoundingBox)
{
	NewBoundingBox.MaximumBounds += this->StartModelPosition - NewModelPosition;
	NewBoundingBox.MinimumBounds += this->StartModelPosition - NewModelPosition;

	this->StartModelPosition = NewModelPosition;
	return NewBoundingBox;
}