#ifndef CGPRJ2023_24_PARTICLESYSTEM_HPP
#define CGPRJ2023_24_PARTICLESYSTEM_HPP

#include "../Engine/Starter.hpp"

#define MAXPARTICLES 20

struct Particle{
    glm::mat4 ViewProj;
    glm::mat4 Model;
    glm::vec4 directions[MAXPARTICLES];
    float time;
};

struct ParticleUniformBufferObject{
    glm::mat4 ViewProj;
    glm::mat4 Model[MAXPARTICLES];
    glm::vec4 directions[MAXPARTICLES*MAXPARTICLES];
    float time[MAXPARTICLES];
};


class ParticleSystem{
public:
    Pipeline P;
    DescriptorSetLayout DSL;
    VertexDescriptor VD;

    DescriptorSet DSParticle;

    std::vector<Particle> particles;
    std::vector<Particle> noLongerDrawn;

    ParticleUniformBufferObject pubo;

    BaseProject* app;

    void newParticle(const glm::mat4& model)
    {

        Particle particle;

        particle.Model = model;
        particle.time = 0.0f;
        for(int i = 0; i < MAXPARTICLES; i++)
        {
            particle.directions[i].x = -1 + 2*(float)rand()/(float)RAND_MAX;
            particle.directions[i].y = -1 + 2*(float)rand()/(float)RAND_MAX;
            particle.directions[i].z = -1 + 2*(float)rand()/(float)RAND_MAX;
            particle.directions[i].w = 1.0f;

            glm::normalize(particle.directions[i]);
        }

        particles.push_back(particle);
    }


    void init(BaseProject* bp)
    {
        this->app = bp;

        this->DSL.init(bp, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT}});

        this->VD.init(bp, {}, {});

        this->P.init(bp, &VD, "../src/shaders/particleShaderVert.spv", "../src/shaders/particleShaderFrag.spv", {&this->DSL});

        this->P.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, false);

    }

    void pipelineAndDSInit(BaseProject* bp){
        this->P.create(false, 0, VK_SHADER_STAGE_ALL);

        this->DSParticle.init(app, &this->DSL, {
                {0, UNIFORM, sizeof(ParticleUniformBufferObject), nullptr}
        });

    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage){
        this->P.bind(commandBuffer);
        if(!particles.empty())
        {
            DSParticle.bind(commandBuffer, this->P, 0, currentImage);
            vkCmdDraw(commandBuffer, 24, MAXPARTICLES * particles.size(), 0, 0);
        }


    }

    void cleanup(){
        this->P.destroy();
        this->DSL.cleanup();
    }

    void pipelineAndDSCleanup(){
        this->P.cleanup();
        DSParticle.cleanup();
    }


};
#endif //CGPRJ2023_24_PARTICLESYSTEM_HPP
