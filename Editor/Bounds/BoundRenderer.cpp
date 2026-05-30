#include "BoundRenderer.h"

BoundsRenderer::BoundsRenderer(void) {}

void BoundsRenderer::CreateBoundingBox(Shader* NewBoundShader, const glm::vec3& MinimumBounds, const glm::vec3& MaximumBounds, const glm::vec3& Position, const glm::quat& Rotation, const glm::vec3& Scale)
{
	BoundObject NewObject;

    std::vector<float> Vertices = {
        MinimumBounds.x,MinimumBounds.y,MinimumBounds.z,
        MaximumBounds.x,MinimumBounds.y,MinimumBounds.z,
        MaximumBounds.x,MaximumBounds.y,MinimumBounds.z,
        MinimumBounds.x,MaximumBounds.y,MinimumBounds.z,
        MinimumBounds.x,MinimumBounds.y,MaximumBounds.z,
        MaximumBounds.x,MinimumBounds.y,MaximumBounds.z,
        MaximumBounds.x,MaximumBounds.y,MaximumBounds.z,
        MinimumBounds.x,MaximumBounds.y,MaximumBounds.z
    };

    std::vector<unsigned int> Indices = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

	NewObject.ObjectShader          = NewBoundShader;
    NewObject.Vertices              = Vertices;
    NewObject.Indices               = Indices;
    NewObject.TranslationMatrix     = glm::mat4(1.0f);
    NewObject.RotationMatrix        = glm::mat4(1.0f);
    NewObject.ScaleMatrix           = glm::mat4(1.0f);

    NewObject.TranslationMatrix     = glm::translate(NewObject.TranslationMatrix, Position);
    NewObject.RotationMatrix        = glm::mat4_cast(Rotation);
    NewObject.ScaleMatrix           = glm::scale(NewObject.ScaleMatrix, Scale);

    glGenVertexArrays(1, &NewObject.VertexArray);
    glGenBuffers(1, &NewObject.VertexBuffer);
    glGenBuffers(1, &NewObject.ElementBuffer);

    glBindVertexArray(NewObject.VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, NewObject.VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, NewObject.Vertices.size() * sizeof(float), NewObject.Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewObject.ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NewObject.Indices.size() * sizeof(unsigned int), NewObject.Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
    this->DrawableBounds.push_back(NewObject);
}

void BoundsRenderer::DrawBoundingBox(const glm::mat4& Projection, const glm::mat4& View)
{
    for (int Index = 0; Index < this->DrawableBounds.size(); Index++)
    {
        glBindVertexArray(this->DrawableBounds[Index].VertexArray);

        this->DrawableBounds[Index].ObjectShader->ActivateShader();
        this->DrawableBounds[Index].ObjectShader->SetMatrix4(Projection, "Projection");
        this->DrawableBounds[Index].ObjectShader->SetMatrix4(View, "View");
        this->DrawableBounds[Index].ObjectShader->SetMatrix4(this->DrawableBounds[Index].TranslationMatrix, "Translation");
        this->DrawableBounds[Index].ObjectShader->SetMatrix4(this->DrawableBounds[Index].RotationMatrix, "Rotation");
        this->DrawableBounds[Index].ObjectShader->SetMatrix4(this->DrawableBounds[Index].ScaleMatrix, "Scale");
        this->DrawableBounds[Index].ObjectShader->SetVector3(glm::vec3(0.0f, 1.0f, 0.0f), "BoundColour");

        glDrawElements(GL_LINES, this->DrawableBounds[Index].Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void BoundsRenderer::DrawBoundingBoxIndexed(const glm::mat4& Projection, const glm::mat4& View, int Index)
{
    if (Index != -1 && Index > this->DrawableBounds.size()) { Console::SysPrint("Invalid index selected to draw bounds", 2); return; }

    glBindVertexArray(this->DrawableBounds[Index].VertexArray);

    this->DrawableBounds[Index].ObjectShader->ActivateShader();
    this->DrawableBounds[Index].ObjectShader->SetMatrix4(Projection, "Projection");
    this->DrawableBounds[Index].ObjectShader->SetMatrix4(View, "View");
    this->DrawableBounds[Index].ObjectShader->SetMatrix4(this->DrawableBounds[Index].TranslationMatrix, "Translation");
    this->DrawableBounds[Index].ObjectShader->SetMatrix4(this->DrawableBounds[Index].RotationMatrix, "Rotation");
    this->DrawableBounds[Index].ObjectShader->SetMatrix4(this->DrawableBounds[Index].ScaleMatrix, "Scale");
    this->DrawableBounds[Index].ObjectShader->SetVector3(glm::vec3(0.0f, 1.0f, 0.0f), "BoundColour");

    glDrawElements(GL_LINES, this->DrawableBounds[Index].Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void BoundsRenderer::DestroyBoundRender(int Index)
{
    if (Index != -1 && Index > this->DrawableBounds.size()) { Console::SysPrint("Invalid index to destroy", 2); return; }

    glDeleteVertexArrays(1, &this->DrawableBounds[Index].VertexArray);
    glDeleteBuffers(1, &this->DrawableBounds[Index].ElementBuffer);
    glDeleteBuffers(1, &this->DrawableBounds[Index].VertexBuffer);

    this->DrawableBounds.erase(this->DrawableBounds.begin() + Index);
}