#ifndef __EDITOR_HEADER__
#define __EDITOR_HEADER__

#include "Session/SessionManager.h"
#include "Utilites/EditorUtilites.h"

#include "../Resources/glm/gtx/matrix_decompose.hpp"
#include "../Resources/imgui/ImGuizmo.h"

class EditorMain
{
    public:
                                    EditorMain(void);
        void                        StartEngine(void);
        void                        ShowEditGizmo(int ViewportWidth, int ViewportHeight);
        void                        RenderEngine(void);
        void                        CloseEngine(void);
        void                        SpecialKeyFunction(void);
    private:
        void                        SelectObjectFromWorld(ImVec2 ViewportCursorPosition);
        int                         ViewportWidth;
        int                         ViewportHeight;
        double                      LastNormalTrigger;

        std::vector<GeometryData>   CurrentGeometryData;
        glm::vec3                   RayOrigin;
        glm::vec3                   RayDirection;
        glm::vec3                   DragDelta;
        ImVec2                      EngineViewportSize;
        SessionManager              NewSessionManager;

        glm::vec3                   ObjectScale;
        glm::vec3                   ObjectTranslation;
        glm::quat                   ObjectRotation;
        glm::vec3                   Skew;
        glm::vec4                   Perspective;
};

#endif