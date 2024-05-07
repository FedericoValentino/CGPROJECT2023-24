#ifndef PROJECT_H
#define PROJECT_H

#include "ControlWrapper.cpp"
#include "Starter.hpp"
#include "../Piece.hpp"

struct GlobalUniformBufferObject {
    alignas(16) glm::vec3 lightDir;
    alignas(16) glm::vec4 lightColor;
    alignas(16) glm::vec3 eyePos;
};

class Project : public BaseProject
{
private:

    std::vector<Piece*> pieces;

    int numObj = 100;
    float Ar;

    void localInit() final;

    void pipelinesAndDescriptorSetsInit() final;

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int i) final;

    void setWindowParameters() final;

    void updateUniformBuffer(uint32_t currentImage) final;

    void pipelinesAndDescriptorSetsCleanup() final;

    void localCleanup() final;

    void entityGeneration() final;
};

void Project::localInit() {

    Piece* p = new Piece();

    p->init(this);
    p->ubo.model = glm::mat4(1);

    pieces.push_back(p);

}

void Project::pipelinesAndDescriptorSetsInit() {
    for(Piece* p : pieces)
    {
        p->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));
    }
}

void Project::populateCommandBuffer(VkCommandBuffer commandBuffer, int i) {
    for(Piece* p : pieces)
    {
        p->populateCommandBuffer(commandBuffer, i);
    }
}

void Project::setWindowParameters() {
    windowWidth = 1280;
    windowHeight = 720;
    windowTitle = "TIME PILOT 2024";
    windowResizable = GLFW_TRUE;
    initialBackgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};

    uniformBlocksInPool = numObj*2;
    texturesInPool = numObj;
    setsInPool = numObj;

    Ar = 5.0f / 3.0f;
}

void Project::updateUniformBuffer(uint32_t currentImage) {
    GlobalUniformBufferObject gubo;

    //light update
    gubo.lightDir = glm::vec3(cos(glm::radians(30.0f)), sin(glm::radians(30.0f)), 0.0f);
    gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gubo.eyePos = glm::vec3(100.0, 100.0, 100.0);


    float deltaT;
    float time;
    bool SpaceBar = false;
    bool BackSpace = false;
    glm::vec3 m = glm::vec3(0.0f);
    glm::vec3 r = glm::vec3(0.0f);

    getSixAxis(deltaT, time, m, r, SpaceBar, BackSpace);


    glm::mat4 S = updateCam(Ar, deltaT, m, r);

    for(Piece* p : pieces)
    {
        p->ubo.model =  glm::rotate(p->ubo.model, deltaT *glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        p->ubo.worldViewProj = S * p->ubo.model;
        p->ubo.normal = glm::inverse(glm::transpose(p->ubo.model));

        p->DS.map(currentImage, &p->ubo, sizeof(p->ubo), 0);

        p->DS.map(currentImage, &gubo, sizeof(gubo), 2);
    }

}

void Project::pipelinesAndDescriptorSetsCleanup() {
    for(Piece* p : pieces)
    {
        p->pipelineAndDSClenup();
    }
}

void Project::localCleanup() {
    for(Piece* p : pieces)
    {
        p->cleanup();
        p->P.destroy();
        p->DSL.cleanup();
    }
}

void Project::entityGeneration() {

    static bool spacePress = false;
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePress){
        if(!spacePress) {
            spacePress = true;
        }
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE){
        spacePress = false;
    }

    static bool backSpace = false;
    if(glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && !backSpace){
        if(!backSpace) {
            backSpace = true;
        }
    }

    if(glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_RELEASE){
        backSpace = false;
    }


    if(spacePress)
    {
        Piece* p = new Piece();

        p->init(this);
        p->ubo.model = glm::mat4(1);
        p->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));

        pieces.push_back(p);

        printf("Added a cube!\n");
    }

    if(backSpace)
    {

        if(pieces.size() > 0)
        {
            vkDeviceWaitIdle(this->device);
            Piece* p = pieces[pieces.size() - 1];
            pieces.pop_back();
            p->pipelineAndDSClenup();

            p->cleanup();
            p->P.destroy();
            p->DSL.cleanup();
        }
    }


}

#endif