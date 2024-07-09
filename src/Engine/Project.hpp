#ifndef PROJECT_H
#define PROJECT_H

#include "ControlWrapper.cpp"
#include "Starter.hpp"
#include "../View/PlaneView.hpp"
#include "../View/TileView.hpp"
#include "../View/ParticleSystem.hpp"
#include "../View/BulletView.hpp"
#include "../Model/Include/Partita.h"
#include "../View/AirplaneLights.hpp"


struct directLightObject{
    glm::vec4 color;
    glm::vec4 direction;
};

struct pointLightObject{
    glm::vec4 pointLightColor;
    glm::vec4 PointlightPosition;
    float time;
    float size;
    float pad[2];
};

struct GlobalUniformBufferObject {
    pointLightObject pointLights[MAXBULLETS + 2]; // plus two lights on the airplane
    glm::vec4 ambientLight;
    directLightObject moon;
    alignas(4)int lightCounter;
};

class Project : public BaseProject
{
private:

    Partita* partita;
    PlaneView* planes;
    TileView* tiles;
    BulletView* bullets;
    ParticleSystem* particles;
    AirplaneLights* planeLights;
    GlobalUniformBufferObject gubo;

    int numObj = 100;
    float Ar;

    float deltaT;
    float time;
    glm::vec3 m = glm::vec3(0.0f);
    glm::vec3 r = glm::vec3(0.0f);

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

    void updateMapUniform(glm::mat4 S, int currentImage);

    void updatePlayerUniform(glm::mat4 S, int currentImage);

    void updatePlaneLightsUniform(int currentImage);

    void updateEnemyUniform(glm::mat4 S, int currentImage);

    void updateBossUniform(glm::mat4 S, int currentImage);

    void updateBulletsUniform(glm::mat4 S, int currentImage);

    void updateParticlesUniforms(glm::mat4 S, int currentImage);

    void updateLights();
};

void Project::localInit() {

    this->partita = new Partita();
    partita->generateWorld();

    this->particles = new ParticleSystem();
    particles->init(this);

    this->planeLights = new AirplaneLights();
    planeLights->init(this);

    this->tiles = new TileView;
    tiles->init(this);

    for(int row = 0; row < MAPDIM; row++)
    {
        for(int col = 0; col < MAPDIM; col++)
        {
            tiles->newTile(row, col, partita->map[row][col]->height);
        }
    }

    //Light updates
    gubo.ambientLight = glm::vec4(1.0f, 1.0f, 1.0f, 0.02f);
    gubo.moon.direction = glm::vec4(glm::vec3(0.0f) - glm::vec3(40.0f), 1.0f);
    gubo.moon.color = glm::vec4(0.965f,0.945f,0.835f, 0.02f);


    //TODO Change pointers
    this->planes = new PlaneView();
    planes->init(this);
    planes->newPlayer(partita->player);


    bullets = new BulletView();
    bullets->init(this);
}

void Project::pipelinesAndDescriptorSetsInit() {
    particles->pipelineAndDSInit(this);
    planeLights->pipelineAndDSInit(this);
    bullets->pipelineAndDSInit(this, sizeof(BulletUniformBufferObject), sizeof(FlickeringObject));
    tiles->pipelineAndDSInit(this, sizeof(TileUniformBufferObject), sizeof(GlobalUniformBufferObject));
    planes->pipelineAndDSInit(this, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));
}

void Project::populateCommandBuffer(VkCommandBuffer commandBuffer, int i) {
    particles->populateCommandBuffer(commandBuffer, i);
    planeLights->populateCommandBuffer(commandBuffer,i);
    planes->populateCommandBuffer(commandBuffer, i);
    tiles->populateCommandBuffer(commandBuffer, i);
    bullets->populateCommandBuffer(commandBuffer, i);
}

void Project::setWindowParameters() {
    windowWidth = 1280;
    windowHeight = 720;
    windowTitle = "TIMEPILOT 0.1";
    windowResizable = GLFW_TRUE;
    initialBackgroundColor = {0.012f,0.031f,0.11f, 1.0f};

    uniformBlocksInPool = numObj*2;
    texturesInPool = numObj;
    setsInPool = numObj;

    Ar = 5.0f / 3.0f;
}

void Project::updateMapUniform(glm::mat4 S, int currentImage)
{
    //Map updates
    for(int i = 0; i < tiles->floorTiles.size(); i++)
    {
        TileInfo* info = tiles->floorTiles[i];
        glm::vec3 tilePosition = glm::vec3(info->ubo.model[3][0], info->ubo.model[3][1], info->ubo.model[3][2]);

        info->ubo.worldViewProj = S * info->ubo.model;

        tiles->tuboFloor.worldViewProj[i] = info->ubo.worldViewProj;
        tiles->tuboFloor.model[i] = info->ubo.model;
        tiles->tuboFloor.normal[i] = info->ubo.normal;
    }

    tiles->DSFloor.map(currentImage, &tiles->tuboFloor, sizeof(TileUniformBufferObject), 0);
    tiles->DSFloor.map(currentImage, &gubo, sizeof(GlobalUniformBufferObject), 2);

    for(int i = 0; i < tiles->houseTiles.size(); i++)
    {
        TileInfo* info = tiles->houseTiles[i];
        glm::vec3 tilePosition = glm::vec3(info->ubo.model[3][0], info->ubo.model[3][1], info->ubo.model[3][2]);

        info->ubo.worldViewProj = S * info->ubo.model;
        tiles->tuboHouse.worldViewProj[i] = info->ubo.worldViewProj;
        tiles->tuboHouse.model[i] = info->ubo.model;
        tiles->tuboHouse.normal[i] = info->ubo.normal;
    }

    tiles->DSHouse.map(currentImage, &tiles->tuboHouse, sizeof(TileUniformBufferObject), 0);
    tiles->DSHouse.map(currentImage, &gubo, sizeof(GlobalUniformBufferObject), 2);

    for(int i = 0; i < tiles->skyscraperTiles.size(); i++)
    {
        TileInfo* info = tiles->skyscraperTiles[i];
        glm::vec3 tilePosition = glm::vec3(info->ubo.model[3][0], info->ubo.model[3][1], info->ubo.model[3][2]);

        info->ubo.worldViewProj = S * info->ubo.model;
        tiles->tuboSkyscraper.worldViewProj[i] = info->ubo.worldViewProj;
        tiles->tuboSkyscraper.model[i] = info->ubo.model;
        tiles->tuboSkyscraper.normal[i] = info->ubo.normal;
    }

    tiles->DSSkyscraper.map(currentImage, &tiles->tuboSkyscraper, sizeof(TileUniformBufferObject), 0);
    tiles->DSSkyscraper.map(currentImage, &gubo, sizeof(GlobalUniformBufferObject), 2);
}


void Project::updatePlayerUniform(glm::mat4 S, int currentImage)
{
    //Player Updates
    planes->playerInfo->toDraw = true;
    planes->playerInfo->ubo.worldViewProj = S * planes->playerInfo->ubo.model; // S = View-Proj
    planes->playerInfo->ubo.normal = glm::inverse(planes->playerInfo->ubo.model);

    planes->playerInfo->DS.map(currentImage, &planes->playerInfo->ubo, sizeof(planes->playerInfo->ubo), 0);

    planes->playerInfo->DS.map(currentImage, &gubo, sizeof(GlobalUniformBufferObject), 2);
}

void Project::updateEnemyUniform(glm::mat4 S, int currentImage)
{
    //buffer update sequence for planes
    for(PlaneInfo* info : planes->enemyInfo)
    {
        auto pos = info->pEnemy->getPosition();
        info->toDraw = !info->pEnemy->getDead();

        info->ubo.model = glm::mat4(1);
        info->ubo.model = glm::translate(info->ubo.model, pos.origin);
        info->ubo.model = glm::scale(info->ubo.model, glm::vec3(0.1f, 0.1f, 0.1f));
        info->ubo.model = glm::rotate(info->ubo.model, pos.rotation.y, glm::vec3(0, 1, 0));
        info->ubo.model = glm::rotate(info->ubo.model, pos.rotation.z, glm::vec3(0, 0, 1));

        info->ubo.worldViewProj = S * info->ubo.model;
        info->ubo.normal = glm::inverse(glm::transpose(info->ubo.model));


        info->DS.map(currentImage, &info->ubo, sizeof(UniformBufferObject), 0);
        info->DS.map(currentImage, &gubo, sizeof(GlobalUniformBufferObject), 2);
    }
}


void Project::updateBossUniform(glm::mat4 S, int currentImage)
{
    if(partita->bossSpawned)
    {
        planes->bossInfo->toDraw = !planes->bossInfo->pEnemy->getDead();
        auto pos = planes->bossInfo->pEnemy->getPosition();

        planes->bossInfo->ubo.model = glm::mat4(1);
        planes->bossInfo->ubo.model = glm::translate(planes->bossInfo->ubo.model, pos.origin);
        planes->bossInfo->ubo.model = glm::rotate(planes->bossInfo->ubo.model, pos.rotation.y + 3.14f, glm::vec3(0, 1, 0));


        planes->bossInfo->ubo.worldViewProj = S * planes->bossInfo->ubo.model;
        planes->bossInfo->ubo.normal = glm::inverse(planes->bossInfo->ubo.model);

        planes->bossInfo->DS.map(currentImage, &planes->bossInfo->ubo, sizeof(planes->bossInfo->ubo), 0);

        planes->bossInfo->DS.map(currentImage, &gubo, sizeof(GlobalUniformBufferObject), 2);
    }
}


void Project::updateBulletsUniform(glm::mat4 S, int currentImage)
{
    if(!bullets->bulletInfo.empty())
    {
        for (int i = 0; i < bullets->bulletInfo.size(); i++) {
            BulletInfo *info = bullets->bulletInfo[i];


            info->ubo.model = glm::translate(glm::mat4(1), info->pBullet->getPosition3D().origin);
            info->ubo.model = glm::scale(info->ubo.model, glm::vec3(0.3f));
            info->ubo.model = glm::rotate(info->ubo.model, info->pBullet->getPosition3D().rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            info->ubo.worldViewProj = S * info->ubo.model;
            info->ubo.normal = glm::inverse(info->ubo.model);


            bullets->buboBullet.worldViewProj[i] = info->ubo.worldViewProj;
            bullets->buboBullet.model[i] = info->ubo.model;
            bullets->buboBullet.normal[i] = info->ubo.normal;

            bullets->fo.flick[i].time = bullets->bulletInfo[i]->time;
            bullets->fo.flick[i].size = bullets->bulletInfo[i]->pBullet->getSize();
        }
        bullets->DSBullet.map(currentImage, &bullets->buboBullet, sizeof(BulletUniformBufferObject), 0);
        bullets->DSBullet.map(currentImage, &bullets->fo, sizeof(FlickeringObject), 2);
    }
}


void Project::updateParticlesUniforms(glm::mat4 S, int currentImage)
{
    for(auto& p : particles->particles)
    {
        p.pubo.ViewProj = S;
        p.DS.map(currentImage, &p.pubo, sizeof(particleUniformBufferObject), 0);
    }
}

void Project::updatePlaneLightsUniform(int currentImage)
{

    planeLights->planeLights.plubo.View = planes->playerInfo->ubo.view;
    planeLights->planeLights.plubo.Proj = planes->playerInfo->ubo.proj;
    planeLights->planeLights.plubo.model = planes->playerInfo->ubo.model;
    planeLights->planeLights.plubo.WVP = planes->playerInfo->ubo.worldViewProj;
    planeLights->planeLights.plubo.translation[0] = glm::translate(glm::mat4(1.0f),glm::vec3(2.5f,0.0f,-1.45f));
    planeLights->planeLights.plubo.translation[1] = glm::translate(glm::mat4(1.0f),glm::vec3(-2.5f,0.0f,-1.45f));

    planeLights->planeLights.fo.flick.color = glm::vec4(0.0f,1.0f,0.0f,1.0f); // green
    planeLights->planeLights.fo.flick.size = 1;

    planeLights->planeLights.DS.map(currentImage,&planeLights->planeLights.plubo,sizeof(PlaneLightUniformBufferObject),0);
    planeLights->planeLights.DS.map(currentImage,&planeLights->planeLights.fo,sizeof(FlickeringSphere),1);
}

void Project::updateLights()
{
    gubo.lightCounter = 2;
    for (int i = 0; i < bullets->bulletInfo.size(); i++) {

        bullets->fo.flick[i].color = bullets->bulletInfo[i]->color;

        gubo.pointLights[i].time = bullets->bulletInfo[i]->time;
        gubo.pointLights[i].size = bullets->bulletInfo[i]->pBullet->getSize();

        gubo.pointLights[i].PointlightPosition = glm::vec4(bullets->bulletInfo[i]->pBullet->getPosition3D().origin, 1.0f);
        gubo.pointLights[i].pointLightColor = bullets->bulletInfo[i]->color;
        gubo.lightCounter++;
    }
    gubo.pointLights[MAXBULLETS].PointlightPosition  = glm::translate(glm::mat4(1.0f),glm::vec3(2.5f,0.0f,-1.45f)) *
                                                       glm::vec4(partita->player->getPosition().origin,1.0f);
    gubo.pointLights[MAXBULLETS + 1].PointlightPosition  = glm::translate(glm::mat4(1.0f),glm::vec3(-2.5f,0.0f,-1.45f)) *
                                                           glm::vec4(partita->player->getPosition().origin,1.0f);
    gubo.pointLights[MAXBULLETS + 1].PointlightPosition  = glm::vec4(0.0f,8.4f,0.0f,1.0f); // REMOVE
    gubo.pointLights[MAXBULLETS].pointLightColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    gubo.pointLights[MAXBULLETS + 1].pointLightColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    gubo.pointLights[MAXBULLETS].size = 10;
    gubo.pointLights[MAXBULLETS + 1].size = 10;
}

/**
 * updateUniformBuffer performs all the necessary operations on our scene objects.
 * We start by setting up lights and then performing a camera update. From the camera matrix we then extract the frustum planes,
 * in order to do some frustum culling and prevent the CPU from sending too many draw calls to the GPU.
 * For every object we update the UBO, performing the per frame transformations we require.
 * @param currentImage
 */
void Project::updateUniformBuffer(uint32_t currentImage) {
    glm::vec4 frustumPlanes[6];

    //update Starship world matrix
    glm::mat4& WorldMatrixPlane = planes->playerInfo->ubo.model;

    //update Plane world matrix
    updatePlaneMatrix(WorldMatrixPlane,partita->player->getPosition());

    //Camera Update(View-Proj)
    auto [S,proj,view] = updateCam(Ar, partita->player->getPosition(),WorldMatrixPlane);

    WorldMatrixPlane = glm::rotate(WorldMatrixPlane, partita->player->getPosition().rotation.z, glm::vec3(0, 0, 1));
    //light updates
    auto rotate = glm::rotate(glm::mat4(1.0f), deltaT, glm::vec3(0.0f, 1.0f, 0.0f));


    //View - Proj for bullets
    bullets->buboBullet.proj = proj;
    bullets->buboBullet.view = view;

    //View - Proj for the map
    tiles->tuboFloor.proj = proj;
    tiles->tuboFloor.view = view;

    tiles->tuboHouse.proj = proj;
    tiles->tuboHouse.view = view;

    tiles->tuboSkyscraper.proj = proj;
    tiles->tuboSkyscraper.view = view;

    //View - Proj for planes
    for(PlaneInfo* info : planes->enemyInfo) {
        info->ubo.proj = proj;
        info->ubo.view = view;
    }

    planes->playerInfo->ubo.proj = proj;
    planes->playerInfo->ubo.view = view;

    if(partita->bossSpawned) {
        planes->bossInfo->ubo.proj = proj;
        planes->bossInfo->ubo.view = view;
    }

    //for FrustumCulling
    extractFrustumPlanes(frustumPlanes, S);

    updateLights();

    updateMapUniform(S, currentImage);

    updatePlayerUniform(S, currentImage);

    updateEnemyUniform(S, currentImage);

    updateBossUniform(S, currentImage);

    updateBulletsUniform(S, currentImage);

    updateParticlesUniforms(S, currentImage);

    updatePlaneLightsUniform(currentImage);
}

void Project::pipelinesAndDescriptorSetsCleanup() {
    particles->pipelineAndDSCleanup();
    planeLights->pipelineAndDSCleanup();
    tiles->pipelineAndDSCleanup();
    planes->pipelineAndDSCleanup();
    bullets->pipelineAndDSCleanup();
}

void Project::localCleanup() {
    bullets->cleanup();
    particles->cleanup();
    planeLights->cleanup();
    tiles->cleanup();
    planes->cleanup();
}

void Project::onWindowResize(int w, int h)
{

}

void Project::gameLogic()
{
    m = glm::vec3(0.0f);
    r = glm::vec3(0.0f);


    bool shoot = false;
    getSixAxis(deltaT, time, m, r, shoot);

    //INCREMENT INTERNAL CLOCK
    for(PlaneInfo* info : planes->enemyInfo)
    {
        info->pEnemy->timePasses(deltaT);
    }
    partita->player->timePasses(deltaT);
    if(partita->bossSpawned)
        planes->bossInfo->pEnemy->timePasses(deltaT);

    for(auto& p : particles->particles)
        p.pubo.time += deltaT;

    //CHECK COLLISION
    partita->checkCollision();
    if(partita->state == END)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    //RIMUOVI ROBA

    //DELETE BULLETS
    //TODO ORRENDO CAMBIARE IN QUALCOSA PIU' BELLO
    std::vector<BulletInfo*> tmp;
    for(int i = 0; i < bullets->bulletInfo.size(); i++)
    {
        BulletInfo* info = bullets->bulletInfo[i];
        info->time += deltaT;
        if(!info->pBullet->toClear)
        {
            tmp.push_back(info);
        }
        else
        {
            info->ubo.model = glm::scale(info->ubo.model, glm::vec3(10.0f));
            particles->newParticle(info->ubo.model);
        }
    }

    bullets->bulletInfo.clear();
    for(int i = 0; i < tmp.size(); i++)
    {
        bullets->bulletInfo.push_back(tmp[i]);
    }

    //TODO DELETE PARTICLES


    //MAKE ENEMIES SHOOT
    for(PlaneInfo* info : planes->enemyInfo)
    {
        if(!info->pEnemy->getDead())
        {
            auto b = info->pEnemy->shoot(partita->player->getPosition(), deltaT);
            if (b)
                bullets->newBullet(b);
        }
    }
    //MAKE PLAYER SHOOT
    if(shoot)
    {
        auto b = partita->player->shoot({}, deltaT);
        if(b)
            bullets->newBullet(b);
    }


    //MAKE BOSS SHOOT
    if(partita->bossSpawned)
    {
        if(!planes->bossInfo->pEnemy->getDead())
        {
            auto b = planes->bossInfo->pEnemy->shoot(planes->playerInfo->pEnemy->getPosition(), deltaT);
            if (b)
                bullets->newBullet(b);
        }
    }



    //MAKE BULLETS MOVE
    for(PlaneInfo* info : planes->enemyInfo)
    {
        for(Bullet* bullet : info->pEnemy->getBullets())
        {
            bullet->move(deltaT);
        }
    }

    for(Bullet* bullet : partita->player->getBullets())
    {
        bullet->move(deltaT);
    }

    if(partita->bossSpawned)
    {
        for(Bullet* bullet : planes->bossInfo->pEnemy->getBullets())
        {
            bullet->move(deltaT);
        }
    }

    //SPAWN
    spawnPlane();


    //MUOVI PLAYER
    auto pl_pos = partita->getPlayer()->position;
    //update Plane position & orientation
    updatePlaneMatrix(pl_pos, deltaT, time, m, r);
    partita->player->setOrientation(pl_pos.rotation);
    partita->player->setPosition(pl_pos.origin);

    //MUOVI NEMICI
    for(PlaneInfo* info : planes->enemyInfo)
    {
        info->pEnemy->moveTowardsPoint(partita->player->getPosition(), deltaT);
    }

    //MUOVI BOSS
    if(partita->bossSpawned)
    {
        PlaneInfo* info = planes->bossInfo;

        Boss* boss = dynamic_cast<Boss*>(info->pEnemy);

        boss->bossMovement(partita->player->getPosition(), deltaT);
    }

    //update flickering time for the spheres
    planeLights->planeLights.fo.flick.time+= deltaT;
    gubo.pointLights[MAXBULLETS].time += deltaT;
    gubo.pointLights[MAXBULLETS + 1].time += deltaT;

}

void Project::spawnPlane()
{
    static float time = 0;

    time += deltaT;

    if(time > partita->spawnRate && !partita->bossSpawned)
    {
        auto plane = partita->spawn();
        if(plane)
        {
            switch(plane->getType())
            {
                case ENEMY:
                    planes->newEnemy(plane, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));
                    break;
                case BOSS:
                    planes->newBoss(plane, sizeof(UniformBufferObject), sizeof(GlobalUniformBufferObject));
                    break;
            }
        }
        time = 0;
    }
}



#endif