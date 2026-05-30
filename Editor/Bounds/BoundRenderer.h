#ifndef __BOUND_RENDERER__
#define __BOUND_RENDERER__

#include <vector>
#include "../../Common/Shader.h"

struct BoundObject
{
	unsigned int				VertexBuffer;
	unsigned int				VertexArray;
	unsigned int				ElementBuffer;

	std::vector<unsigned int>	Indices;
	std::vector<float>			Vertices;

	glm::mat4					TranslationMatrix;
	glm::mat4					RotationMatrix;
	glm::mat4					ScaleMatrix;
	Shader*						ObjectShader;
};

class BoundsRenderer
{
	public:
									BoundsRenderer(void);
		void						CreateBoundingBox(Shader* NewBoundShader, const glm::vec3& MinimumBounds, const glm::vec3& MaximumBounds, const glm::vec3& Position, const glm::quat& Rotation, const glm::vec3& Scale);
		void						DrawBoundingBox(const glm::mat4& Projection, const glm::mat4& View);
		void						DrawBoundingBoxIndexed(const glm::mat4& Projection, const glm::mat4& View, int Index);
		void						DestroyBoundRender(int Index);
	private:
		std::vector<BoundObject>	DrawableBounds;
};

#endif