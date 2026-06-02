#ifndef __EDITOR_HEADER__
#define __EDITOR_HEADER__

#include "Session/SessionManager.h"
#include "Utilites/EditorUtilites.h"

#include "../Resources/glm/gtx/matrix_decompose.hpp"

//#define ALPHA_SELECTION_MODE

class EditorMain
{
    public:
                                    EditorMain(void);
        void                        StartEngine(void);
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
};

#endif