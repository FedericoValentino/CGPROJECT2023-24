#ifndef CGPRJ2023_24_BULLETVIEW_H
#define CGPRJ2023_24_BULLETVIEW_H

#include "../Engine/Starter.hpp"
#include "../Model/Include/Partita.h"

#define MAXBULLETS 400

struct BulletUniformBufferObject {
    alignas(16) glm::mat4 worldViewProj[MAXBULLETS];
    alignas(16) glm::mat4 model[MAXBULLETS];
    alignas(16) glm::mat4 normal[MAXBULLETS];
};


struct BulletInfo{
    Bullet* pBullet;
    UniformBufferObject ubo;
};

class BulletView {
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;

    Model bullet;

    Texture bulletTexture;

    std::vector<BulletInfo*> bulletInfo;

    BaseProject* app;

    BulletUniformBufferObject buboBullet;
    DescriptorSet DSBullet;

    void newBullet(Bullet* bullet)
    {
        BulletInfo* newInfo = new BulletInfo();

        newInfo->pBullet = bullet;
        newInfo->ubo.model = glm::mat4(1);
        newInfo->ubo.normal = glm::inverse(glm::transpose(newInfo->ubo.model));

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
                // this array contains the binding:
                // first  element : the binding number
                // second element : the type of element (buffer or texture)
                // third  element : the pipeline stage where it will be used
                {0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});


        this->P.init(bp, &VD, "../src/shaders/bulletVert.spv", "../src/shaders/bulletFrag.spv", {&this->DSL});

        this->bulletTexture.init(bp, "../src/textures/House.png");

        this->bullet.init(bp, &VD, "../src/models/floor.obj", OBJ);
    }

    void pipelineAndDSInit(BaseProject* bp, int ubosize, int gubosize){
        this->P.create();

        DSBullet.init(bp, &this->DSL, {
                {0, STORAGE, ubosize,  nullptr},
                {1, TEXTURE, 0,        &this->bulletTexture},
                {2, UNIFORM, gubosize, nullptr}
        });
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){

        if(!bulletInfo.empty())
        {
            this->P.bind(commandBuffer);
            DSBullet.bind(commandBuffer, this->P, 0, currentImage);
            this->bullet.bind(commandBuffer);

            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(this->bullet.indices.size()), bulletInfo.size(), 0, 0, 0);
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
