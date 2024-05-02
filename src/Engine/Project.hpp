#ifndef PROJECT_H
#define PROJECT_H

#include "Starter.hpp"

class Project : public BaseProject
{
private:
    void localInit() final;

    void pipelinesAndDescriptorSetsInit() final;

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int i) final;

    void setWindowParameters() final;

    void updateUniformBuffer(uint32_t currentImage) final;

    void pipelinesAndDescriptorSetsCleanup() final;

    void localCleanup() final;
};

void Project::localInit() {

}

void Project::pipelinesAndDescriptorSetsInit() {

}

void Project::populateCommandBuffer(VkCommandBuffer commandBuffer, int i) {

}

void Project::setWindowParameters() {

}

void Project::updateUniformBuffer(uint32_t currentImage) {

}

void Project::pipelinesAndDescriptorSetsCleanup() {

}

void Project::localCleanup() {

}

#endif