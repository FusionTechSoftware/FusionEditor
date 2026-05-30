#include "ModelSystem.h"

ModelSystem::ModelSystem(void)
{
    this->ModelGroups = {};
};

Model* ModelSystem::GetModelInfomation(const std::string MeshSource)
{
    for (int Index = 0; Index < this->ModelGroups.size(); Index++)
    {
        if (this->ModelGroups[Index].MeshSource == MeshSource) 
        {
            return &this->ModelGroups[Index].ModelGroup;
        }
    }

    Console::SysPrint("Failed to get model reference of mesh source '"+MeshSource+"'.",3);
    return nullptr;
}

void ModelSystem::AddEntity(Parser* EngineParser,const std::string& MeshSource,const glm::vec3& Translation,const glm::quat& Rotation,const glm::vec3& Scale)
{
    EntityComponents NewComponents;
    Model NewModel;
    
    NewModel.LoadFromGLTF(EngineParser,MeshSource,Translation,Rotation,Scale);
    NewComponents.MeshSource    = MeshSource;
    NewComponents.ModelGroup    = NewModel;   

    this->ModelGroups.push_back(NewComponents);
}

void ModelSystem::RemoveEntity(const std::string& MeshSource)
{
    for (int Index = 0; Index < this->ModelGroups.size(); Index++)
    {
        if (this->ModelGroups[Index].MeshSource == MeshSource) 
        {
            this->ModelGroups[Index].ModelGroup.DestroyModel();
            this->ModelGroups.erase(this->ModelGroups.begin()+Index);
        }
    }
}

void ModelSystem::RenderGroup(Shader* EngineShader,Camera* EngineCamera,float NearView,float FarView,float FieldOfView,float DeltaTime)
{
    for (int Index = 0; Index < this->ModelGroups.size(); Index++)
    {
        this->ModelGroups[Index].ModelGroup.RenderModel(EngineShader, EngineCamera, NearView, FarView, FieldOfView);
    }
}

void ModelSystem::DestroyGroup(void)
{
    for (int Index = 0; Index < this->ModelGroups.size(); Index++)
    {
        this->ModelGroups[Index].ModelGroup.DestroyModel();
    }
}