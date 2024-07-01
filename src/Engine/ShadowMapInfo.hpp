#ifndef CGPRJ2023_24_SHADOWMAPINFO_H
#define CGPRJ2023_24_SHADOWMAPINFO_H

#include "Starter.hpp"

struct shadowUBOPlane{

};

struct shadowUBOMap{

};

class ShadowMapInfo
{
    VkImage shadowImage;
    VkImageView shadowMapView;
    VkRenderPass shadowRenderPass;
    VkFramebuffer shadowFrameBuffer;

    VkPipeline planePipe;
    VkPipeline mapPipe;


    DescriptorSet mapDS;
    DescriptorSetLayout mapLayout;
    VertexDescriptor mapVD;

    DescriptorSet planeDS;
    DescriptorSetLayout planeLayout;
    VertexDescriptor planeVD;




public:
    ShadowMapInfo(VkDevice device, size_t width, size_t height, BaseProject* bp)
    {
        createShadowView(device, width, height);

        createShadowImageView(device);

        createShadowRenderPass(device);

        createShadowFrameBuffer(device, width, height);

        this->mapLayout.init(bp, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        });

        this->mapVD.init(bp, {
                {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
        }, {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
                                      sizeof(glm::vec3), POSITION}});

        createShadowPipeline(device, bp, mapPipe, &this->mapVD, {&this->mapLayout}, "../src/shaders/planeVert.spv");

        this->mapDS.init(bp, &mapLayout, {
                {0, UNIFORM, sizeof(shadowUBOMap), nullptr},
                {1, TEXTURE, 0, &this->T}} );

        this->planeLayout.init(bp, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        });

        this->planeVD.init(bp, {
                {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
        }, {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
             sizeof(glm::vec3), POSITION}});

        createShadowPipeline(device, bp, planePipe, &this->planeVD, {&this->planeLayout}, "../src/shaders/planeVert.spv");

        this->planeDS.init(bp, &planeLayout, {
                {0, UNIFORM, sizeof(shadowUBOPlane), nullptr}} );

    }


    void createShadowView(VkDevice device, size_t width, size_t height)
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.pNext = nullptr;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = VK_FORMAT_D32_SFLOAT;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                          VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.queueFamilyIndexCount = 0;
        imageInfo.pQueueFamilyIndices = nullptr;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.flags = 0;

        vkCreateImage(device, &imageInfo, nullptr, &shadowImage);
    }

    void createShadowImageView(VkDevice device)
    {
        VkImageViewCreateInfo view_info = {};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.pNext = nullptr;
        view_info.image = shadowImage;
        view_info.format = VK_FORMAT_D32_SFLOAT;
        view_info.components.r = VK_COMPONENT_SWIZZLE_R;
        view_info.components.g = VK_COMPONENT_SWIZZLE_G;
        view_info.components.b = VK_COMPONENT_SWIZZLE_B;
        view_info.components.a = VK_COMPONENT_SWIZZLE_A;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.flags = 0;

        vkCreateImageView(device, &view_info, nullptr, &shadowMapView);
    }

    void createShadowRenderPass(VkDevice device)
    {
        VkAttachmentDescription attachments[2];

        // Depth attachment (shadow map)
        attachments[0].format = VK_FORMAT_D32_SFLOAT;
        attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
        attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        attachments[0].flags = 0;

        // Attachment references from subpasses
        VkAttachmentReference depth_ref;
        depth_ref.attachment = 0;
        depth_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // Subpass 0: shadow map rendering
        VkSubpassDescription subpass[1];
        subpass[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass[0].flags = 0;
        subpass[0].inputAttachmentCount = 0;
        subpass[0].pInputAttachments = nullptr;
        subpass[0].colorAttachmentCount = 0;
        subpass[0].pColorAttachments = nullptr;
        subpass[0].pResolveAttachments = nullptr;
        subpass[0].pDepthStencilAttachment = &depth_ref;
        subpass[0].preserveAttachmentCount = 0;
        subpass[0].pPreserveAttachments = nullptr;

        // Create render pass
        VkRenderPassCreateInfo rp_info;
        rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        rp_info.pNext = nullptr;
        rp_info.attachmentCount = 1;
        rp_info.pAttachments = attachments;
        rp_info.subpassCount = 1;
        rp_info.pSubpasses = subpass;
        rp_info.dependencyCount = 0;
        rp_info.pDependencies = nullptr;
        rp_info.flags = 0;

        vkCreateRenderPass(device, &rp_info, nullptr, &shadowRenderPass);

    }


    void createShadowFrameBuffer(VkDevice device, size_t width, size_t height)
    {
        VkFramebufferCreateInfo fb_info;
        fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_info.pNext = nullptr;
        fb_info.renderPass = shadowRenderPass;
        fb_info.attachmentCount = 1;
        fb_info.pAttachments = &shadowMapView;
        fb_info.width = width;
        fb_info.height = height;
        fb_info.layers = 1;
        fb_info.flags = 0;

        vkCreateFramebuffer(device, &fb_info, nullptr, &shadowFrameBuffer);
    }

    VkShaderModule createShaderModule(VkDevice device,const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;

        VkResult result = vkCreateShaderModule(device, &createInfo, nullptr,
                                               &shaderModule);
        if (result != VK_SUCCESS) {
            PrintVkError(result);
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }

    void createShadowPipeline(VkDevice device, BaseProject* BP, VkPipeline pipe, VertexDescriptor* VD, std::vector<DescriptorSetLayout *> D,
                              const std::string& VertShader)
    {
        auto vertShaderCode = readFile(VertShader);
        VkShaderModule vertShaderModule = createShaderModule(device, vertShaderCode);


        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType =
                VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";


        VkPipelineShaderStageCreateInfo shaderStages[] =
                {vertShaderStageInfo};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType =
                VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        auto bindingDescription = VD->getBindingDescription();
        auto attributeDescriptions = VD->getAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescription.size());
        vertexInputInfo.vertexAttributeDescriptionCount =
                static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data();
        vertexInputInfo.pVertexAttributeDescriptions =
                attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType =
                VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) BP->getSwapChainExtent().width;
        viewport.height = (float) BP->getSwapChainExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = BP->getSwapChainExtent();

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType =
                VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        std::vector<VkDescriptorSetLayout> DSL(D.size());
        for(int i = 0; i < D.size(); i++) {
            DSL[i] = D[i]->descriptorSetLayout;
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType =
                VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = DSL.size();
        pipelineLayoutInfo.pSetLayouts = DSL.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional


        VkPipelineLayout layout;
        VkResult result = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr,
                                                 &layout);
        if (result != VK_SUCCESS) {
            PrintVkError(result);
            throw std::runtime_error("failed to create pipeline layout!");
        }

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType =
                VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType =
                VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = nullptr;
        pipelineInfo.pMultisampleState = nullptr;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = nullptr;
        pipelineInfo.pDynamicState = nullptr; // Optional
        pipelineInfo.layout = layout;
        pipelineInfo.renderPass = shadowRenderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
                                           &pipelineInfo, nullptr, &pipe);
        if (result != VK_SUCCESS) {
            PrintVkError(result);
            throw std::runtime_error("failed to create graphics pipeline!");
        }
    }
};




#endif //CGPRJ2023_24_SHADOWMAPINFO_H
