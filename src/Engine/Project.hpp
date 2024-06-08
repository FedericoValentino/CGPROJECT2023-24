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
    PlaneView* planes;
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

    void destroyPlane();
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


    //TODO Change pointers
    this->planes = new PlaneView();
    planes->init(this);
    planes->newPlayer(partita->player);
    planes->newBoss(nullptr);

}

void Project::pipelinesAndDescriptorSetsInit() {
    tiles->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));
    planes->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));
}

void Project::populateCommandBuffer(VkCommandBuffer commandBuffer, int i) {
    planes->populateCommandBuffer(commandBuffer, i);
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


    //TODO buffer update sequence for planes
    planes->playerInfo->toDraw = true;
    planes->playerInfo->ubo.model = planes->playerInfo->ubo.model =  glm::rotate(planes->playerInfo->ubo.model, deltaT *glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    planes->playerInfo->ubo.worldViewProj = S * planes->playerInfo->ubo.model;
    planes->playerInfo->ubo.normal = glm::inverse(planes->playerInfo->ubo.model);
    planes->playerInfo->DS.map(currentImage, &planes->playerInfo->ubo, sizeof(planes->playerInfo->ubo), 0);

    planes->playerInfo->DS.map(currentImage, &gubo, sizeof(gubo), 2);

    planes->bossInfo->toDraw = false;
}

void Project::pipelinesAndDescriptorSetsCleanup() {
    tiles->pipelineAndDSCleanup();
    planes->pipelineAndDSClenup();
}

void Project::localCleanup() {

    tiles->cleanup();
    planes->cleanup();
}

void Project::onWindowResize(int w, int h)
{

}

void Project::gameLogic()
{
    //CHECK COLLISION
    partita->checkCollision();
    //RIMUOVI ROBA

    //SPAWN
    partita->spawn();
    //MUOVI PLAYER
    //partita->player->changeDirection();
    //MUOVI NEMICI
    for(int i = 0; i < 4; i++)
    {

    }
}

void Project::spawnPlane()
{
    //planes->newEnemy();
}

void Project::destroyPlane()
{
    vkDeviceWaitIdle(this->device);
}

#endif