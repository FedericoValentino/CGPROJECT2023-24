#ifndef CGPRJ2023_24_PARTICLESYSTEM_HPP
#define CGPRJ2023_24_PARTICLESYSTEM_HPP

#include "../Engine/Starter.hpp"

#define MAXPARTICLES 20

struct particleUniformBufferObject
{
    glm::mat4 ViewProj;
    glm::mat4 Model;
    glm::vec4 directions[MAXPARTICLES];
    alignas(4) float time;
};

struct Particle{
    particleUniformBufferObject pubo;
    DescriptorSet DS;
};

class ParticleSystem{
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;

    std::vector<Particle> particles;
    std::vector<Particle> noLongerDrawn;

    BaseProject* app;

    void newParticle(const glm::mat4& model)
    {

        Particle particle;

        particle.pubo.Model = model;
        particle.pubo.time = 0.0f;
        for(int i = 0; i < MAXPARTICLES; i++)
        {
            particle.pubo.directions[i].x = -1 + 2*(float)rand()/(float)RAND_MAX;
            particle.pubo.directions[i].y = -1 + 2*(float)rand()/(float)RAND_MAX;
            particle.pubo.directions[i].z = -1 + 2*(float)rand()/(float)RAND_MAX;
            particle.pubo.directions[i].w = 1.0f;

            glm::normalize(particle.pubo.directions[i]);
        }
        particle.DS.init(app, &this->DSL, {
                {0, UNIFORM, sizeof(particleUniformBufferObject), nullptr}
        });
        particles.push_back(particle);
    }


    void init(BaseProject* bp)
    {
        this->app = bp;

        this->DSL.init(bp, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT}});

        this->VD.init(bp, {}, {});

        this->P.init(bp, &VD, "../src/shaders/particleShaderVert.spv", "../src/shaders/particleShaderFrag.spv", {&this->DSL});

        this->P.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false);

    }

    void pipelineAndDSInit(BaseProject* bp){
        this->P.create(false, 0, VK_SHADER_STAGE_ALL);
        for(Particle &p: particles)
        {
            p.DS.init(app, &this->DSL, {
                    {0, UNIFORM, sizeof(particleUniformBufferObject), nullptr}
            });
        }
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);

        for(Particle p : particles)
        {
            p.DS.bind(commandBuffer, this->P, 0, currentImage);
            vkCmdDraw(commandBuffer, 24, MAXPARTICLES, 0, 0);
        }

    }

    void cleanup(){
        this->P.destroy();
        this->DSL.cleanup();
    }

    void pipelineAndDSCleanup(){
        this->P.cleanup();
        for(Particle p : particles)
        {
            p.DS.cleanup();
        }

    }


};
#endif //CGPRJ2023_24_PARTICLESYSTEM_HPP
