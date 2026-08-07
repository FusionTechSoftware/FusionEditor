#include "EditorUtilites.h"

bool EditorUtilites::DoesRayIntersectTriangle(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& TrianglePointA, const glm::vec3& TrianglePointB, const glm::vec3& TrianglePointC, float& Triangle)
{
	const float Epsilon = 0.0000001f;

	glm::vec3 Edge1			= TrianglePointB - TrianglePointA;
	glm::vec3 Edge2			= TrianglePointC - TrianglePointA;
	glm::vec3 NewDirection	= glm::cross(RayDirection, Edge2);

	float A					= glm::dot(Edge1, NewDirection);
	if (fabs(A) < Epsilon)			{ return false; }

	float F			= 1.0f / A;
	glm::vec3 S		= RayOrigin - TrianglePointA;
	float U			= F * glm::dot(S, NewDirection);
	if (U < 0.0f || U > 1.0f)		{ return false; }

	glm::vec3 Q		= glm::cross(S, Edge1);
	float V			= F * glm::dot(RayDirection, Q);
	if (V < 0.0f || U + V > 1.0f)	{ return false; }

	Triangle = F * glm::dot(Edge2, Q);

	return Triangle > Epsilon;
}

bool EditorUtilites::DoesRayIntersectBounds(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& BoxMinimum, const glm::vec3& BoxMaximum, float& T)
{
	float TMin	= (BoxMinimum.x - RayOrigin.x) / RayDirection.x;
	float TMax	= (BoxMaximum.x - RayOrigin.x) / RayDirection.x;

	if (TMin > TMax)
	{
		std::swap(TMin, TMax);
	}

	float TyMin = (BoxMinimum.y - RayOrigin.y) / RayDirection.y;
	float TyMax = (BoxMaximum.y - RayOrigin.y) / RayDirection.y;

	if (TyMin > TyMax)
	{
		std::swap(TyMin, TyMax);
	}

	if ((TMin > TyMax) || (TyMin > TMax)) return false;
	if (TyMin > TMin) TMin = TyMin;
	if (TyMax < TMax) TMax = TyMax;

	float TzMin = (BoxMinimum.z - RayOrigin.z) / RayDirection.z;
	float TzMax = (BoxMaximum.z - RayOrigin.z) / RayDirection.z;

	if (TzMin > TzMax)
	{
		std::swap(TzMin, TzMax);
	}

	if ((TMin > TzMax) || (TzMin > TMax)) return false;
	if (TzMin > TMin) TMin = TzMin;
	if (TzMax < TMax) TMax = TzMax;

	T = TMin;
	return true;
}

int EditorUtilites::PickVertexFromMesh(const std::vector<VertexPoint>& Vertices, const glm::vec2& MousePosition, const glm::mat4& View, const glm::mat4& Projection, const glm::mat4& Model, int Width, int Height)
{
	float MinimumDistance	= 25.0f;
	int BestPoint			= -1;

	for (int Index = 0; Index < Vertices.size(); Index++)
	{
		glm::vec3 WorldPosition = glm::vec3(Model * glm::vec4(Vertices[Index].Position, 1.0f));
		glm::vec2 Screen		= EditorUtilites::WorldToScreen(WorldPosition, View, Projection, Width, Height);
		float Distance			= glm::length(Screen - MousePosition);
		
		if (Distance < MinimumDistance)
		{
			MinimumDistance = Distance;
			BestPoint		= Index;
		}
	}
	
	return BestPoint;
}

float EditorUtilites::DistanceRayPoint(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::vec3& Point)
{
	glm::vec3 DirectionOfPoint	= Point - RayOrigin;
	float T						= glm::dot(DirectionOfPoint, RayDirection);
	if (T < 0.0f) { return FLT_MAX; }

	glm::vec3 Closest = RayOrigin + RayDirection * T;

	return glm::length(Point - Closest);
}

void EditorUtilites::ConvertStringToLowercase(std::string& NewString)
{
	for (char& Character : NewString)
	{
		if (Character >= 'A' && Character <= 'Z')
		{
			Character += 32;
		}
	}
}

std::vector<VertexPoint> EditorUtilites::ConvertVertexToPosition(const std::vector<float>& Vertices)
{
	std::vector<VertexPoint> EditorPoint = {};

	for (int Index = 0; Index < Vertices.size(); Index+=8)
	{
		VertexPoint CurrentPoint;

		CurrentPoint.Position	= glm::vec3(Vertices[Index], Vertices[Index + 1], Vertices[Index + 2]);
		CurrentPoint.Normal		= glm::vec3(Vertices[Index + 3], Vertices[Index + 4], Vertices[Index + 5]);
		CurrentPoint.TextureUV	= glm::vec2(Vertices[Index + 6], Vertices[Index + 7]);
	
		EditorPoint.push_back(CurrentPoint);
	}

	return EditorPoint;
}

std::vector<glm::vec3> EditorUtilites::GroupVector3(const std::vector<float>& FloatVector)
{
	std::vector<glm::vec3> Vectors;
	for (int Index = 0; Index < FloatVector.size(); Index+=3)
	{
		Vectors.push_back(glm::vec3(FloatVector[Index],FloatVector[Index+1],FloatVector[Index+2]));
	}

	return Vectors;
}

std::vector<float> EditorUtilites::GetPosition(const std::vector<float>& FloatVector)
{
	std::vector<float> NewVector;
	
	for (int Index = 0; Index < FloatVector.size(); Index++)
	{
		NewVector.push_back(FloatVector[Index]);
		NewVector.push_back(FloatVector[Index + 1]);
		NewVector.push_back(FloatVector[Index + 2]);
	}

	return NewVector;
}

std::vector<VertexPoint> EditorUtilites::GetWorldPointsFromMatrix(const std::vector<float>& Vertices, const glm::mat4& NewMatrix)
{
	std::vector<VertexPoint> NewPoints = {};

	for (int Index = 0; Index < Vertices.size(); Index+=8)
	{
		VertexPoint CurrentPoints;

		CurrentPoints.Position	= glm::vec3(NewMatrix * glm::vec4(Vertices[Index], Vertices[Index + 1], Vertices[Index + 2], 1.0f));
		CurrentPoints.Normal	= glm::vec3(Vertices[Index+3],Vertices[Index+4],Vertices[Index+5]);
		CurrentPoints.TextureUV = glm::vec2(Vertices[Index+6],Vertices[Index+7]);
		
		NewPoints.push_back(CurrentPoints);
	}

	return NewPoints;
}

std::vector<float> EditorUtilites::ConvertPositionToVertex(const std::vector<VertexPoint>& Vertices)
{
	std::vector<float> EditorPoints = {};

	for (int Index = 0; Index < Vertices.size(); Index++)
	{
		EditorPoints.push_back(Vertices[Index].Position.x);
		EditorPoints.push_back(Vertices[Index].Position.y);
		EditorPoints.push_back(Vertices[Index].Position.z);

		EditorPoints.push_back(Vertices[Index].Normal.x);
		EditorPoints.push_back(Vertices[Index].Normal.y);
		EditorPoints.push_back(Vertices[Index].Normal.z);

		EditorPoints.push_back(Vertices[Index].TextureUV.x);
		EditorPoints.push_back(Vertices[Index].TextureUV.y);
	}

	return EditorPoints;
}

RayResult EditorUtilites::GetRayAndDirectionFromMouse(const glm::mat4& Projection, const glm::mat4& View, double MouseX, double MouseY, int Width, int Height)
{
	RayResult NewResult;
	float X = (1.0f * MouseX) / Width - 1.0f;
	float Y = 1.0f - (2.0f * MouseY) / Width;

	glm::vec4 NearNDC	= glm::vec4(X, Y, -1.0f, 1.0f);
	glm::vec4 FarNDC	= glm::vec4(X, Y, 1.0f, 1.0f);
	glm::mat4 InverseVP = glm::inverse(Projection * View);

	glm::vec4 NearWorld = InverseVP * NearNDC; NearWorld /= NearWorld.w;
	glm::vec4 FarWorld	= InverseVP * FarNDC;  FarWorld /= FarWorld.w;

	glm::vec3 Origin	= glm::vec3(NearWorld);
	glm::vec3 Direction = glm::normalize(glm::vec3(FarWorld - NearWorld));

	NewResult.Origin	= Origin;
	NewResult.Direction = Direction;

	return NewResult;
}

glm::vec2 EditorUtilites::WorldToScreen(const glm::vec3& Position, const glm::mat4& View, const glm::mat4& Projection, int Width, int Height)
{
	glm::vec2 Screen	= glm::vec2(0.0f);
	glm::vec4 Clip		= Projection * View * glm::vec4(Position, 1.0f);
	Clip /= Clip.w;

	Screen.x = (Clip.x * 0.5f + 0.5f) * Width;
	Screen.y = (1.0f - (Clip.y * 0.5f + 0.5f)) * Height;

	return Screen;
}

glm::vec3 EditorUtilites::GetTriangleNormalFromWorldPosition(const glm::vec3& Vertex0, const glm::vec3& Vertex1, const glm::vec3& Vertex2)
{
	return glm::normalize(glm::cross(Vertex1 - Vertex0, Vertex2 - Vertex0));
}

glm::vec3 EditorUtilites::GetRayFromMouse(const glm::mat4& Projection, const glm::mat4& View, double MouseX, double MouseY, int Width, int Height)
{
#if 1
	float X = (1.0f * MouseX) / Width - 1.0f;
	float Y = 1.0f - (2.0f * MouseY) / Height;

	glm::vec4 NearNDC = glm::vec4(MouseX, MouseY, -1.0f, 1.0f);
	glm::vec4 FarNDC = glm::vec4(MouseX, MouseY, 1.0f, 1.0f);
	glm::mat4 InverseVP = glm::inverse(Projection * View);

	glm::vec4 NearWorld = InverseVP * NearNDC; NearWorld /= NearWorld.w;
	glm::vec4 FarWorld = InverseVP * FarNDC;  FarWorld /= FarWorld.w;

	glm::vec3 Origin = glm::vec3(NearWorld);
	glm::vec3 Direction = glm::normalize(glm::vec3(FarWorld - NearWorld));

	return Direction;
#else

	float X = (2.0f * MouseX) / Width - 1.0f;
	float Y = 1.0f - (2.0f * MouseY) / Height;

	glm::vec4 RaycastClip = glm::vec4(X, Y, -1.0f, 1.0f);
	glm::vec4 RayEye = glm::inverse(Projection) * RaycastClip;

	RayEye = glm::vec4(RayEye.x, RayEye.y, -1.0f, 0.0f);
	
	glm::vec3 RayWorld = glm::normalize(glm::vec3(glm::inverse(View) * RayEye));

	return RayWorld;
#endif

}


//glm::vec3 EditorUtilites::GetRayFromMouse(
//	const glm::mat4& projection,
//	const glm::mat4& view,
//	double mouseX,
//	double mouseY,
//	int width,
//	int height)
//{
//	float x = (2.0f * mouseX) / width - 1.0f;
//	float y = 1.0f - (2.0f * mouseY) / height;
//
//	glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
//
//	glm::vec4 rayEye = glm::inverse(projection) * rayClip;
//	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
//
//	glm::vec3 rayWorld =
//		glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
//
//	return rayWorld;
//}