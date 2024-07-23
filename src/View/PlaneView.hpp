#ifndef CGPROJECT_2023_24_PIECE_HPP
#define CGPROJECT_2023_24_PIECE_HPP

#include "../Engine/Starter.hpp"
#include "../Model/Include/Enemy.h"


struct PlaneUniformBufferObject
{
    alignas(16) glm::mat4 ModelViewProj[20];
    alignas(16) glm::mat4 proj = constant::Proj;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 model[20];
    alignas(16) glm::mat4 normal[20];
};

struct PlaneInfo
{
    std::shared_ptr<Plane> pEnemy = nullptr;
    bool toDraw;
    int indexInPubo;
    UniformBufferObject ubo;
};

struct pushPlane
{
    alignas(4) int planeIndex;
    alignas(4) int planeType;
};

class PlaneView {
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    DescriptorSet DSPlane;
    VertexDescriptor VD;

    Model player;
    Model baseEnemy;
    Model Boss;

    Texture playerTexture;
    Texture enemyTexture;
    Texture bossTexture;

    int visibleEnemies;

    std::vector<std::shared_ptr<PlaneInfo>> enemyInfo;
    std::shared_ptr<PlaneInfo> bossInfo;
    bool bossSpawned;
    std::shared_ptr<PlaneInfo> playerInfo;

    PlaneUniformBufferObject pubo;

    //deleted toClean
    BaseProject* app;


    void newPlayer(const std::shared_ptr<Player>& pPlayer)
    {
        playerInfo = std::make_shared<PlaneInfo>();
        playerInfo->pEnemy = pPlayer;
        playerInfo->ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 8.40f, 0.0f));
        playerInfo->indexInPubo = 0;
    }

    void newEnemy(std::shared_ptr<Plane> enemy)
    {

        std::shared_ptr<PlaneInfo> newEnemyInfo = std::make_shared<PlaneInfo>();
        newEnemyInfo->pEnemy = enemy;
        newEnemyInfo->ubo.model = glm::translate(glm::mat4(1.0f), enemy->getPosition().origin);
        newEnemyInfo->indexInPubo = 2 + enemyInfo.size();
        enemyInfo.push_back(newEnemyInfo);

    }

    void newBoss(std::shared_ptr<Plane> enemy)
    {
        bossSpawned = true;
        bossInfo = std::make_shared<PlaneInfo>();
        bossInfo->pEnemy = enemy;
        bossInfo->ubo.model = glm::translate(glm::mat4(1.0f), enemy->getPosition().origin);
        bossInfo->indexInPubo = 1;
    }

    void init(BaseProject* bp)
    {
        this->app = bp;

        this->DSL.init(bp, {
                // this array contains the binding:
                // first  element : the binding number
                // second element : the type of element (buffer or texture)
                // third  element : the pipeline stage where it will be used
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
                {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                {4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}});

        this->VD.init(bp, {
            {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
        }, {
                {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
                 sizeof(glm::vec3), POSITION},
                {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
                 sizeof(glm::vec2), UV},
                {0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
                 sizeof(glm::vec3), NORMAL}
        });

        this->P.init(bp, &VD, "../src/shaders/planeVert.spv", "../src/shaders/planeFrag.spv", {&this->DSL});

        this->playerTexture.init(bp, "../src/textures/B2.png");
        this->enemyTexture.init(bp, "../src/textures/Biplane.png");
        this->bossTexture.init(bp, "../src/textures/Zeppelin.png");

        this->player.init(bp, &VD, "../src/models/player.obj", OBJ);
        this->Boss.init(bp, &VD, "../src/models/Zeppelin.obj", OBJ);
        this->baseEnemy.init(bp, &VD, "../src/models/Biplane.obj", OBJ);
    }

    void pipelineAndDSInit(BaseProject* bp, int ubosize, int gubosize){
        this->P.create(true, sizeof(pushPlane), VK_SHADER_STAGE_VERTEX_BIT);
        this->DSPlane.init(bp, &this->DSL, {
                {0, UNIFORM, ubosize, nullptr},
                {1, TEXTURE, 0, &this->playerTexture},
                {2, UNIFORM, gubosize, nullptr},
                {3, TEXTURE, 0, &this->enemyTexture},
                {4, TEXTURE, 0, &this->bossTexture}
        });
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);
        DSPlane.bind(commandBuffer, this->P, 0, currentImage);
        if(!enemyInfo.empty())
        {
            this->baseEnemy.bind(commandBuffer);
            pushPlane push{2,ENEMY};
            vkCmdPushConstants(commandBuffer, this->P.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushPlane), &push);
            vkCmdDrawIndexed(commandBuffer,static_cast<uint32_t>(this->baseEnemy.indices.size()), visibleEnemies, 0, 0, 2);
        }

        if(bossSpawned && !bossInfo->pEnemy->getDead())
        {
            this->Boss.bind(commandBuffer);
            pushPlane push{1,BOSS};
            vkCmdPushConstants(commandBuffer, this->P.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushPlane), &push);
            vkCmdDrawIndexed(commandBuffer,static_cast<uint32_t>(this->Boss.indices.size()), 1, 0, 0, 1);
        }

        if(playerInfo->toDraw)
        {
            this->player.bind(commandBuffer);
            pushPlane push{0,PLAYER};
            vkCmdPushConstants(commandBuffer, this->P.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushPlane), &push);
            vkCmdDrawIndexed(commandBuffer,static_cast<uint32_t>(this->player.indices.size()), 1, 0, 0, 0);
        }
    }

    void cleanup(){
        this->playerTexture.cleanup();
        this->bossTexture.cleanup();
        this->enemyTexture.cleanup();
        this->baseEnemy.cleanup();
        this->Boss.cleanup();
        this->player.cleanup();
        this->P.destroy();
        this->DSL.cleanup();
    }

    void pipelineAndDSCleanup(){
        this->P.cleanup();
        DSPlane.cleanup();
    }


};


#endif //CGPROJECT_2023_24_PIECE_HPP
