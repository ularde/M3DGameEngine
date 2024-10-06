#include "pch.h"
#include "Test.h"
#include "Model.h"
#include "Camera.h"
#include "Scene.h"

class MScene;
class MModel;
MModel* myModel = 0;
MScene* tScene = 0;

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(0.0f,  0.0f, 0.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

void InitTest(MBasicPlatform* platform) {
    tScene = new MScene(true, platform, "Scenes\\Example.xml");

    glBindProgramPipeline(platform->pipelineID);

    platform->camera = new MFreeCamera(platform, platform->vertShader, platform->fragShader, 2.0f, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //myModel = new MModel(true, platform->assetManager, GenerateGuidA(), "model_static", "Objects\\mmodel\\char1.fbx");

    glBindProgramPipeline(0);
}

void RunTest(MBasicPlatform* platform) {
    static float z = 0.0f;
    z += 0.3f;
    // pass projection matrix to shader (note that in this case it could change every frame)
    glEnable(GL_DEPTH_TEST);
    glBindProgramPipeline(platform->pipelineID);
    platform->vertShader->UseProgramStage();
    platform->fragShader->UseProgramStage();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)platform->GetWindowWidth() / (float)platform->GetWindowHeight(), 0.1f, 100.0f);
    platform->vertShader->UniformMat4("projection", projection);

    glm::mat4 view = platform->camera->GetViewMatrix();
    platform->vertShader->UniformMat4("view", view);
    platform->camera->ProcessMovement();


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.13f, 0.1f, 0.1f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    platform->vertShader->UniformMat4("model", model);

    platform->vertShader->UniformVec3("viewPos", platform->camera->position);
    platform->fragShader->UniformFloat("shininess", 32.0f);
    platform->vertShader->UniformVec3("lightPos1", glm::vec3(0.0f, 8.0f, 5.0f));
    platform->fragShader->UniformFloat("lightConstant1", 1.0f);
    platform->fragShader->UniformFloat("lightLinear1", 0.09f);
    platform->fragShader->UniformFloat("lightQuadratic1", 0.032f);
    platform->fragShader->UniformVec3("lightAmbient1", glm::vec3(1.0f, 1.0f, 1.0f));
    platform->fragShader->UniformVec3("lightDiffuse1", glm::vec3(1.0f, 1.0f, 1.0f));
    platform->fragShader->UniformVec3("lightSpecular1", glm::vec3(1.0f, 1.0f, 1.0f));
    platform->vertShader->UniformVec3("lightPos2", glm::vec3(3.0f * cos(glfwGetTime()), 20.0f, 3.0f * sin(glfwGetTime())));
    platform->fragShader->UniformFloat("lightConstant2", 1.0f);
    platform->fragShader->UniformFloat("lightLinear2", 0.09f);
    platform->fragShader->UniformFloat("lightQuadratic2", 0.032f);
    platform->fragShader->UniformVec3("lightAmbient2", glm::vec3(1.0f, 1.0f, 1.0f));
    platform->fragShader->UniformVec3("lightDiffuse2", glm::vec3(1.0f, 1.0f, 1.0f));
    platform->fragShader->UniformVec3("lightSpecular2", glm::vec3(1.0f, 1.0f, 1.0f));

    //myModel->Render();
    tScene->Tick(platform->GetDeltaTime());

    glBindProgramPipeline(0);
}
