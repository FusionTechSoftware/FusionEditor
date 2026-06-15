#ifndef __GRID_EDITOR_WINDOW_HEADER__
#define __GRID_EDITOR_WINDOW_HEADER__

#include <vector>
#include "../../../../Resources/glm/glm.hpp"
#include "../../../../Resources/imgui/imgui.h"
#include "../../../Utilites/ImGuiUtilites.h"

#define GRID_STEP 32.0f
#define CLOSE_DISTANCE 32.0f * 0.5f

enum ViewMode
{
	VIEW_NULL = 0,
	VIEW_TOP,
	VIEW_FRONT,
	VIEW_SIDE
};

struct GeometryEdge
{
	int Start;
	int End;
};

// This would be the structure of the face sector needed to render 3D geometries in 2D space.
struct GridFaceSector
{
	std::vector<glm::vec3>		Vertices;
	std::vector<unsigned int>	Indices;
	std::vector<GeometryEdge>	FaceEdges;
};

// Helper function for IMGUI vector 2 addition as it doesnt come with it.

inline ImVec2 operator+(const ImVec2& ObjectA, const ImVec2& ObjectB)
{
	return ImVec2(ObjectA.x + ObjectB.x, ObjectA.y + ObjectB.y);
}

inline ImVec2 operator-(const ImVec2& ObjectA, const ImVec2& ObjectB)
{
	return ImVec2(ObjectA.x - ObjectB.x, ObjectA.y - ObjectB.y);
}

inline ImVec2 operator*(const ImVec2& ObjectA, float ObjectB)
{
	return ImVec2(ObjectA.x * ObjectB, ObjectA.y * ObjectB);
}

class GridEditor
{
	public:
										GridEditor(void);
		void							DrawGrids(void);
		void							SubmitWorldGeometry(const std::vector<glm::vec3>& Vertices, const std::vector<unsigned int> Indices);
	private:
		void							GenerateGeometry(const ImVector<ImVec2>& Points, ViewMode CurrentViewMode);
		float							FloorHeight;

		glm::vec3						GridToWorldSpace(const ImVec2& Point, ViewMode CurrentViewMode);
		ImVec2							WorldToGridSpace(const glm::vec3& Point, ViewMode CurrentViewMode);
		std::vector<GridFaceSector>		GridSectors;
};


#endif
