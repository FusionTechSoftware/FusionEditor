#include "EditorWorkspace.h"

EditorWorkspace::EditorWorkspace(void) 
{
	this->GivenSelectedIndex	= -1;
	this->GeometryClipboard		= nullptr;
}

BoundsRenderer& EditorWorkspace::GetBoundsSystem(void) { return this->BoundDrawingSystem; }

glm::vec3& EditorWorkspace::GetPosition(int Index)
{
	if (!this->DoesIndexExist(Index)) { return glm::vec3(0.0f,0.0f,0.0f); }
	return this->DrawableGeometry[Index].Translation;
}

glm::quat& EditorWorkspace::GetRotation(int Index)
{
	if (!this->DoesIndexExist(Index)) { return glm::quat(0.0f,0.0f,0.0f,1.0f); }
	return this->DrawableGeometry[Index].Rotation;
}

glm::vec3& EditorWorkspace::GetScale(int Index)
{
	if (!this->DoesIndexExist(Index)) { return glm::vec3(0.0f,0.0f,0.0f); }
	return this->DrawableGeometry[Index].Scale;
}

glm::mat4& EditorWorkspace::GetPositionMatrix(int Index)
{
	if (!this->DoesIndexExist(Index)) { return glm::mat4(1.0f); }
	return this->DrawableGeometry[Index].TranslationMatrix;
}

glm::mat4& EditorWorkspace::GetRotationMatrix(int Index)
{
	if (!this->DoesIndexExist(Index)) { return glm::mat4(1.0f); }
	return this->DrawableGeometry[Index].RotationMatrix;
}

glm::mat4& EditorWorkspace::GetScaleMatrix(int Index)
{
	if (!this->DoesIndexExist(Index)) { return glm::mat4(1.0f); }
	return this->DrawableGeometry[Index].ScaleMatrix;
}

std::vector<GeometryData>& EditorWorkspace::GetGeometryList(void)
{ return this->DrawableGeometry; }

bool EditorWorkspace::DoesIndexExist(int Index)
{
	if (Index > this->DrawableGeometry.size()) { Console::SysPrint("Invalid index given.", 2); return false; }
	return true;
}

void EditorWorkspace::UpdateVertices(int Index,const std::vector<float>& NewVertices,bool ModifyVertices)
{
	if (ModifyVertices) { this->DrawableGeometry[Index].Vertices = NewVertices; }

	glBindBuffer(GL_ARRAY_BUFFER, this->DrawableGeometry[Index].VertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->DrawableGeometry[Index].Vertices.size() * sizeof(float), this->DrawableGeometry[Index].Vertices.data());
}

void EditorWorkspace::UpdateWorldGeometryData(int Index,const glm::mat4& Matrix)
{
	if (this->DrawableGeometry[Index].IsGeometryInWorldPosition)
	{
		this->DrawableGeometry[Index].Vertices = EditorUtilites::UpdateWorldSpaceGeometryFromMatrix(
			this->DrawableGeometry[Index].OriginalVertices,
			Matrix
		);
	}

	this->UpdateVertices(Index, this->DrawableGeometry[Index].Vertices, false);
}

void EditorWorkspace::SetPosition(int Index, const glm::vec3& Translation)
{
	if (!this->DoesIndexExist(Index)) { return; }
	this->DrawableGeometry[Index].Translation			= Translation;
	this->DrawableGeometry[Index].TranslationMatrix		= glm::translate(glm::mat4(1.0f), Translation);

	this->UpdateWorldGeometryData(Index, 
		this->DrawableGeometry[Index].TranslationMatrix * 
		this->DrawableGeometry[Index].RotationMatrix * 
		this->DrawableGeometry[Index].ScaleMatrix
	);
}

void EditorWorkspace::SetRotation(int Index, const glm::quat& Rotation)
{
	if (!this->DoesIndexExist(Index)) { return; }
	this->DrawableGeometry[Index].Rotation			= Rotation;
	this->DrawableGeometry[Index].RotationMatrix	= glm::mat4_cast(Rotation);

	this->UpdateWorldGeometryData(Index,
		this->DrawableGeometry[Index].TranslationMatrix *
		this->DrawableGeometry[Index].RotationMatrix *
		this->DrawableGeometry[Index].ScaleMatrix
	);
}

void EditorWorkspace::SetScale(int Index, const glm::vec3& Scale)
{
	if (!this->DoesIndexExist(Index)) { return; }
	this->DrawableGeometry[Index].Scale			= Scale;
	this->DrawableGeometry[Index].ScaleMatrix	= glm::scale(glm::mat4(1.0f), Scale);

	this->UpdateWorldGeometryData(Index,
		this->DrawableGeometry[Index].TranslationMatrix *
		this->DrawableGeometry[Index].RotationMatrix *
		this->DrawableGeometry[Index].ScaleMatrix
	);
}

void EditorWorkspace::SetVisible(int Index, bool Visible)
{
	if (!this->DoesIndexExist(Index)) { return; }
	this->DrawableGeometry[Index].GeometryExternalFormat.Visible = Visible;
}

void EditorWorkspace::SetPhysicsLocked(int Index, bool PhysicsLocked)
{
	if (!this->DoesIndexExist(Index)) { return; }
	this->DrawableGeometry[Index].GeometryExternalFormat.PhysicsLocked = PhysicsLocked;
}

void EditorWorkspace::SetSelectedIndex(int Index)
{
	this->GivenSelectedIndex = Index;
}

void EditorWorkspace::SetGeometryData(Shader* BoundShader, GeometryData NewGeometryData)
{
	NewGeometryData.WorldPositions = EditorUtilites::GetWorldPointsFromMatrix(NewGeometryData.Vertices, NewGeometryData.TranslationMatrix * NewGeometryData.RotationMatrix * NewGeometryData.ScaleMatrix);

	glGenVertexArrays(1, &NewGeometryData.VertexArray);
	glGenBuffers(1, &NewGeometryData.VertexBuffer);
	glGenBuffers(1, &NewGeometryData.ElementBuffer);

	glBindVertexArray(NewGeometryData.VertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, NewGeometryData.VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, NewGeometryData.WorldPositions.size() * sizeof(VertexPoint), NewGeometryData.WorldPositions.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewGeometryData.ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NewGeometryData.Indices.size() * sizeof(unsigned int), NewGeometryData.Indices.data(), GL_STATIC_DRAW); // Indices should not be modified as much.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	this->BoundDrawingSystem.CreateBoundingBox(
		BoundShader,
		NewGeometryData.GeometryBoundingBox.MinimumBounds,
		NewGeometryData.GeometryBoundingBox.MaximumBounds,
		NewGeometryData.Translation,
		NewGeometryData.Rotation,
		NewGeometryData.Scale
	);

	this->DrawableGeometry.push_back(NewGeometryData);
}

void EditorWorkspace::CreateGeometryData(Shader* BoundShader, const std::vector<float>& Vertices, const std::vector<unsigned int>& Indices, const glm::vec3& Translation, const glm::quat& Rotation, const glm::vec3& Scale, const std::string& TextureName, GeometryShapeType NewType, unsigned int TextureID)
{
	GeometryData NewGeometryData;

	NewGeometryData.OriginalVertices	= Vertices;
	NewGeometryData.Vertices			= Vertices;
	NewGeometryData.Indices				= Indices;

	NewGeometryData.Translation			= Translation;
	NewGeometryData.Rotation			= Rotation;
	NewGeometryData.Scale				= Scale;
		
	NewGeometryData.TranslationMatrix	= glm::translate(glm::mat4(1.0f), Translation);
	NewGeometryData.RotationMatrix		= glm::mat4_cast(Rotation);
	NewGeometryData.ScaleMatrix			= glm::scale(glm::mat4(1.0f), Scale);

	NewGeometryData.GeometryTextureFormat.DiffuseID			= TextureID;
	NewGeometryData.GeometryTextureFormat.DiffuseTexture	= TextureName;
	NewGeometryData.GeometryBoundingBox						= this->BoundCreationSystem.GenerateNewBounds(EditorUtilites::GroupVector3(Vertices), Translation);
	NewGeometryData.IsGeometryInWorldPosition				= true;

	this->SetGeometryData(BoundShader, NewGeometryData);
}

void EditorWorkspace::DrawGeometryScene(Shader* DrawableShader, const glm::mat4& Projection, const glm::mat4& View,const glm::vec3& CameraPosition)
{
	for (int Index = 0; Index < this->DrawableGeometry.size(); Index++)
	{
		this->DrawableGeometry[Index].GeometryBoundingBox = this->BoundCreationSystem.UpdateBounds(this->DrawableGeometry[Index].Translation,this->DrawableGeometry[Index].GeometryBoundingBox);

		glBindVertexArray(this->DrawableGeometry[Index].VertexArray);
	
		if (this->DrawableGeometry[Index].GeometryTextureFormat.DiffuseID != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->DrawableGeometry[Index].GeometryTextureFormat.DiffuseID);
			DrawableShader->SetInt(0, "Diffuse");
		}

		if (this->DrawableGeometry[Index].GeometryShapeFormat == SHAPE_MESH)	{glFrontFace(GL_CW);}
		else																	{glFrontFace(GL_CCW);}
		
		DrawableShader->ActivateShader();
		DrawableShader->SetMatrix4(Projection, "Projection");
		DrawableShader->SetMatrix4(View, "View");
		DrawableShader->SetVector3(CameraPosition, "CameraPosition");

		DrawableShader->SetMatrix4(this->DrawableGeometry[Index].TranslationMatrix, "Translation");
		DrawableShader->SetMatrix4(this->DrawableGeometry[Index].RotationMatrix, "Rotation");
		DrawableShader->SetMatrix4(this->DrawableGeometry[Index].ScaleMatrix, "Scale");

		DrawableShader->SetVector1(1.0f, "IsColour");
		DrawableShader->SetVector1((this->DrawableGeometry[Index].IsGeometryInWorldPosition ? 1.0f : 0.0f), "WorldSpaceProjection");
		DrawableShader->SetVector4(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), "Colour");

		glDrawElements(GL_TRIANGLES, this->DrawableGeometry[Index].Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	if (this->GivenSelectedIndex != -1) { this->BoundDrawingSystem.DrawBoundingBoxIndexed(Projection, View, this->GivenSelectedIndex); }
}

void EditorWorkspace::CopyObject(int Index)
{
	//Just copy the element into an object clipboard.
	this->GeometryClipboard = &this->DrawableGeometry[Index];
}

void EditorWorkspace::PasteObject(int Index)
{ 
	if (this->GeometryClipboard != nullptr)
	{
		this->DrawableGeometry.push_back(*this->GeometryClipboard);
		this->GeometryClipboard = nullptr; // Clear the pointer after its done
	}
}

void EditorWorkspace::DestroyBlocks(int Index)
{
	if (!this->DoesIndexExist(Index)) { return; }

	glDeleteVertexArrays(1, &this->DrawableGeometry[Index].VertexArray);
	glDeleteBuffers(1, &this->DrawableGeometry[Index].VertexBuffer);
	glDeleteBuffers(1, &this->DrawableGeometry[Index].ElementBuffer);

	this->BoundDrawingSystem.DestroyBoundRender(Index);
	this->DrawableGeometry.erase(this->DrawableGeometry.begin() + Index);
}