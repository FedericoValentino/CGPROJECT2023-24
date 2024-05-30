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
    std::vector<TileView*> mapTiles;
    Model house;
    Model floor;
    Model skyscraper;

    VertexDescriptor VD;

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

    VD.init(this, {
            {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
    }, {
                          {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
                                  sizeof(glm::vec3), POSITION},
                          {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
                                  sizeof(glm::vec2), UV},
                          {0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
                                  sizeof(glm::vec3), NORMAL}
                  });

    house.init(this, &VD, "../src/models/house.obj", OBJ);
    floor.init(this, &VD, "../src/models/floor.obj", OBJ);
    skyscraper.init(this, &VD, "../src/models/floor.obj", OBJ);



    for(int row = 0; row < MAPDIM; row++)
    {
        for(int col = 0; col < MAPDIM; col++)
        {
            TileView* tp = new TileView(row, col);
            tp->init(this, partita->getMap(row, col)->height);
            switch(partita->getMap(row, col)->height)
            {
                case 0:
                    tp->M = floor;
                    break;
                case 1:
                    tp->M = house;
                    break;
                case 2:
                    tp->M = skyscraper;
                    break;
                default:
                    exit(1);
                    break;
            }
            tp->ubo.model = glm::mat4(1);
            tp->ubo.model = glm::translate(glm::mat4(1.0), glm::vec3((MAPDIM/2) * 2.80, 0.0, (MAPDIM/2) * 2.80));
            tp->ubo.model *= glm::translate(glm::mat4(1.0), glm::vec3(tp->row_ * 5.60 - 5.60 * (MAPDIM), 0.0, tp->col_ * 5.60 - 5.60 * (MAPDIM)));
            tp->ubo.normal = glm::inverse(glm::transpose(tp->ubo.model));
            mapTiles.push_back(tp);
        }
    }


    PlaneView* p = new PlaneView();

    p->init(this);
    p->ubo.model = glm::mat4(1);


    p->ubo.model *= glm::translate(glm::mat4(1), glm::vec3(0.0, 8.40, 0.0));

    Planes.push_back(p);

}

void Project::pipelinesAndDescriptorSetsInit() {
    for(TileView* tp : mapTiles)
    {
        tp->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));
    }
    for(PlaneView* p : Planes)
    {
        p->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));
    }
}

void Project::populateCommandBuffer(VkCommandBuffer commandBuffer, int i) {
    for(TileView* tp : mapTiles)
    {
        tp->populateCommandBuffer(commandBuffer, i);
    }
    for(PlaneView* p : Planes)
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
    printf("%f\n", deltaT);


    glm::mat4 S = updateCam(Ar, deltaT, m, r, false);


    for(TileView* tp : mapTiles)
    {
        tp->ubo.worldViewProj = S * tp->ubo.model;
        tp->DS.map(currentImage, &tp->ubo, sizeof(tp->ubo), 0);

        tp->DS.map(currentImage, &gubo, sizeof(gubo), 2);
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
    for(TileView* tp : mapTiles)
    {
        tp->pipelineAndDSClenup();
    }
    for(PlaneView* p : Planes)
    {
        p->pipelineAndDSClenup();
    }
}

void Project::localCleanup() {
    floor.cleanup();
    house.cleanup();
    skyscraper.cleanup();
    for(TileView* tp : mapTiles)
    {
        tp->cleanup();
        tp->P.destroy();
        tp->DSL.cleanup();
    }
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