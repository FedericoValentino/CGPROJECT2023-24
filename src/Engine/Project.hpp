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
    std::set<PlaneView*> Planes;
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

    void gameLogic() final;

    void spawnPlane();

    void destroyPlane(PlaneView* p);
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
            tiles->newTile(row, col, partita->map[row][col]->height);
        }
    }


    PlaneView* p = new PlaneView();
    p->init(this);
    p->ubo.model = glm::mat4(1);
    p->ubo.model *= glm::translate(glm::mat4(1), glm::vec3(0.0, 8.40, 0.0));

    Planes.insert(p);

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


/**
 * updateUniformBuffer performs all the necessary operations on our scene objects.
 * We start by setting up lights and then performing a camera update. From the camera matrix we then extract the frustum planes,
 * in order to do some frustum culling and prevent the CPU from sending too many draw calls to the GPU.
 * For every object we update the UBO, performing the per frame transformations we require.
 * @param currentImage
 */
void Project::updateUniformBuffer(uint32_t currentImage) {

    float deltaT;
    float time;
    bool SpaceBar = false;
    bool BackSpace = false;
    GlobalUniformBufferObject gubo;
    glm::vec3 m = glm::vec3(0.0f);
    glm::vec3 r = glm::vec3(0.0f);
    glm::vec4 frustumPlanes[6];


    //Light updates
    gubo.lightDir = glm::vec3(cos(glm::radians(30.0f)), sin(glm::radians(30.0f)), 0.0f);
    gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gubo.eyePos = glm::vec3(100.0, 100.0, 100.0);


    //Camera Update
    getSixAxis(deltaT, time, m, r, SpaceBar, BackSpace);
    glm::mat4 S = updateCam(Ar, deltaT, m, r, false);


    //for FrustumCulling
    extractFrustumPlanes(frustumPlanes, S);



    //Entities updates
    for(TileInfo* info : tiles->floorTiles)
    {
        glm::vec3 tilePosition = glm::vec3(info->ubo.model[3][0], info->ubo.model[3][1], info->ubo.model[3][2]);

        info->toDraw = sphereInFrustum(frustumPlanes, tilePosition, 2.0f);
        info->ubo.worldViewProj = S * info->ubo.model;
        info->DS.map(currentImage, &info->ubo, sizeof(info->ubo), 0);

        info->DS.map(currentImage, &gubo, sizeof(gubo), 2);
    }

    for(TileInfo* info : tiles->houseTiles)
    {
        glm::vec3 tilePosition = glm::vec3(info->ubo.model[3][0], info->ubo.model[3][1], info->ubo.model[3][2]);

        info->toDraw = sphereInFrustum(frustumPlanes, tilePosition, 2.0f);
        info->ubo.worldViewProj = S * info->ubo.model;
        info->DS.map(currentImage, &info->ubo, sizeof(info->ubo), 0);

        info->DS.map(currentImage, &gubo, sizeof(gubo), 2);
    }

    for(TileInfo* info : tiles->skyscraperTiles)
    {
        glm::vec3 tilePosition = glm::vec3(info->ubo.model[3][0], info->ubo.model[3][1], info->ubo.model[3][2]);

        info->toDraw = sphereInFrustum(frustumPlanes, tilePosition, 2.0f);
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

void Project::onWindowResize(int w, int h)
{

}

void Project::gameLogic()
{

}

void Project::spawnPlane()
{
    PlaneView* p = new PlaneView();

    p->init(this);
    p->ubo.model = glm::mat4(1);
    p->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));

    Planes.insert(p);
}

void Project::destroyPlane(PlaneView* p)
{
    vkDeviceWaitIdle(this->device);
    Planes.erase(p);

    p->pipelineAndDSClenup();
    p->cleanup();
    p->P.destroy();
    p->DSL.cleanup();
}

#endif