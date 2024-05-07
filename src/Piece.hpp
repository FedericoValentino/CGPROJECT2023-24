#ifndef CGPROJECT_2023_24_PIECE_HPP
#define CGPROJECT_2023_24_PIECE_HPP

#include "Engine/Starter.hpp"

struct UniformBufferObject {
    alignas(16) glm::mat4 worldViewProj;
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 normal;
};

class Piece {
public:
    DescriptorSetLayout DSL;
    Pipeline P;
    Model M;
    Texture T;
    DescriptorSet DS;
    UniformBufferObject ubo;

    BaseProject* app;

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

        this->P.init(bp, "../src/shaders/vert.spv", "../src/shaders/frag.spv", {&this->DSL});
        this->T.init(bp, "../src/textures/cube.png");
        this->M.init(bp, "../src/models/cube.obj");
    }

    void pipelineAndDSInit(BaseProject* bp, int ubosize, int gubosize){
        this->P.create();
        this->DS.init(bp, &this->DSL, {
                {0, UNIFORM, ubosize, nullptr},
                {1, TEXTURE, 0, &this->T},
                {2, UNIFORM, gubosize, nullptr}
        });
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);
        this->M.bind(commandBuffer);
        this->DS.bind(commandBuffer, this->P, currentImage);

        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(this->M.indices.size()), 1, 0, 0, 0);
    }

    void cleanup(){
        this->T.cleanup();
        this->M.cleanup();
    }

    void pipelineAndDSClenup(){
        this->P.cleanup();
        this->DS.cleanup();
    }


};


#endif //CGPROJECT_2023_24_PIECE_HPP
