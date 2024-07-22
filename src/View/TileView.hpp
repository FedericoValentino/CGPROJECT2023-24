
#ifndef CGPRJ2023_24_TILEVIEW_H
#define CGPRJ2023_24_TILEVIEW_H

#include "../Engine/Starter.hpp"
#include "../Model/Include/Partita.h"

struct TileUniformBufferObject {
    alignas(16) glm::mat4 proj = constant::Proj;
    alignas(16) glm::mat4 model[constant::MAPDIM*constant::MAPDIM];
    alignas(16) glm::mat4 normal[constant::MAPDIM*constant::MAPDIM];
};

struct SpotLightsFloorBuffer{
    glm::vec4 spotlightPosition[constant::MAXFLOORSPOTLIGHTS];
    glm::vec4 spotlightDirection = glm::vec4(0.0f,-1.0f,0.0f,1.0f);
    glm::vec4 spotlightColor = glm::vec4(0.878,0.565,0.227, 0.01);
    float spotLightCosIn = 0.98;
    float spotLightCosOut = 0.50;
    int counter = 0;
    float pad[1];
};


struct TileInfo{
    int row_;
    int col_;
    bool toDraw;
    UniformBufferObject ubo;
};

struct pushTile
{
    glm::mat4 view;
    alignas(4) int type = 0;
};

class TileView {
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    DescriptorSetLayout DSL2;
    VertexDescriptor VD;

    Model floor;
    Model house;
    Model skyscraper;

    Texture House;
    Texture Skyscraper;
    Texture Floor;

    std::vector<std::shared_ptr<TileInfo>> floorTiles;
    std::vector<std::shared_ptr<TileInfo>> houseTiles;
    std::vector<std::shared_ptr<TileInfo>> skyscraperTiles;
    SpotLightsFloorBuffer floorLights;

    BaseProject* app;

    TileUniformBufferObject tubo;
    DescriptorSet DSTiles;
    DescriptorSet DSLights;

    glm::mat4 view;

    const glm::mat4& newTile(int row, int col, int type)
    {
        std::shared_ptr<TileInfo> newInfo = std::make_shared<TileInfo>();

        newInfo->row_ = row;
        newInfo->col_ = col;

        newInfo->ubo.model = glm::mat4(1);
        newInfo->ubo.model = glm::translate(newInfo->ubo.model, glm::vec3(2.8072f * (constant::MAPDIM-1), 0.0f, 2.8072f * (constant::MAPDIM-1)));
        newInfo->ubo.model = glm::translate(newInfo->ubo.model, glm::vec3(-5.6144f * (float)row, 0.0f, -5.6144f * (float)col));
        newInfo->ubo.normal = glm::inverse(glm::transpose(newInfo->ubo.model));

        switch(type)
        {
            case 0:
                floorTiles.push_back(newInfo);
                break;
            case 1:
                houseTiles.push_back(newInfo);
                break;
            case 2:
                skyscraperTiles.push_back(newInfo);
                break;
        }
        return newInfo->ubo.model;
    }

    // check the "size" sorrounding square
    bool canSetTrue(const std::vector<std::vector<bool>>& matrix, int x, int y, int size) {
        int halfSize = size / 2;
        for (int i = std::max(0, x - halfSize); i <= std::min((int)matrix.size() - 1, x + halfSize); ++i) {
            for (int j = std::max(0, y - halfSize); j <= std::min((int)matrix[0].size() - 1, y + halfSize); ++j) {
                if (matrix[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    void floorObjectBuilder(const glm::mat4& model) {
        const glm::vec3& lightPos = glm::vec3(model[3][0],model[3][1],model[3][2]);

        floorLights.spotlightPosition[floorLights.counter] = glm::vec4(lightPos + glm::vec3(0.0f,6.0f,0.0f),1.0f);
        floorLights.counter++;
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
                {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
                {3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                {4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                {5,VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});


        this->P.init(bp, &VD, "../src/shaders/vert.spv", "../src/shaders/frag.spv", {&this->DSL,});

        this->House.init(bp, "../src/textures/House.png");
        this->Floor.init(bp, "../src/textures/Floor.png");
        this->Skyscraper.init(bp, "../src/textures/Skyscraper.png");

        this->floor.init(bp, &VD, "../src/models/floor.obj", OBJ);
        this->house.init(bp, &VD, "../src/models/house.obj", OBJ);
        this->skyscraper.init(bp, &VD, "../src/models/skyscraper.obj", OBJ);
    }

    void pipelineAndDSInit(BaseProject* bp, int ubosize, int gubosize){
        this->P.create(true, sizeof(pushTile), VK_SHADER_STAGE_VERTEX_BIT);

        DSTiles.init(bp, &this->DSL, {
                {0, STORAGE, ubosize,  nullptr},
                {1, TEXTURE, 0,        &this->Floor},
                {2, UNIFORM, gubosize, nullptr},
                {3, TEXTURE, 0,        &this->House},
                {4, TEXTURE, 0,        &this->Skyscraper},
                {5, UNIFORM,sizeof(SpotLightsFloorBuffer),nullptr}
        });
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);
        DSTiles.bind(commandBuffer, this->P, 0, currentImage);

        if(!floorTiles.empty())
        {
            pushTile push{view, FLOOR};
            vkCmdPushConstants(commandBuffer, this->P.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushTile), &push);
            this->floor.bind(commandBuffer);

            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(this->floor.indices.size()), floorTiles.size(), 0, 0, 0);
        }

        if(!houseTiles.empty())
        {
            pushTile push{view, HOUSE};
            vkCmdPushConstants(commandBuffer, this->P.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushTile), &push);

            this->house.bind(commandBuffer);

            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(this->house.indices.size()), houseTiles.size(), 0, 0, floorTiles.size());
        }


        if(!skyscraperTiles.empty())
        {
            pushTile push{view, SKYSCRAPER};
            vkCmdPushConstants(commandBuffer, this->P.pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushTile), &push);

            this->skyscraper.bind(commandBuffer);

            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(this->skyscraper.indices.size()), skyscraperTiles.size(), 0, 0, floorTiles.size() + houseTiles.size());
        }



    }

    void cleanup(){
        this->Floor.cleanup();
        this->House.cleanup();
        this->Skyscraper.cleanup();
        this->floor.cleanup();
        this->house.cleanup();
        this->skyscraper.cleanup();
        this->P.destroy();
        this->DSL.cleanup();
    }

    void pipelineAndDSCleanup(){
        this->P.cleanup();
        DSTiles.cleanup();
    }


};
#endif //CGPRJ2023_24_TILEVIEW_H
