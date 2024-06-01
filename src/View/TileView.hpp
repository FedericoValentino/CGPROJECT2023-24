
#ifndef CGPRJ2023_24_TILEVIEW_H
#define CGPRJ2023_24_TILEVIEW_H

#include "../Engine/Starter.hpp"
#include "../Model/Include/Partita.h"


struct TileInfo{
    int row_;
    int col_;
    DescriptorSet DS;
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

    Texture T;

    std::vector<TileInfo*> floorTiles;
    std::vector<TileInfo*> houseTiles;
    std::vector<TileInfo*> skyscraperTiles;

    BaseProject* app;


    void newTile(BaseProject* bp, int ubosize, int gubosize, int row, int col, int type)
    {
        TileInfo* newInfo = new TileInfo();

        newInfo->row_ = row;
        newInfo->col_ = col;

        newInfo->ubo.model = glm::mat4(1);
        newInfo->ubo.model = glm::translate(glm::mat4(1.0), glm::vec3((MAPDIM/2) * 2.80, 0.0, (MAPDIM/2) * 2.80));
        newInfo->ubo.model *= glm::translate(glm::mat4(1.0), glm::vec3(newInfo->row_ * 5.60 - 5.60 * (MAPDIM), 0.0, newInfo->col_ * 5.60 - 5.60 * (MAPDIM)));
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
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}});


        this->P.init(bp, &VD, "../src/shaders/vert.spv", "../src/shaders/frag.spv", {&this->DSL});

        this->T.init(bp, "../src/textures/cube.png");

        this->floor.init(bp, &VD, "../src/models/floor.obj", OBJ);
        this->house.init(bp, &VD, "../src/models/house.obj", OBJ);
        this->skyscraper.init(bp, &VD, "../src/models/skyscraper.obj", OBJ);
    }

    void pipelineAndDSInit(BaseProject* bp, int ubosize, int gubosize){
        this->P.create();
        for(TileInfo* info : floorTiles) {
            info->DS.init(bp, &this->DSL, {
                    {0, UNIFORM, ubosize,  nullptr},
                    {1, TEXTURE, 0,        &this->T},
                    {2, UNIFORM, gubosize, nullptr}
            });
        }

        for(TileInfo* info : houseTiles) {
            info->DS.init(bp, &this->DSL, {
                    {0, UNIFORM, ubosize,  nullptr},
                    {1, TEXTURE, 0,        &this->T},
                    {2, UNIFORM, gubosize, nullptr}
            });
        }

        for(TileInfo* info : skyscraperTiles) {
            info->DS.init(bp, &this->DSL, {
                    {0, UNIFORM, ubosize,  nullptr},
                    {1, TEXTURE, 0,        &this->T},
                    {2, UNIFORM, gubosize, nullptr}
            });
        }
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);

        if(!floorTiles.empty())
        {
            this->floor.bind(commandBuffer);

            for (TileInfo* info: floorTiles) {
                info->DS.bind(commandBuffer, this->P, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                                 static_cast<uint32_t>(this->floor.indices.size()), 1, 0, 0, 0);
            }
        }

        if(!houseTiles.empty())
        {
            this->house.bind(commandBuffer);

            for (TileInfo* info: houseTiles) {
                info->DS.bind(commandBuffer, this->P, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                                 static_cast<uint32_t>(this->house.indices.size()), 1, 0, 0, 0);
            }
        }


        if(!skyscraperTiles.empty())
        {
            this->skyscraper.bind(commandBuffer);

            for (TileInfo* info: skyscraperTiles) {
                info->DS.bind(commandBuffer, this->P, 0, currentImage);
                vkCmdDrawIndexed(commandBuffer,
                                 static_cast<uint32_t>(this->skyscraper.indices.size()), 1, 0, 0, 0);
            }
        }



    }

    void cleanup(){
        this->T.cleanup();
        this->floor.cleanup();
        this->house.cleanup();
        this->skyscraper.cleanup();
        this->P.destroy();
        this->DSL.cleanup();
    }

    void pipelineAndDSCleanup(){
        this->P.cleanup();
        for(TileInfo* info : floorTiles)
        {
            info->DS.cleanup();
        }
        for(TileInfo* info : houseTiles)
        {
            info->DS.cleanup();
        }
        for(TileInfo* info : skyscraperTiles)
        {
            info->DS.cleanup();
        }
    }


};
#endif //CGPRJ2023_24_TILEVIEW_H
