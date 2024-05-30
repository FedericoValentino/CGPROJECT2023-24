
#ifndef CGPRJ2023_24_TILEVIEW_H
#define CGPRJ2023_24_TILEVIEW_H

#include "../Engine/Starter.hpp"

class TileView {
public:
    DescriptorSetLayout DSL;
    VertexDescriptor VD;
    Pipeline P;
    Model M;
    Texture T;
    DescriptorSet DS;
    UniformBufferObject ubo;

    BaseProject* app;

    int row_;
    int col_;

    TileView(int row, int col):
        row_(row), col_(col){}



    void init(BaseProject* bp, int type)
    {

        std::string modelURL;
        switch(type)
        {
            case 0:
                modelURL = "../src/models/floor.obj";
                break;
            case 1:
                modelURL = "../src/models/house.obj";
                break;
            case 2:
                modelURL = "../src/models/skyscraper.obj";
                break;
            default:
                exit(1);
                break;
        }


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

        this->P.init(bp, &VD, "../src/shaders/vert.spv", "../src/shaders/frag.spv", {&this->DSL});
        this->T.init(bp, "../src/textures/cube.png");
        this->M.init(bp, &VD, modelURL, OBJ);
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
        this->DS.bind(commandBuffer, this->P, 0, currentImage);

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
#endif //CGPRJ2023_24_TILEVIEW_H