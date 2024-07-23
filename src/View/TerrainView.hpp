#ifndef CGPRJ2023_24_TERRAINVIEW_HPP
#define CGPRJ2023_24_TERRAINVIEW_HPP

#include "../Engine/Starter.hpp"

struct terrainUBO
{
    glm::mat4 View;
    glm::mat4 Proj = constant::Proj;
    glm::vec3 pos;
};

struct pushTerrain{
    glm::vec4 terrainColor;
};

class TerrainView{
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;
    DescriptorSet DS;
    terrainUBO ubo;

    BaseProject* app;

    glm::vec4 color;



    void init(BaseProject* bp)
    {
        this->app = bp;

        this->DSL.init(bp, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
                {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});

        this->VD.init(bp, {}, {});

        this->P.init(bp, &VD, "../src/shaders/terrainShaderVert.spv", "../src/shaders/terrainShaderFrag.spv", {&this->DSL});
    }

    void pipelineAndDSInit(BaseProject* bp, int gubosize){
        this->P.create(true, sizeof(pushTerrain), VK_SHADER_STAGE_FRAGMENT_BIT);
        this->DS.init(bp, &this->DSL, {
                {0, UNIFORM, sizeof(terrainUBO), nullptr},
                {1, UNIFORM, gubosize, nullptr}
        });
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);
        this->DS.bind(commandBuffer, this->P, 0, currentImage);
        pushTerrain push{color};
        vkCmdPushConstants(commandBuffer, this->P.pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pushTerrain), &push);
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
#endif //CGPRJ2023_24_TERRAINVIEW_HPP