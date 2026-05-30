#ifndef __EDITOR_WORKSPACE_HEADER__
#define __EDITOR_WORKSPACE_HEADER__

#include "Bounds/BoundCreation.h"
#include "Bounds/BoundRenderer.h"

#include "../Utilites/EditorUtilites.h"

enum GeometryShapeType
{
	SHAPE_NULL = 0,
	SHAPE_BLOCK,
	SHAPE_MESH
};

struct TextureFileFormat
{
	std::string					DiffuseTexture;
	std::string					SpecularTexture;
	std::string					NormalTexture;

	unsigned int				DiffuseID = -1;
	unsigned int				SpecularID = -1;
	unsigned int				NormalID = -1;
};

struct ExternalData
{
	bool						Visible;
	bool						PhysicsLocked;
};

struct GeometryData
{
	std::vector<VertexPoint>	WorldPositions;
	std::vector<float>			Vertices;
	std::vector<unsigned int>	Indices;
	BoundingBox					GeometryBoundingBox;
	TextureFileFormat			GeometryTextureFormat;
	GeometryShapeType			GeometryShapeFormat;
	ExternalData				GeometryExternalFormat;

	glm::vec3					Translation;
	glm::quat					Rotation;
	glm::vec3					Scale;

	glm::mat4					TranslationMatrix;
	glm::mat4					RotationMatrix;
	glm::mat4					ScaleMatrix;

	unsigned int				VertexArray;
	unsigned int				VertexBuffer;
	unsigned int				ElementBuffer;
};

class EditorWorkspace
{
	public:
									EditorWorkspace(void);
		void						SetGeometryData(Shader* BoundShader, GeometryData NewGeometryData);
		void						CreateGeometryData(Shader* BoundShader, const std::vector<float>& Vertices, const std::vector<unsigned int>& Indices, const glm::vec3& Translation, const glm::quat& Rotation, const glm::vec3& Scale, const std::string& TextureName, GeometryShapeType NewType, unsigned int TextureID);

		void						SetPosition(int Index, const glm::vec3& Translation);
		void						SetRotation(int Index, const glm::quat& Rotation);
		void						SetScale(int Index, const glm::vec3& Scale);
		void						SetVisible(int Index, bool Visible);
		void						SetPhysicsLocked(int Index, bool PhysicsLocked);

		void						CopyObject(int Index);
		void						PasteObject(int Index);
		void						SetSelectedIndex(int Index);

		void						DrawGeometryScene(Shader* DrawableShader, const glm::mat4& Projection, const glm::mat4& View, const glm::vec3& CameraPosition);
		void						DestroyBlocks(int Index);

		std::vector<GeometryData>&	GetGeometryList(void);
		glm::vec3&					GetPosition(int Index);
		glm::quat&					GetRotation(int Index);
		glm::vec3&					GetScale(int Index);

		glm::mat4&					GetPositionMatrix(int Index);
		glm::mat4&					GetRotationMatrix(int Index);
		glm::mat4&					GetScaleMatrix(int Index);
		BoundsRenderer&				GetBoundsSystem(void);
	private:
		bool						DoesIndexExist(int Index);
		int							GivenSelectedIndex;

		BoundsSystem				BoundCreationSystem;
		BoundsRenderer				BoundDrawingSystem;
		std::vector<GeometryData>	DrawableGeometry;
};


#endif