#ifndef CGPROJECT_2023_24_PIECE_HPP
#define CGPROJECT_2023_24_PIECE_HPP

#include "../Engine/Starter.hpp"
#include "../Model/Include/Enemy.h"

struct PlaneInfo
{
    Plane* pEnemy = nullptr;
    bool toDraw;
    DescriptorSet DS;
    UniformBufferObject ubo;
};

class PlaneView {
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;

    Model player;
    Model baseEnemy;
    Model Boss;
    Texture T;

    std::set<PlaneInfo*> enemyInfo;
    PlaneInfo* bossInfo;
    bool bossSpawned;
    PlaneInfo* playerInfo;

    std::vector<DescriptorSet*> toClean;

    BaseProject* app;


    void newPlayer(Player* pPlayer)
    {
        playerInfo = new PlaneInfo();
        playerInfo->pEnemy = pPlayer;
        playerInfo->ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 8.40f, 0.0f));
    }

    void newEnemy(Plane* enemy, int ubosize, int gubosize)
    {
        PlaneInfo* newEnemyInfo = new PlaneInfo();
        newEnemyInfo->pEnemy = enemy;
        newEnemyInfo->ubo.model = glm::translate(glm::mat4(1.0f), enemy->getPosition().origin);
        newEnemyInfo->DS.init(app, &DSL, {
                {0, UNIFORM, ubosize, nullptr},
                {1, TEXTURE, 0, &this->T},
                {2, UNIFORM, gubosize, nullptr}
        });
        enemyInfo.insert(newEnemyInfo);

    }

    void newBoss(Plane* enemy, int ubosize, int gubosize)
    {
        bossSpawned = true;
        bossInfo = new PlaneInfo();
        bossInfo->pEnemy = enemy;
        bossInfo->ubo.model = glm::translate(glm::mat4(1.0f), enemy->getPosition().origin);
        bossInfo->DS.init(app, &DSL, {
                {0, UNIFORM, ubosize, nullptr},
                {1, TEXTURE, 0, &this->T},
                {2, UNIFORM, gubosize, nullptr}
        });
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
                {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});

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
        this->T.init(bp, "../src/textures/cube.png");
        this->player.init(bp, &VD, "../src/models/plane.obj", OBJ);
        this->Boss.init(bp, &VD, "../src/models/dirigibile.obj", OBJ);
        this->baseEnemy.init(bp, &VD, "../src/models/Aereoaviatore.obj", OBJ);
    }

    void pipelineAndDSInit(BaseProject* bp, int ubosize, int gubosize){
        this->P.create();
        this->playerInfo->DS.init(bp, &this->DSL, {
                {0, UNIFORM, ubosize, nullptr},
                {1, TEXTURE, 0, &this->T},
                {2, UNIFORM, gubosize, nullptr}
        });
        if(!enemyInfo.empty())
        {
            for(PlaneInfo* info : enemyInfo)
            {
                info->DS.init(bp, &this->DSL, {
                        {0, UNIFORM, ubosize, nullptr},
                        {1, TEXTURE, 0, &this->T},
                        {2, UNIFORM, gubosize, nullptr}});
            }
        }

        if(bossSpawned)
        {
            bossInfo->DS.init(bp, &this->DSL, {
                    {0, UNIFORM, ubosize, nullptr},
                    {1, TEXTURE, 0, &this->T},
                    {2, UNIFORM, gubosize, nullptr}});
        }

    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);
        if(!enemyInfo.empty())
        {
            this->baseEnemy.bind(commandBuffer);
            for(PlaneInfo* planeInfo : enemyInfo)
            {
                if(planeInfo->toDraw)
                {
                    planeInfo->DS.bind(commandBuffer, this->P, 0, currentImage);

                    vkCmdDrawIndexed(commandBuffer,
                                     static_cast<uint32_t>(this->baseEnemy.indices.size()), 1, 0, 0, 0);
                }
            }
        }

        if(bossSpawned)
        {
            if (bossInfo->toDraw) {
                this->Boss.bind(commandBuffer);
                this->bossInfo->DS.bind(commandBuffer, this->P, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                                 static_cast<uint32_t>(this->Boss.indices.size()), 1, 0, 0, 0);
            }
        }

        if(playerInfo->toDraw)
        {
            this->player.bind(commandBuffer);
            this->playerInfo->DS.bind(commandBuffer, this->P, 0, currentImage);
            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(this->player.indices.size()), 1, 0, 0, 0);
        }
    }

    void cleanup(){
        this->T.cleanup();
        this->baseEnemy.cleanup();
        this->Boss.cleanup();
        this->player.cleanup();
        this->P.destroy();
        this->DSL.cleanup();
    }

    void pipelineAndDSCleanup(){
        this->P.cleanup();
        for(PlaneInfo* planeInfo : enemyInfo)
            planeInfo->DS.cleanup();
        playerInfo->DS.cleanup();
        if(bossSpawned)
            bossInfo->DS.cleanup();
    }


};


#endif //CGPROJECT_2023_24_PIECE_HPP
