#ifndef __ENTITY_COMPONENT_SERVICE__
#define __ENTITY_COMPONENT_SERVICE__

#include <vector>

#include "../Rendering/Models/Model.h"


struct EntityComponents
{
    std::string     MeshSource;
    Model           ModelGroup;
};

class ModelSystem
{
    public:
                                        ModelSystem(void);
        void                            AddEntity(Parser* EngineParser,const std::string& MeshSource,const glm::vec3& Translation,const glm::quat& Rotation,const glm::vec3& Scale);
        void                            RemoveEntity(const std::string& MeshSource);

        void                            RenderGroup(Shader* EngineShader,Camera* EngineCamera,float NearView,float FarView,float FieldOfView,float DeltaTime);
        void                            DestroyGroup(void);

        Model*                          GetModelInfomation(const std::string MeshSource);
    private:
        std::vector<EntityComponents>   ModelGroups;
};

#endif