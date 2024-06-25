#ifndef CGPRJ2023_24_INFINITEGRID_HPP
#define CGPRJ2023_24_INFINITEGRID_HPP

#include "../Engine/Starter.hpp"

struct gridUBO
{
    alignas(16) glm::mat4 ViewProj;
    alignas(16) glm::vec3 pos;
};

class GridView{
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;
    DescriptorSet DS;
    gridUBO ubo;

    BaseProject* app;



    void init(BaseProject* bp)
    {
        this->app = bp;

        this->DSL.init(bp, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}});

        this->VD.init(bp, {}, {});

        this->P.init(bp, &VD, "../src/shaders/gridVert.spv", "../src/shaders/gridFrag.spv", {&this->DSL});
    }

    void pipelineAndDSInit(BaseProject* bp){
        this->P.create();
        this->DS.init(bp, &this->DSL, {
                {0, UNIFORM, sizeof(gridUBO), nullptr}
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
#endif //CGPRJ2023_24_INFINITEGRID_HPP
