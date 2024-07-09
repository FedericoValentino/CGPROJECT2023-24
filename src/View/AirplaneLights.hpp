//
// Created by Niccolò Feresini on 07/07/24.
//

#ifndef CGPRJ2023_24_AIRPLANELIGHTS_H
#define CGPRJ2023_24_AIRPLANELIGHTS_H
#include "../Engine/Starter.hpp"
#include "../Model/Include/Partita.h"

struct AirplaneVertex {
    glm::vec3 pos;
    glm::vec3 norm;
};

struct PlaneLightUniformBufferObject
{
    glm::mat4 WVP[10 * Partita::MAX_PLANE];
    alignas(4) int counter;
};

// flickering of the lights on the wings of the airplane
struct FlickeringSphere
{
    bulletFlicker flick;
};

struct PlaneLight{
    PlaneLightUniformBufferObject plubo;
    FlickeringSphere fo;
    DescriptorSet DS;
};


class AirplaneLights{

private:
    void MakeSphere(float radius, int rings, int slices, std::vector<std::array<float,6>> &vertices, std::vector<uint32_t> &indices)
    {
        vertices.clear();
        indices.clear();

        // Generate vertices
        for (int i = 0; i <= rings; ++i) {
            float phi = float(i) * float(M_PI) / float(rings);
            for (int j = 0; j <= slices; ++j) {
                float theta = float(j) * 2.0f * float(M_PI) / float(slices);
                float x = radius * std::sin(phi) * std::cos(theta);
                float y = radius * std::cos(phi);
                float z = radius * std::sin(phi) * std::sin(theta);
                float nx = x / radius;
                float ny = y / radius;
                float nz = z / radius;
                vertices.push_back({x, y, z, nx, ny, nz});
            }
        }

        // Generate indices
        for (int i = 0; i < rings; ++i) {
            for (int j = 0; j < slices; ++j) {
                int first = (i * (slices + 1)) + j;
                int second = first + slices + 1;
                indices.push_back(second);
                indices.push_back(first);
                indices.push_back(first + 1);

                indices.push_back(second + 1);
                indices.push_back(second);
                indices.push_back(first + 1);
            }
        }
    }



public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;
    BaseProject* app;
    PlaneLight planeLights;
    Model M;
    int& numberOfEnemies;

    AirplaneLights(int& nOEnemies):numberOfEnemies(nOEnemies){};

    void init(BaseProject* bp) {
        this->app = bp;
        this->DSL.init(bp, {
                // this array contains the binding:
                // first  element : the binding number
                // second element : the type of element (buffer or texture)
                // third  element : the pipeline stage where it will be used
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}});


        this->VD.init(bp, {
                {0, sizeof(AirplaneVertex), VK_VERTEX_INPUT_RATE_VERTEX}
        }, {
                              {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(AirplaneVertex, pos),
                                      sizeof(glm::vec3), POSITION},
                              {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(AirplaneVertex, norm),
                                      sizeof(glm::vec3), NORMAL}
                      });

        this->P.init(bp, &VD, "../src/shaders/planeLightsVert.spv", "../src/shaders/planeLightsFrag.spv", {&this->DSL});
        this->P.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false);

        std::vector<std::array<float, 6>> vertices_pos;
        MakeSphere(1.5f, 24, 48, vertices_pos, M.indices);//0.2
        int mainStride = VD.Bindings[0].stride;
        M.vertices = std::vector<unsigned char>(vertices_pos.size() * sizeof(AirplaneVertex), 0);
        for (int i = 0; i < vertices_pos.size(); i++) {
            AirplaneVertex *V_vertex = (AirplaneVertex *) (&(M.vertices[i * mainStride]));
            V_vertex->pos.x = vertices_pos[i][0];
            V_vertex->pos.y = vertices_pos[i][1];
            V_vertex->pos.z = vertices_pos[i][2];
            V_vertex->norm.x = vertices_pos[i][3];
            V_vertex->norm.y = vertices_pos[i][4];
            V_vertex->norm.z = vertices_pos[i][5];
        }

        M.initMesh(bp,&VD);
    }

    void pipelineAndDSInit(BaseProject* bp){
        this->P.create();

        this->planeLights.DS.init(bp, &this->DSL, {
                {0, UNIFORM, sizeof(PlaneLightUniformBufferObject), nullptr},
                {1, UNIFORM, sizeof(FlickeringSphere),nullptr}
        });
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);
        this->M.bind(commandBuffer);
        planeLights.DS.bind(commandBuffer, this->P, 0, currentImage);

        vkCmdDrawIndexed(commandBuffer,static_cast<uint32_t>(M.indices.size()),2*numberOfEnemies,0, 0,0);
        }

    void cleanup(){
        this->M.cleanup();
        this->DSL.cleanup();
        this->P.destroy();
    }

    void pipelineAndDSCleanup(){
        this->P.cleanup();
        planeLights.DS.cleanup();
    }
};

#endif //CGPRJ2023_24_AIRPLANELIGHTS_H
