#ifndef PROJECT_H
#define PROJECT_H

#include "ControlWrapper.cpp"
#include "Starter.hpp"
#include "../View/PlaneView.hpp"
#include "../View/TileView.hpp"
#include "../Model/Include/Partita.h"

struct GlobalUniformBufferObject {
    alignas(16) glm::vec3 lightDir;
    alignas(16) glm::vec4 lightColor;
    alignas(16) glm::vec3 eyePos;
};

class Project : public BaseProject
{
private:

    Partita* partita;
    std::vector<PlaneView*> Planes;
    TileView* tiles;

    int numObj = 100;
    float Ar;

    void localInit() final;

    void pipelinesAndDescriptorSetsInit() final;

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int i) final;

    void setWindowParameters() final;

    void onWindowResize(int w, int h) final;

    void updateUniformBuffer(uint32_t currentImage) final;

    void pipelinesAndDescriptorSetsCleanup() final;

    void localCleanup() final;

    void entityGeneration() final;
};

void Project::localInit() {

    this->partita = new Partita();
    partita->generateWorld();

    this->tiles = new TileView;
    tiles->init(this);

    for(int row = 0; row < MAPDIM; row++)
    {
        for(int col = 0; col < MAPDIM; col++)
        {
            tiles->newTile(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject), row, col, partita->map[row][col]->height);
        }
    }


    PlaneView* p = new PlaneView();
    p->init(this);
    p->ubo.model = glm::mat4(1);
    p->ubo.model *= glm::translate(glm::mat4(1), glm::vec3(0.0, 8.40, 0.0));

    Planes.push_back(p);

}

void Project::pipelinesAndDescriptorSetsInit() {
    tiles->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));

    for(PlaneView* p : Planes)
    {
        p->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));
    }
}

void Project::populateCommandBuffer(VkCommandBuffer commandBuffer, int i) {
    for(PlaneView* p : Planes)
    {
        p->populateCommandBuffer(commandBuffer, i);
    }
    tiles->populateCommandBuffer(commandBuffer, i);
}

void Project::setWindowParameters() {
    windowWidth = 1280;
    windowHeight = 720;
    windowTitle = "TIMEPILOT 0.1";
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


    glm::mat4 S = updateCam(Ar, deltaT, m, r, false);

    for(TileInfo* info : tiles->floorTiles)
    {
        info->ubo.worldViewProj = S * info->ubo.model;
        info->DS.map(currentImage, &info->ubo, sizeof(info->ubo), 0);

        info->DS.map(currentImage, &gubo, sizeof(gubo), 2);
    }

    for(TileInfo* info : tiles->houseTiles)
    {
        info->ubo.worldViewProj = S * info->ubo.model;
        info->DS.map(currentImage, &info->ubo, sizeof(info->ubo), 0);

        info->DS.map(currentImage, &gubo, sizeof(gubo), 2);
    }

    for(TileInfo* info : tiles->skyscraperTiles)
    {
        info->ubo.worldViewProj = S * info->ubo.model;
        info->DS.map(currentImage, &info->ubo, sizeof(info->ubo), 0);

        info->DS.map(currentImage, &gubo, sizeof(gubo), 2);
    }



    for(PlaneView* p : Planes)
    {
        p->ubo.model =  glm::rotate(p->ubo.model, deltaT *glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        p->ubo.worldViewProj = S * p->ubo.model;
        p->ubo.normal = glm::inverse(glm::transpose(p->ubo.model));

        p->DS.map(currentImage, &p->ubo, sizeof(p->ubo), 0);

        p->DS.map(currentImage, &gubo, sizeof(gubo), 2);
    }

}

void Project::pipelinesAndDescriptorSetsCleanup() {
    tiles->pipelineAndDSCleanup();
    for(PlaneView* p : Planes)
    {
        p->pipelineAndDSClenup();
    }
}

void Project::localCleanup() {

    tiles->cleanup();
    for(PlaneView* p : Planes)
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
        PlaneView* p = new PlaneView();

        p->init(this);
        p->ubo.model = glm::mat4(1);
        p->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));

        Planes.push_back(p);

        printf("Added a cube!\n");
    }

    if(backSpace)
    {

        if(Planes.size() > 0)
        {
            vkDeviceWaitIdle(this->device);
            PlaneView* p = Planes[Planes.size() - 1];
            Planes.pop_back();
            p->pipelineAndDSClenup();

            p->cleanup();
            p->P.destroy();
            p->DSL.cleanup();
        }
    }


}

void Project::onWindowResize(int w, int h)
{

}

#endif