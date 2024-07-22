#ifndef CGPRJ2023_24_TERRAIN_HPP
#define CGPRJ2023_24_TERRAIN_HPP

#include "../Engine/Starter.hpp"

struct terrainUBO
{
    glm::mat4 View;
    glm::mat4 Proj = constant::Proj;
    glm::vec3 pos;
};

class TerrainView{
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;
    DescriptorSet DS;
    terrainUBO ubo;

    BaseProject* app;



    void init(BaseProject* bp)
    {
        this->app = bp;

        this->DSL.init(bp, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT}});

        this->VD.init(bp, {}, {});

        this->P.init(bp, &VD, "../src/shaders/terrainShaderVert.spv", "../src/shaders/terrainShaderFrag.spv", {&this->DSL});
    }

    void pipelineAndDSInit(BaseProject* bp){
        this->P.create(false, 0, VK_SHADER_STAGE_ALL);
        this->DS.init(bp, &this->DSL, {
                {0, UNIFORM, sizeof(terrainUBO), nullptr}
        });
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);
        this->DS.bind(commandBuffer, this->P, 0, currentImage);
        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    }

    void cleanup(){
        this->P.destroy();
        this->DSL.cleanup();
    }

    void pipelineAndDSClenup(){
        this->P.cleanup();
        this->DS.cleanup();
    }


};
#endif //CGPRJ2023_24_TERRAIN_HPP