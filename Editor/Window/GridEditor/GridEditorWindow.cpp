#include "GridEditorWindow.h"
#include "../../../Common/Console.h"

#include <iostream>

GridEditor::GridEditor(void) 
{
	this->LastPressedKey	= 0.0;
	this->FloorHeight		= 128.0f;
	this->GridSectors		= {};
};

glm::vec3 GridEditor::GridToWorldSpace(const ImVec2& Point, ViewMode CurrentViewMode)
{
	// This transforms 2D points into 3D points (this would be normalised, hence please multiply into world space)
	// Or perhaps we could move this into world space to edit, so we can directly edit fully in world space
	// Easier and saves time!

	switch (CurrentViewMode)
	{
		case VIEW_TOP:		return glm::vec3(Point.x, 0.0f, Point.y);
		case VIEW_FRONT:	return glm::vec3(Point.x, Point.y, 0.0f);
		case VIEW_SIDE:		return glm::vec3(0.0f, Point.y, Point.x);
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}

ImVec2 GridEditor::WorldToGridSpace(const glm::vec3& Point, ViewMode CurrentViewMode)
{
	// Just transforms 3D points (Assuming normalised) into 2D grid editor space.

	switch (CurrentViewMode)
	{
		case VIEW_TOP:		return ImVec2(Point.x, Point.z);
		case VIEW_FRONT:	return ImVec2(Point.x, Point.y);
		case VIEW_SIDE:		return ImVec2(Point.z, Point.y);
	}

	return ImVec2(0.0f, 0.0f);
}

void GridEditor::SubmitWorldGeometry(const std::vector<glm::vec3>& Vertices, const std::vector<unsigned int> Indices)
{
	// This would be a system that would submit world geometry into 2D workspace.

	GridFaceSector NewSector;

	NewSector.Indices	= Indices;
	NewSector.Vertices	= Vertices; // @TODO: This appears to be within a smaller space? Perhaps multiply?

	this->GridSectors.push_back(NewSector);
}

void GridEditor::GenerateGeometry(const ImVector<ImVec2>& Points, ViewMode CurrentViewMode)
{
	// Creating a "sector" would be similar to how DOOM does its editor sector. Draw a sector and select a height for a sector
	// The difference for this is that we can edit the vertex of the sector, creating unique shapes

	// @TODO: Maybe add more vertex edit tools such as subdivide, remove ect.
	
	if (Points.size() < 3) { return; }

	GridFaceSector CurrentSector;

	// Draw the top and bottom of the "sector"
	for (auto& CurrentPoints : Points)
	{
		CurrentSector.Vertices.push_back(this->GridToWorldSpace(CurrentPoints, CurrentViewMode) + glm::vec3(0.0f, 0.0f, 0.0f));
	}
	for (auto& CurrentPoints : Points)
	{
		CurrentSector.Vertices.push_back(this->GridToWorldSpace(CurrentPoints, CurrentViewMode) + glm::vec3(0.0f, this->FloorHeight, 0.0f));
	}

	int VertexCount = static_cast<int>(Points.size());
	int OffsetCount = VertexCount;

	for (int Index = 1; Index < VertexCount - 1; Index++)
	{
		CurrentSector.Indices.push_back(0);
		CurrentSector.Indices.push_back(Index);
		CurrentSector.Indices.push_back(Index + 1);
	}

	for (int Index = 1; Index < VertexCount - 1; Index++)
	{
		CurrentSector.Indices.push_back(OffsetCount);
		CurrentSector.Indices.push_back(OffsetCount + Index);
		CurrentSector.Indices.push_back(OffsetCount + Index + 1);
	}

	// Create the walls for the "sector"
	for (int Index = 0; Index < VertexCount; Index++)
	{
		int Next = (Index + 1) % VertexCount;

		unsigned int BottomPoint0	= Index;
		unsigned int BottomPoint1	= Next;
		unsigned int TopPoint0		= Index + VertexCount;
		unsigned int TopPoint1		= Next + VertexCount;

		CurrentSector.Indices.push_back(BottomPoint0);
		CurrentSector.Indices.push_back(TopPoint0);
		CurrentSector.Indices.push_back(TopPoint1);

		CurrentSector.Indices.push_back(BottomPoint0);
		CurrentSector.Indices.push_back(TopPoint1);
		CurrentSector.Indices.push_back(BottomPoint1);
	}

	// @TODO: Add this to the enviroment worldspace!

	this->GridSectors.push_back(CurrentSector);
} 

void GridEditor::DrawGrids(GLFWwindow* EngineWindow)
{
	// @TODO - Could add a system to scroll forward and backwards to adjust the scale of the points.

	ImGuiUtilites::BeginGradient("GridView",{ IM_COL32(40, 40, 40, 255), IM_COL32(20, 20, 20, 255) });

	static ImVector<glm::vec3> Point3D;
	static ImVec2 Scrolling;
	static ViewMode CurrentViewMode = VIEW_TOP;

	// A mini function to snap the current mouse position into the grid space.
	auto MouseGridSnap = [&](ImVec2 Point)
	{
		return ImVec2(floorf(Point.x / GRID_STEP + 0.5f) * GRID_STEP, floorf(Point.y / GRID_STEP + 0.5f) * GRID_STEP);
	};

	if (ImGui::Button("Top (Y)"))	{ CurrentViewMode = VIEW_TOP; }
	ImGui::SameLine();
	if (ImGui::Button("Front (X)")) { CurrentViewMode = VIEW_FRONT; }
	ImGui::SameLine();
	if (ImGui::Button("Side (Z)"))	{ CurrentViewMode = VIEW_SIDE; }
	ImGui::SameLine();
	ImGui::SliderFloat("Height", &this->FloorHeight, 0.01f, 1000.0f); // This would seem to be a good max point.

	ImVec2 CanvasPosition		= ImGui::GetCursorScreenPos();
	ImVec2 CanvasSize			= ImGui::GetContentRegionAvail();

	CanvasSize.x				= std::max(CanvasSize.x, 50.0f);
	CanvasSize.y				= std::max(CanvasSize.y, 50.0f);
	ImVec2 CanvasEnd			= CanvasPosition + CanvasSize; 

	ImGuiIO& CurrentIO			= ImGui::GetIO();
	ImDrawList* CurrentDrawList = ImGui::GetWindowDrawList();

	CurrentDrawList->AddRectFilled(CanvasPosition, CanvasEnd, IM_COL32(0.0f, 0.0f, 0.0f, 255.0f));
	CurrentDrawList->AddRect(CanvasPosition, CanvasEnd, IM_COL32(255.0f, 255.0f, 255.0f, 255.0f));

	// Add a space where we can interact with the canvas. Click, drag ect.
	ImGui::InvisibleButton("CanvasSpace", CanvasSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

	const bool WindowHovered	= ImGui::IsItemHovered();
	const bool WindowActive		= ImGui::IsItemActive();
	static bool VertexActive	= false;

	ImVec2 Origin				= CanvasPosition + Scrolling;
	ImVec2 MouseRawPosition		= CurrentIO.MousePos - Origin;
	ImVec2 Mouse2D				= MouseGridSnap(MouseRawPosition);
	glm::vec3 Mouse3D			= this->GridToWorldSpace(Mouse2D, CurrentViewMode);

	if (glfwGetKey(EngineWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS && glfwGetTime() - this->LastPressedKey > 1.0f) // We would use a debounce key here. Check if the input is less than 1 seconds or so.
	{
		// @TODO - This seems safe for now but in the future we shouldnt assume that the user didnt press this after creating the new object!
		Point3D.clear();
		this->LastPressedKey = glfwGetTime(); // Update the press time.
	}
	
	if (WindowHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		// @TODO - Perhaps make this more efficent, maybe dont use a loop every time we click/limit the usage.
		for (const GridFaceSector& CurrentSector : this->GridSectors)
		{
			for (size_t Index = 0; Index < CurrentSector.Indices.size(); Index++)
			{
				if (Mouse3D == CurrentSector.Vertices[CurrentSector.Indices[Index]]) // Just check if this is a selectable vertex we can access.
				{
					// We can send the address of the boolean to change within a parameter
					// to hence edit the condition that would be given.
					VertexActive = true;
					std::cout << "Pressed within the space" << "\n";
				}
			}
		}



		if (!VertexActive)
		{
			if (Point3D.empty()) { Point3D.push_back(Mouse3D); }
			else
			{
				glm::vec3 First = Point3D[0];
				float DX = Mouse3D.x - First.x;
				float DY = Mouse3D.y - First.y;
				float DZ = Mouse3D.z - First.z;
				float Distance = sqrtf(DX * DX + DY * DY + DZ * DZ);

				// @TODO - Create a cleaner triangle with the vertex being provided (too messy!)

				if (Distance < CLOSE_DISTANCE)
				{
					ImVector<ImVec2> Point2D;

					for (auto& Points : Point3D)
					{
						Point2D.push_back(this->WorldToGridSpace(Points, CurrentViewMode));
					}
					this->GenerateGeometry(Point2D, CurrentViewMode);
					Point3D.clear();
				}
				else
				{
					Point3D.push_back(Mouse3D);
				}
			}
		}
	}

	if (WindowActive && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		Console::LockMouse = true;
		Scrolling.x += CurrentIO.MouseDelta.x;
		Scrolling.y += CurrentIO.MouseDelta.y;
	}

	// Draw the grid lines for the grid.
	for (float X = fmodf(Scrolling.x, GRID_STEP); X < CanvasSize.x; X += GRID_STEP)
	{
		CurrentDrawList->AddLine(ImVec2(CanvasPosition.x + X, CanvasPosition.y), ImVec2(CanvasPosition.x + X, CanvasEnd.y), IM_COL32(100, 500, 100, 60));
	}
	for (float Y = fmodf(Scrolling.y, GRID_STEP); Y < CanvasSize.y; Y += GRID_STEP)
	{
		CurrentDrawList->AddLine(ImVec2(CanvasPosition.x, CanvasPosition.y + Y), ImVec2(CanvasEnd.x, CanvasPosition.y + Y), IM_COL32(100, 500, 100, 60));
	}

	CurrentDrawList->PushClipRect(CanvasPosition, CanvasEnd, true);

	for (int Index = 0; Index < Point3D.Size - 1; Index++)
	{
		ImVec2 Point0, Point1;

		Point0 = this->WorldToGridSpace(Point3D[Index], CurrentViewMode);
		Point1 = this->WorldToGridSpace(Point3D[Index + 1], CurrentViewMode);

		// Draw the actual polygon that would be used to render
		CurrentDrawList->AddLine(Origin + Point0, Origin + Point1, IM_COL32(255, 159, 56, 255), 2.0f);
	}

	if (!Point3D.empty())
	{
		ImVec2 Point0;

		// Draw the actual line.
		Point0 = this->WorldToGridSpace(Point3D.back(),CurrentViewMode);
		CurrentDrawList->AddLine(Origin + Point0, Origin + Mouse2D, IM_COL32(255, 159, 56, 255), 2.0f);
	}

	for (auto& CurrentPoint : Point3D)
	{
		ImVec2 NewPoint;

		NewPoint = this->WorldToGridSpace(CurrentPoint, CurrentViewMode);
		CurrentDrawList->AddCircleFilled(Origin + NewPoint, 5.0f, IM_COL32(252, 132, 3, 255));
	}

	if (!Point3D.empty())
	{
		// Point highlight if near.
		glm::vec3 FirstPoint = Point3D[0];

		float DX = Mouse3D.x - FirstPoint.x;
		float DY = Mouse3D.y - FirstPoint.y;
		float DZ = Mouse3D.z - FirstPoint.z;

		float Distance = sqrtf(DX * DX + DY * DY + DZ * DZ);

		if (Distance < CLOSE_DISTANCE)
		{
			ImVec2 NewPoint;
			NewPoint = this->WorldToGridSpace(FirstPoint, CurrentViewMode);
			CurrentDrawList->AddCircleFilled(Origin + NewPoint, 8.0f, IM_COL32(255, 159, 56, 255));
		}
	}

	// Draw all the geometry
	// @TODO: Maybe make this function seperate since we can use this for all items within the world space.
	for (const GridFaceSector& CurrentSector : this->GridSectors)
	{
		for (size_t Index = 0; Index < CurrentSector.Indices.size(); Index+=3)
		{
			ImVec2 Point0 = this->WorldToGridSpace(CurrentSector.Vertices[CurrentSector.Indices[Index]], CurrentViewMode);
			ImVec2 Point1 = this->WorldToGridSpace(CurrentSector.Vertices[CurrentSector.Indices[Index + 1]], CurrentViewMode);
			ImVec2 Point2 = this->WorldToGridSpace(CurrentSector.Vertices[CurrentSector.Indices[Index + 2]], CurrentViewMode);
			
			CurrentDrawList->AddTriangleFilled(Origin + Point0, Origin + Point1, Origin + Point2, IM_COL32(60, 60, 60, 255));
		}

		for (size_t IndexA = 0; IndexA < CurrentSector.Vertices.size(); IndexA++)
		{
			for (size_t IndexB = IndexA + 1; IndexB < CurrentSector.Vertices.size(); IndexB++)
			{
				ImVec2 Point0 = this->WorldToGridSpace(CurrentSector.Vertices[IndexA], CurrentViewMode);
				ImVec2 Point1 = this->WorldToGridSpace(CurrentSector.Vertices[IndexB], CurrentViewMode);

				CurrentDrawList->AddLine(Origin + Point0, Origin + Point1, IM_COL32(255, 255, 255, 255), 2.0f);
			}
		}

		for (size_t Index = 0; Index < CurrentSector.Indices.size(); Index++)
		{
			// We would draw the vertex from each of the vertices from the geometry we would have created. This should change
			// for each axis we switch from

			// @TODO - Make the vertex editable and dragable when being clicked on.

			ImVec2 CurrentPoint = this->WorldToGridSpace(CurrentSector.Vertices[CurrentSector.Indices[Index]], CurrentViewMode);
			CurrentDrawList->AddCircleFilled(Origin + CurrentPoint, 5, IM_COL32(60, 255, 60, 255)); // We would need to add towards the origin (camera point)
		}

	}

	CurrentDrawList->PopClipRect();
	ImGui::End();
}