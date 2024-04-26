#ifndef CGPROJECT_2022_23_UTILS_H
#define CGPROJECT_2022_23_UTILS_H
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);

        //Can do instance rendering and per-vertex rendering
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        /*
         * Format is the specifier for the type of data the shader is receiving
         * float: VK_FORMAT_R32_SFLOAT
         * vec2: VK_FORMAT_R32G32_SFLOAT
         * vec3: VK_FORMAT_R32G32B32_SFLOAT
         * vec4: VK_FORMAT_R32G32B32A32_SFLOAT
         */
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

struct UniformBufferObject{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

#endif //CGPROJECT_2022_23_UTILS_H
