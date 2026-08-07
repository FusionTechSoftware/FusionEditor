#ifndef __EDITOR_UTILITES_HEADER__
#define __EDITOR_UTILITES_HEADER__

#include <vector>
#include <string>
#include "../../Resources/glm/glm.hpp"

enum SyntaxConditions
{
	SYNTAX_UNKNOWN = 0,
	SYNTAX_FUNCTION,
	SYNTAX_BITWISE_FUNCTION,
	SYNTAX_DATATYPE
};

struct VertexPoint
{
	glm::vec3	Position;
	glm::vec3	Normal;
	glm::vec2	TextureUV;
};

struct RayResult
{
	glm::vec3	Origin;
	glm::vec3	Direction;
};

struct SyntaxHighlightLayout
{
	glm::vec4			HighlightColour;
	SyntaxConditions	SyntaxType;
};

class EditorUtilites
{
	public:
		static bool							DoesRayIntersectTriangle(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& TrianglePointA, const glm::vec3& TrianglePointB, const glm::vec3& TrianglePointC, float& Triangle);
		static bool							DoesRayIntersectBounds(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& BoxMinimum, const glm::vec3& BoxMaximum, float& T);
		static int							PickVertexFromMesh(const std::vector<VertexPoint>& Vertices, const glm::vec2& MousePosition, const glm::mat4& View, const glm::mat4& Projection, const glm::mat4& Model, int Width, int Height);
		static float						DistanceRayPoint(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& Point);
		static void							ConvertStringToLowercase(std::string& NewString);
		static glm::vec2					WorldToScreen(const glm::vec3& Position, const glm::mat4& View, const glm::mat4& Projection, int Width, int Height);
		static glm::vec3					GetRayFromMouse(const glm::mat4& Projection, const glm::mat4& View, double MouseX, double MouseY, int Width, int Height);
		static glm::vec3					GetTriangleNormalFromWorldPosition(const glm::vec3& Vertex0, const glm::vec3& Vertex1, const glm::vec3& Vertex2);
		static std::vector<VertexPoint>		ConvertVertexToPosition(const std::vector<float>& Vertices);
		static std::vector<VertexPoint>		GetWorldPointsFromMatrix(const std::vector<float>& Vertices, const glm::mat4& NewMatrix);
		static std::vector<float>			ConvertPositionToVertex(const std::vector<VertexPoint>& Vertices);
		static std::vector<float>			GetPosition(const std::vector<float>& FloatVector);
		static std::vector<glm::vec3>		GroupVector3(const std::vector<float>& FloatVector);
		static RayResult					GetRayAndDirectionFromMouse(const glm::mat4& Projection, const glm::mat4& View, double MouseX, double MouseY, int Width, int Height);
};

#endif

