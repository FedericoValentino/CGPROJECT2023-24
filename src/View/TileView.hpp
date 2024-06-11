
#ifndef CGPRJ2023_24_TILEVIEW_H
#define CGPRJ2023_24_TILEVIEW_H

#include "../Engine/Starter.hpp"
#include "../Model/Include/Partita.h"

struct TileUniformBufferObject {
    alignas(16) glm::mat4 worldViewProj[MAPDIM*MAPDIM];
    alignas(16) glm::mat4 model[MAPDIM*MAPDIM];
    alignas(16) glm::mat4 normal[MAPDIM*MAPDIM];
};


struct TileInfo{
    int row_;
    int col_;
    bool toDraw;
    UniformBufferObject ubo;
};

class TileView {
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;

    Model floor;
    Model house;
    Model skyscraper;

    Texture House;
    Texture Skyscraper;
    Texture Floor;

    std::vector<TileInfo*> floorTiles;
    std::vector<TileInfo*> houseTiles;
    std::vector<TileInfo*> skyscraperTiles;

    BaseProject* app;

    TileUniformBufferObject tuboFloor;
    DescriptorSet DSFloor;
    TileUniformBufferObject tuboHouse;
    DescriptorSet DSHouse;
    TileUniformBufferObject tuboSkyscraper;
    DescriptorSet DSSkyscraper;

    void newTile(int row, int col, int type)
    {
        TileInfo* newInfo = new TileInfo();

        newInfo->row_ = row;
        newInfo->col_ = col;

        newInfo->ubo.model = glm::mat4(1);
        newInfo->ubo.model = glm::translate(newInfo->ubo.model, glm::vec3(2.8072f * (MAPDIM-1), 0.0f, 2.8072f * (MAPDIM-1)));
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


        this->P.init(bp, &VD, "../src/shaders/vert.spv", "../src/shaders/frag.spv", {&this->DSL});

        this->House.init(bp, "../src/textures/House.png");
        this->Floor.init(bp, "../src/textures/Floor.png");
        this->Skyscraper.init(bp, "../src/textures/Skyscraper.png");

        this->floor.init(bp, &VD, "../src/models/floor.obj", OBJ);
        this->house.init(bp, &VD, "../src/models/house.obj", OBJ);
        this->skyscraper.init(bp, &VD, "../src/models/skyscraper.obj", OBJ);
    }

    void pipelineAndDSInit(BaseProject* bp, int ubosize, int gubosize){
        this->P.create();

        DSFloor.init(bp, &this->DSL, {
                {0, STORAGE, ubosize,  nullptr},
                {1, TEXTURE, 0,        &this->Floor},
                {2, UNIFORM, gubosize, nullptr}
        });

        DSHouse.init(bp, &this->DSL, {
                {0, STORAGE, ubosize,  nullptr},
                {1, TEXTURE, 0,        &this->House},
                {2, UNIFORM, gubosize, nullptr}
        });

        DSSkyscraper.init(bp, &this->DSL, {
                {0, STORAGE, ubosize,  nullptr},
                {1, TEXTURE, 0,        &this->Skyscraper},
                {2, UNIFORM, gubosize, nullptr}
        });
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);



        if(!floorTiles.empty())
        {
            DSFloor.bind(commandBuffer, this->P, 0, currentImage);
            this->floor.bind(commandBuffer);

            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(this->floor.indices.size()), floorTiles.size(), 0, 0, 0);
        }

        if(!houseTiles.empty())
        {
            DSHouse.bind(commandBuffer, this->P, 0, currentImage);
            this->house.bind(commandBuffer);

            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(this->house.indices.size()), houseTiles.size(), 0, 0, 0);
        }


        if(!skyscraperTiles.empty())
        {
            DSSkyscraper.bind(commandBuffer, this->P, 0, currentImage);
            this->skyscraper.bind(commandBuffer);

            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(this->skyscraper.indices.size()), skyscraperTiles.size(), 0, 0, 0);
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
        DSHouse.cleanup();
        DSFloor.cleanup();
        DSSkyscraper.cleanup();
    }


};
#endif //CGPRJ2023_24_TILEVIEW_H
