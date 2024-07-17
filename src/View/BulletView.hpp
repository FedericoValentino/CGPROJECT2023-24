#ifndef CGPRJ2023_24_BULLETVIEW_H
#define CGPRJ2023_24_BULLETVIEW_H

#include "../Engine/Starter.hpp"
#include "../Model/Include/Partita.h"

struct BulletUniformBufferObject {
    alignas(16) glm::mat4 worldViewProj[MAXBULLETS];
    alignas(16) glm::mat4 proj;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 model[MAXBULLETS];
    alignas(16) glm::mat4 normal[MAXBULLETS];
};

struct bulletFlicker
{
    glm::vec4 color;
    float time;
    float size;
    float pad[2];
};

struct FlickeringObject{
    bulletFlicker flick[MAXBULLETS];
};

struct BulletInfo{
    std::shared_ptr<Bullet> pBullet;
    UniformBufferObject ubo;
    glm::vec4 color;
    float time;
};

class BulletView {
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;

    Model bullet;

    Texture bulletTexture;

    std::vector<std::shared_ptr<BulletInfo>> bulletInfo;

    BaseProject* app;

    BulletUniformBufferObject buboBullet;
    DescriptorSet DSBullet;

    FlickeringObject fo;

    void newBullet(const std::shared_ptr<Bullet>& bullet)
    {
        std::shared_ptr<BulletInfo> newInfo = std::make_shared<BulletInfo>();

        newInfo->pBullet = bullet;
        newInfo->ubo.model = glm::mat4(1.0f);
        newInfo->ubo.normal = glm::inverse(glm::transpose(newInfo->ubo.model));

        switch (bullet->getType())
        {
            case ENEMY :
                newInfo->color = glm::vec4(1.0f, 0.0f, 0.0f, 3.0f);
                break;
            case PLAYER :
                newInfo->color = glm::vec4(0.0f, 1.0f, 0.0f, 3.0f);
                break;
            case BOSS :
                newInfo->color = glm::vec4(1.0f, 0.0f, 1.0f, 3.0f);
                break;
            default :
                break;
        }
        newInfo->time = 0;
        bulletInfo.push_back(newInfo);
    }

    void init(BaseProject* bp)
    {
        this->app = bp;

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

        this->DSL.init(bp, {
                {0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});

        for(int i=0; i<MAXBULLETS; i++)
            this->fo.flick[i].time = 0.0f;


        this->P.init(bp, &VD, "../src/shaders/bulletVert.spv", "../src/shaders/bulletFrag.spv", {&this->DSL});


        this->bullet.init(bp, &VD, "../src/models/stdbullet.obj", OBJ);
    }

    void pipelineAndDSInit(BaseProject* bp, int ubosize, int foSize){
        this->P.create(false, 0, VK_SHADER_STAGE_ALL);

        DSBullet.init(bp, &this->DSL, {
                {0, STORAGE, ubosize,  nullptr},
                {1, UNIFORM, foSize, nullptr}
        });
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){

        if(!bulletInfo.empty())
        {
            this->P.bind(commandBuffer);
            DSBullet.bind(commandBuffer, this->P, 0, currentImage);
            this->bullet.bind(commandBuffer);

            vkCmdDrawIndexed(commandBuffer,static_cast<uint32_t>(this->bullet.indices.size()), bulletInfo.size(), 0, 0, 0);
        }

    }

    void cleanup(){
        this->bullet.cleanup();
        this->bulletTexture.cleanup();
        this->P.destroy();
        this->DSL.cleanup();
    }

    void pipelineAndDSCleanup(){
        this->P.cleanup();
        this->DSBullet.cleanup();
    }


};

#endif //CGPRJ2023_24_BULLETVIEW_H
