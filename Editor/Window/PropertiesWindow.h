#ifndef __PROPERTIES_HEADER__
#define __PROPERTIES_HEADER__

#include <string>
#include <vector>
#include <map>

#include "../../../Resources/imgui/imgui.h"
#include "../../../Resources/imgui/imgui_impl_opengl3.h"
#include "../../../Resources/imgui/imgui_impl_glfw.h"
#include "../../../Resources/imgui/imgui_stdlib.h"

#include "../../../Resources/glm/glm.hpp"
#include "../../../Resources/glm/gtc/quaternion.hpp"

#define ACTIVE_SELECTION_COLOUR ImVec4(0.10f, 0.65f, 0.35f, 1.00f)

enum InterfaceDataType
{
	DATATYPE_NULL = 0,
	DATATYPE_ENVIROMENT_NOINTERACT,
	DATATYPE_BLOCK,
	DATATYPE_SCRIPT,
	DATATYPE_LIGHT,
	DATATYPE_PLAYER,
	DATATYPE_SKY,
	DATATYPE_SUN,
	DATATYPE_CAMERA
};

struct SkyboxObjectProperties
{
	bool									ChangeSkyboxValue;
	int										ChangeSkyboxIndex;

	std::vector<std::string>				SkyboxValues;
	std::vector<std::string>				SkyboxDimensions = { "Front","Back","Left","Right","Top","Bottom" };
	std::map<std::string, unsigned int>		SkyboxTextureSystem;
};

struct ModelObjectProperties
{
	int					Index = 0;
	int					SelectedMaterial;
	bool				Selected = false;
	bool				Visible = true;
	bool				UseColour = true;
	bool				PhysicsLocked = true;

	std::string			LabelName;
	std::string			LabelPrefix;
	InterfaceDataType	ObjectDataType;

	glm::vec3			Translate;
	glm::quat			Rotation;
	glm::vec3			Scale;

	glm::vec4			Colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
};

class BaseProperties
{
	public:
									BaseProperties(void);
		void						DrawGeometryProperties(ModelObjectProperties NewProperties);
		void						DrawSkyboxProperties(SkyboxObjectProperties NewProperties);
		void						DrawCameraProperties(void);
		void						DrawSunProperties(void);
	private:
		std::string					LastKnownModelName;
		std::vector<std::string>	MaterialItems = { "None","Fluid","Metal" };
};


#endif