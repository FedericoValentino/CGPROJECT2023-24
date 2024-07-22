#ifndef PROJECT_H
#define PROJECT_H


#include "SoundEngine.hpp"
#include "ControlWrapper.cpp"
#include "Starter.hpp"
#include "../View/PlaneView.hpp"
#include "../View/TileView.hpp"
#include "../View/ParticleSystem.hpp"
#include "../View/BulletView.hpp"
#include "../Model/Include/Partita.h"
#include "../View/AirplaneLights.hpp"
#include "../View/Terrain.hpp"
#include <memory.h>
#include <thread>
#include <random>

using namespace std;

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

struct spotLightObject{
    glm::vec4 spotlightPosition;
    glm::vec4 spotlightDirection;
    glm::vec4 spotlightColor;
    float spotLightCosIn;
    float spotLightCosOut;
    float pad[2];
};

struct GlobalUniformBufferObject {
    pointLightObject pointLights[constant::MAXBULLETS];
    pointLightObject pointLightsAirplane[10 * Partita::MAX_PLANE]; // Enemy lights;
    pointLightObject explosions[constant::MAXBULLETS];
    spotLightObject spotlight;
    glm::vec4 ambientLight;
    glm::vec4 sky;
    glm::vec4 eyepos;
    directLightObject moon;
    alignas(4) int lightCounter = 0;
    alignas(4) int pointLightsAirplaneCounter = 0; // number of enemies
    alignas(4) int explosionCounter = 0;
};

struct dayCycleinfo {
    glm::vec4 dawnColor = {0.863,0.761,0.918, 1.0f};
    glm::vec4 noonColor = {0.11,0.635,0.89, 1.0f};
    glm::vec4 sunsetColor = {0.984,0.565,0.384, 1.0f};
    glm::vec4 nightColor = {0.18,0.267, 0.51, 1.0f};

    float dawnTime = 0.0f;
    float noonTime = 7.55f;
    float sunsetTime = 15.0f;
    float nightTime = 22.65f;
};

class Project : public BaseProject
{

private:

    std::shared_ptr<Partita> partita;
    std::shared_ptr<PlaneView> planes;
    std::shared_ptr<TileView> tiles;
    std::shared_ptr<BulletView> bullets;
    std::shared_ptr<ParticleSystem> particles;
    std::shared_ptr<AirplaneLights> planeLights;
    std::shared_ptr<TerrainView> terrain;
    GlobalUniformBufferObject gubo;

    Sound soundEngine;

    int numObj = 100;
    float Ar;

    float deltaT;
    float time;
    int numberOfEnemies = 0;
    glm::vec3 m = glm::vec3(0.0f);
    glm::vec3 r = glm::vec3(0.0f);
    bool isFirstPerson = false;

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

    void setMapUniform();

    void updateMapUniform(const glm::mat4& S, int currentImage);

    void updatePlayerUniform(const glm::mat4& S, int currentImage);

    void updatePlaneLightsUniform(int currentImage);

    void updateEnemyUniform(const glm::mat4& S, int currentImage);

    void updateBossUniform(const glm::mat4& S, int currentImage);

    void updateBulletsUniform(const glm::mat4& S, int currentImage);

    void updateParticlesUniforms(const glm::mat4& S, int currentImage);

    void updateLights();

    void updateEnemyLights();

    void sunCycle();
};

void Project::localInit() {

    soundEngine.init();

    this->partita = std::make_shared<Partita>();
    partita->generateWorld();

    this->terrain = std::make_shared<TerrainView>();
    terrain->init(this);

    this->particles = std::make_shared<ParticleSystem>();
    particles->init(this);

    this->planeLights = std::make_shared<AirplaneLights>(numberOfEnemies);
    planeLights->init(this);

    this->tiles = std::make_shared<TileView>();
    tiles->init(this);

    gubo.sky = glm::vec4(0.863,0.761,0.918, 1.0f);

    std::vector<std::vector<bool>> checker(constant::MAPDIM, std::vector<bool>(constant::MAPDIM, false));

    std::random_device rd;
    std::mt19937 gen(rd());
    float p = 0.3f;
    std::bernoulli_distribution dist(p);

    size_t counter = 0;
    for(int row = 0; row < constant::MAPDIM; row++)
    {
        for(int col = 0; col < constant::MAPDIM; col++)
        {
            const glm::mat4& model = tiles->newTile(row, col, partita->map[row][col]->height);

            if (counter < constant::MAXFLOORSPOTLIGHTS && partita->map[row][col]->height == 0 && tiles->canSetTrue(checker, row, col,7) && dist(gen)) {
                checker[row][col] = true;
                tiles->floorObjectBuilder(model);
                counter++;
            }
        }
    }



    //Light updates
    //TODO moon.color.w was 0.02f
    //gubo.ambientLight = glm::vec4(1.0f, 1.0f, 1.0f, 0.02f);
    gubo.ambientLight = glm::vec4(0.933,0.365,0.424, 0.3f);
    gubo.moon.direction = glm::vec4(-10.0f,-40.0f,0.0f,1.0f);
    //gubo.moon.color = glm::vec4(0.965f,0.945f,0.835f, 0.02f);             original night
    //gubo.moon.color = glm::vec4(0.984,0.565,0.384, 0.7f);                 sunset
    gubo.moon.color = glm::vec4(0.0, 0.0, 0.0, 1.0f);
    gubo.pointLightsAirplaneCounter = 0.0f;
    gubo.spotlight.spotlightColor = glm::vec4(0.0f); // spotLight del dirigibile


    //TODO Change pointers
    this->planes = std::make_shared<PlaneView>();
    planes->init(this);
    planes->newPlayer(partita->player);


    bullets = std::make_shared<BulletView>();
    bullets->init(this);
}

float LERP(float x, float x0, float y0, float x1, float y1)
{
    assert(x1 != x0);
    float numerator = (y0 * (x1 - x)) + (y1 * (x - x0));
    float denominator = x1 - x0;
    return numerator/denominator;
}

void Project::sunCycle()
{
        time += deltaT;
        float redch;
        float greench;
        float bluech;

        redch = 0.5 * (sin(0.052 * 2 * time) + 1);
        greench = 0.5 * (sin(0.052 * 2 * time) + 1);
        bluech = 0.5 * (0.86 * sin(0.052 * 2 * time) + 1);

        gubo.moon.color.x = redch;
        gubo.moon.color.y = greench;
        gubo.moon.color.z = bluech;
}

void Project::pipelinesAndDescriptorSetsInit() {
    terrain->pipelineAndDSInit(this, sizeof(GlobalUniformBufferObject));
    particles->pipelineAndDSInit(this);
    planeLights->pipelineAndDSInit(this);
    bullets->pipelineAndDSInit(this, sizeof(BulletUniformBufferObject), sizeof(FlickeringObject));
    tiles->pipelineAndDSInit(this, sizeof(TileUniformBufferObject), sizeof(GlobalUniformBufferObject));
    planes->pipelineAndDSInit(this, sizeof(PlaneUniformBufferObject), sizeof(GlobalUniformBufferObject));
}

void Project::populateCommandBuffer(VkCommandBuffer commandBuffer, int i) {
    terrain->populateCommandBuffer(commandBuffer, i);
    particles->populateCommandBuffer(commandBuffer, i);
    //planeLights->populateCommandBuffer(commandBuffer,i);
    planes->populateCommandBuffer(commandBuffer, i);
    tiles->populateCommandBuffer(commandBuffer, i);
    bullets->populateCommandBuffer(commandBuffer, i);
}

void Project::setWindowParameters() {
    windowWidth = 1280;
    windowHeight = 720;
    windowTitle = "TIMEPILOT 0.1";
    windowResizable = GLFW_TRUE;
    //initialBackgroundColor = {0.012f,0.031f,0.11f, 1.0f};
    initialBackgroundColor = {0.863,0.761,0.918, 1.0f};

    uniformBlocksInPool = numObj*2;
    texturesInPool = numObj;
    setsInPool = numObj;
}

void Project::setMapUniform()
{

    int offset = 0;
    //Map updates
    for(int i = 0; i < tiles->floorTiles.size(); i++)
    {
        auto info = tiles->floorTiles[i];

        tiles->tubo.model[i+offset] = info->ubo.model;
        tiles->tubo.normal[i+offset] = info->ubo.normal;
    }

    offset += tiles->floorTiles.size();

    for(int i = 0; i < tiles->houseTiles.size(); i++)
    {
        auto info = tiles->houseTiles[i];

        tiles->tubo.model[i+offset] = info->ubo.model;
        tiles->tubo.normal[i+offset] = info->ubo.normal;
    }
    offset += tiles->houseTiles.size();

    for(int i = 0; i < tiles->skyscraperTiles.size(); i++)
    {
        auto info = tiles->skyscraperTiles[i];

        tiles->tubo.model[i+offset] = info->ubo.model;
        tiles->tubo.normal[i+offset] = info->ubo.normal;
    }
}

void Project::updateMapUniform(const glm::mat4& S, int currentImage)
{
    tiles->DSTiles.map(currentImage, &gubo, sizeof(GlobalUniformBufferObject), 2);
    tiles->DSTiles.map(currentImage, &tiles->tubo, sizeof(TileUniformBufferObject), 0);
    tiles->DSTiles.map(currentImage,&tiles->floorLights,sizeof(SpotLightsFloorBuffer),5);
}


void Project::updatePlayerUniform(const glm::mat4& S, int currentImage)
{
    //Player Updates
    planes->playerInfo->toDraw = !partita->getPlayer()->getDead();
    planes->playerInfo->ubo.worldViewProj = S * planes->playerInfo->ubo.model; // S = View-Proj
    planes->playerInfo->ubo.normal = glm::inverse(planes->playerInfo->ubo.model);

    planes->pubo.ModelViewProj[0] = planes->playerInfo->ubo.worldViewProj;
    planes->pubo.model[0] = planes->playerInfo->ubo.model;
    planes->pubo.normal[0] = planes->playerInfo->ubo.normal;
}

void Project::updateEnemyUniform(const glm::mat4& S, int currentImage)
{
    //buffer update sequence for planes
    for(int i = 0; i < planes->enemyInfo.size(); i++)
    {
        auto info = planes->enemyInfo[i];
        auto pos = info->pEnemy->getPosition();

        info->ubo.model = glm::mat4(1);
        info->ubo.model = glm::translate(info->ubo.model, pos.origin);
        info->ubo.model = glm::scale(info->ubo.model, glm::vec3(10.0f));
        info->ubo.model = glm::rotate(info->ubo.model, pos.rotation.y, glm::vec3(0, 1, 0));
        info->ubo.model = glm::rotate(info->ubo.model, pos.rotation.x, glm::vec3(1, 0, 0));
        info->ubo.model = glm::rotate(info->ubo.model, pos.rotation.z, glm::vec3(0, 0, 1));

        info->ubo.worldViewProj = S * info->ubo.model;
        info->ubo.normal = glm::inverse(glm::transpose(info->ubo.model));

        planes->pubo.ModelViewProj[2+i] = info->ubo.worldViewProj;
        planes->pubo.model[2+i] = info->ubo.model;
        planes->pubo.normal[2+i] = info->ubo.normal;
    }

}


void Project::updateBossUniform(const glm::mat4& S, int currentImage)
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

        planes->pubo.ModelViewProj[1] = planes->bossInfo->ubo.worldViewProj;
        planes->pubo.model[1] = planes->bossInfo->ubo.model;
        planes->pubo.normal[1] = planes->bossInfo->ubo.normal;
    }

    planes->DSPlane.map(currentImage, &planes->pubo, sizeof(PlaneUniformBufferObject), 0);
    planes->DSPlane.map(currentImage, &gubo, sizeof(GlobalUniformBufferObject), 2);
}


void Project::updateBulletsUniform(const glm::mat4& S, int currentImage)
{
    if(!bullets->bulletInfo.empty())
    {
        for (int i = 0; i < bullets->bulletInfo.size(); i++) {
            auto info = bullets->bulletInfo[i];


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
        bullets->DSBullet.map(currentImage, &bullets->fo, sizeof(FlickeringObject), 1);
    }
}


void Project::updateParticlesUniforms(const glm::mat4& S, int currentImage)
{
    for(auto& p : particles->particles)
    {
        p.pubo.ViewProj = S;
        p.DS.map(currentImage, &p.pubo, sizeof(particleUniformBufferObject), 0);
    }
}

void Project::updatePlaneLightsUniform(int currentImage)
{
    int i = 0;
    for(auto info:  planes->enemyInfo)
    {
        if(info->pEnemy->getType() == ENEMY &&  info->toDraw)
        {
            planeLights->planeLights.plubo.WVP[2*i] = info->ubo.worldViewProj * glm::translate(glm::mat4(1.0f),glm::vec3(25.0f,9.0f,-7.0f));
            planeLights->planeLights.plubo.WVP[2*i+1] = info->ubo.worldViewProj  * glm::translate(glm::mat4(1.0f),glm::vec3(-25.0f,9.0f,-7.0f));
            ++i;
        }
    }
    planeLights->planeLights.fo.flick.color = glm::vec4(1.0f,0.0f,0.0f,1.0f); // green
    planeLights->planeLights.fo.flick.size = 0.1;
    planeLights->planeLights.plubo.counter = 2*i;

    planeLights->planeLights.DS.map(currentImage,&planeLights->planeLights.plubo,sizeof(PlaneLightUniformBufferObject),0);
    planeLights->planeLights.DS.map(currentImage,&planeLights->planeLights.fo,sizeof(FlickeringSphere),1);
}

void Project::updateEnemyLights(){

    int i = 0;
    for(auto info : planes->enemyInfo)
    {
        if(info->pEnemy->getType() == ENEMY && info->toDraw)
        {
            gubo.pointLightsAirplane[2*i].PointlightPosition = glm::translate(glm::mat4(1.0f),glm::vec3(2.5f,0.0f,-1.45f)) *
                                                               glm::vec4(info->pEnemy->getPosition().origin,1.0f);
            gubo.pointLightsAirplane[2*i+1].PointlightPosition = glm::translate(glm::mat4(1.0f),glm::vec3(-2.5f,0.0f,-1.45f)) *
                                                                 glm::vec4(info->pEnemy->getPosition().origin,1.0f);
            gubo.pointLightsAirplane[2*i].pointLightColor = glm::vec4(1.0f,0.0f,0.0f,1.0f);
            gubo.pointLightsAirplane[2*i+1].pointLightColor = glm::vec4(1.0f,0.0f,0.0f,1.0f);
            gubo.pointLightsAirplane[2*i].size = 1.0;
            gubo.pointLightsAirplane[2*i+1].size = 1.0;
            ++i;
        }
    }
    gubo.pointLightsAirplaneCounter = 2*i;
}

void Project::updateLights()
{
    gubo.lightCounter = 0;
    for (int i = 0; i < bullets->bulletInfo.size(); i++) {

        bullets->fo.flick[i].color = bullets->bulletInfo[i]->color;

        gubo.pointLights[i].time = bullets->bulletInfo[i]->time;
        gubo.pointLights[i].size = bullets->bulletInfo[i]->pBullet->getSize();

        gubo.pointLights[i].PointlightPosition = glm::vec4(bullets->bulletInfo[i]->pBullet->getPosition3D().origin, 1.0f);
        gubo.pointLights[i].pointLightColor = bullets->bulletInfo[i]->color;
        gubo.lightCounter++;
    }

    if(partita->bossSpawned && !planes->bossInfo->pEnemy->getDead())
    {
        gubo.spotlight.spotlightColor = glm::vec4(0.431,0.051,0.204,0.4f);
        gubo.spotlight.spotlightPosition = glm::vec4(planes->bossInfo->pEnemy->getPosition().origin, 1.0f);
        gubo.spotlight.spotlightDirection = glm::vec4(glm::sin(planes->bossInfo->pEnemy->getPosition().rotation.y)*0.5,
                                                      -1.0f,
                                                      glm::cos(planes->bossInfo->pEnemy->getPosition().rotation.y)*0.5,
                                                      1.0f);
        gubo.spotlight.spotLightCosIn = 0.99f;
        gubo.spotlight.spotLightCosOut = 0.80f;
    }
    if(partita->bossSpawned && planes->bossInfo->pEnemy->getDead())
    {
        gubo.spotlight.spotlightColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        partita->state = END;
    }
}

/**
 * updateUniformBuffer performs all the necessary operations on our scene objects.
 * We start by setting up lights and then performing a camera update. From the camera matrix we then extract the frustum planes,
 * in order to do some frustum culling and prevent the CPU from sending too many draw calls to the GPU.
 * For every object we update the UBO, performing the per frame transformations we require.
 * @param currentImage
 */
void Project::updateUniformBuffer(uint32_t currentImage) {

    static bool mapUniformSet = false;

    std::array<glm::vec4,6> frustumPlanes;

    //update Starship world matrix
    glm::mat4 WorldMatrixPlane = updatePlaneMatrix(partita->player->getPosition(), true);
    planes->playerInfo->ubo.model = updatePlaneMatrix(partita->player->getPosition(), false);

    //Camera Update(View-Proj)
    auto [S,view] = updateCam(partita->player->getPosition(),WorldMatrixPlane,isFirstPerson, gubo.eyepos);

    //View - Proj for bullets
    bullets->buboBullet.view = view;

    //View - Proj for the map
    tiles->view = view;

    //View - Proj for grid
    terrain->ubo.View = view;
    terrain->ubo.pos = glm::vec3(1.0f);
    terrain->DS.map(currentImage, &terrain->ubo, sizeof(terrainUBO), 0);


    //View - Proj for planes
    for(auto info : planes->enemyInfo) {
        info->ubo.view = view;
    }

    planes->playerInfo->ubo.view = view;

    if(partita->bossSpawned) {
        planes->bossInfo->ubo.view = view;
    }

    //for FrustumCulling
    extractFrustumPlanes(frustumPlanes, S);

    updateLights();

    terrain->DS.map(currentImage, &gubo, sizeof(GlobalUniformBufferObject), 1);

    //updateEnemyLights();

    if(!mapUniformSet)
        setMapUniform();

    updateMapUniform(S, currentImage);

    updatePlayerUniform(S, currentImage);

    updateEnemyUniform(S, currentImage);

    updateBossUniform(S, currentImage);

    updateBulletsUniform(S, currentImage);

    updateParticlesUniforms(S, currentImage);

    //updatePlaneLightsUniform(currentImage);
}

void Project::pipelinesAndDescriptorSetsCleanup() {
    terrain->pipelineAndDSClenup();
    particles->pipelineAndDSCleanup();
    planeLights->pipelineAndDSCleanup();
    tiles->pipelineAndDSCleanup();
    planes->pipelineAndDSCleanup();
    bullets->pipelineAndDSCleanup();
}

void Project::localCleanup() {
    terrain->cleanup();
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
    static float quitTimer = 0.0f;
    static bool played = false;
    if(!played) {
        soundEngine.playIntro();
        played = true;
    }

    m = glm::vec3(0.0f);
    r = glm::vec3(0.0f);


    bool shoot = false;
    getSixAxis(deltaT, time, m, r, shoot,isFirstPerson);

    //INCREMENT INTERNAL CLOCK
    for(auto info : planes->enemyInfo)
    {
        info->pEnemy->timePasses(deltaT);
    }
    partita->player->timePasses(deltaT);
    if(partita->bossSpawned)
        planes->bossInfo->pEnemy->timePasses(deltaT);

    for(auto& p : particles->particles)
        p.pubo.time += deltaT;

    for(int i = 0; i < gubo.explosionCounter; i++)
    {
        gubo.explosions[i].time += deltaT;
    }

    //CHECK COLLISION
    if(partita->state != END)
        partita->checkCollision(deltaT);

    if(partita->state == END)
    {
        static auto start = std::chrono::high_resolution_clock::now();

        auto end = std::chrono::high_resolution_clock::now();

        float duration = std::chrono::duration<float, std::chrono::seconds::period>(end - start).count();

        if(quitTimer == 0.0f && partita->bossDead)
        {
            soundEngine.playZeppelinExpl();
            soundEngine.playVictory();
        }
        else if(quitTimer == 0.0f)
        {
            soundEngine.playPlayerExplosion();
            soundEngine.playGameOver();
        }
        quitTimer++;

        if(duration >= Sound::getSoundLength(soundEngine.zeppelinExploding) + Sound::getSoundLength(soundEngine.victory) ||
                duration >= Sound::getSoundLength(soundEngine.playerExplosion) + Sound::getSoundLength(soundEngine.gameOver))
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    //RIMUOVI ROBA(DA AGGIORNARE ANCHE IL COUNTER ENEMIES SU CUI SI BASANO LE LUCI DEGLI AEREI)

    //DELETE BULLETS
    auto it = std::remove_if(bullets->bulletInfo.begin(), bullets->bulletInfo.end(), [&](auto info) {
        info->time += deltaT;
        if (info->pBullet->toClear) {
            info->ubo.model = glm::scale(info->ubo.model, glm::vec3(10.0f));
            particles->newParticle(info->ubo.model);
            soundEngine.playbulletImpact();
            gubo.explosions[gubo.explosionCounter] = pointLightObject{glm::vec4(1.0, 0.5, 0.0, 3.0),
                                                                      glm::vec4(info->pBullet->getPosition3D().origin, 1.0f),
                                                                      0.0f,
                                                                      3.0f};
            gubo.explosionCounter++;

            return true;
        }
        return false;
    });

    // Erase the "toClear" elements from the vector
    bullets->bulletInfo.erase(it, bullets->bulletInfo.end());


    //Erasing Particles and DS cleanup
    auto itParticles = std::remove_if(particles->particles.begin(), particles->particles.end(), [&](Particle p)
    {
        vkDeviceWaitIdle(device);
        if(p.pubo.time>0.5f) {
            p.DS.cleanup();
            return true;
        }
        else
        {
            return false;
        }
    });

    particles->particles.erase(itParticles, particles->particles.end());

    //Erasing Dead Planes
    auto itPlanes = std::remove_if(planes->enemyInfo.begin(), planes->enemyInfo.end(), [&](std::shared_ptr<PlaneInfo> pi)
    {
        if(pi->pEnemy->getDead())
        {
            soundEngine.playComicalExplosion();
            for(int i = pi->indexInPubo; i < 20-1; i++)
                planes->pubo.model[i] = planes->pubo.model[i+1];
            return true;
        }
        else
            return false;
    });

    planes->enemyInfo.erase(itPlanes, planes->enemyInfo.end());

    //Overwriting explosions
    for(int i = 0; i < gubo.explosionCounter; i++)
    {
        if(gubo.explosions[i].time>0.6f)
        {
            for(int j = i; j < gubo.explosionCounter; j++)
            {
                gubo.explosions[j] = gubo.explosions[j+1];
            }
            gubo.explosionCounter--;
        }
    }


    //MAKE ENEMIES SHOOT
    if(partita->state != END)
    {
        for(auto info : planes->enemyInfo)
        {
            if(!info->pEnemy->getDead())
            {
                auto b = info->pEnemy->shoot(partita->player->getPosition(), deltaT);
                if (b)
                {
                    bullets->newBullet(b);
                    soundEngine.playEnemyGun();
                }
            }
        }
        //MAKE PLAYER SHOOT
        if(shoot)
        {
            auto b = partita->player->shoot({}, deltaT);
            if(b)
            {
                bullets->newBullet(b);
                soundEngine.playPlayerGun();
            }
        }


        //MAKE BOSS SHOOT
        if(partita->bossSpawned)
        {
            if(!planes->bossInfo->pEnemy->getDead())
            {
                auto b = planes->bossInfo->pEnemy->shoot(planes->playerInfo->pEnemy->getPosition(), deltaT);
                if (b)
                {
                    bullets->newBullet(b);
                    soundEngine.playZeppelinGun();
                }
            }
        }
    }



    //MAKE BULLETS MOVE
    for(auto info : partita->enemies)
    {
        for(auto bullet : info->getBullets())
        {
            bullet->move(deltaT);
        }
    }

    for(auto bullet : partita->player->getBullets())
    {
        bullet->move(deltaT);
    }

    if(partita->bossSpawned)
    {
        for(auto bullet : planes->bossInfo->pEnemy->getBullets())
        {
            bullet->move(deltaT);
        }
    }

    //SPAWN
    spawnPlane();

    //MUOVI PLAYER
    auto pl_pos = partita->getPlayer()->position;
    //update Plane position & orientation
    pl_pos = updatePlaneMatrix(pl_pos, deltaT, r);
    partita->player->setOrientation(pl_pos.rotation);
    partita->player->setPosition(pl_pos.origin);

    //MUOVI NEMICI
    for(auto info : planes->enemyInfo)
    {
        info->pEnemy->moveTowardsPoint(partita->player->getPosition(), deltaT);
    }

    //MUOVI BOSS
    if(partita->bossSpawned)
    {
        auto info = planes->bossInfo;

        std::shared_ptr<Boss> boss = std::dynamic_pointer_cast<Boss>(info->pEnemy);

        boss->bossMovement(partita->player->getPosition(), deltaT);
    }

    //update flickering time for the spheres
    planeLights->planeLights.fo.flick.time += deltaT;
    for(int i = 0; i<gubo.pointLightsAirplaneCounter; ++i)
        gubo.pointLightsAirplane[i].time += deltaT;

    numberOfEnemies = std::count_if(planes->enemyInfo.begin(),planes->enemyInfo.end(),
                                    [](auto info){return !info->pEnemy->getDead();});

    //SunCycle
    sunCycle();
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
                    planes->newEnemy(plane);
                    break;
                case BOSS:
                    planes->newBoss(plane);
                    break;
            }
        }
        time = 0;
    }
}



#endif