#include "qspch.h"
#include "RenderSystem.h"

namespace Quasar {

	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.0f };
		alignas(16) glm::vec3 color;
	};

	RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass) : _device(device)
	{
		CreatePipelineLayout();
		CreatePipeline(renderPass);
	}

	RenderSystem::~RenderSystem()
	{
		vkDestroyPipelineLayout(_device.device(), pipelineLayout, nullptr);
	}

	void RenderSystem::CreatePipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(_device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS)
		{
			QS_CORE_ERROR("Failed to create pipeline layout");
		}
	}
	void RenderSystem::CreatePipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "Can not create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<Pipeline>
			(
				_device,
				"D:/Code/QuasarEngine/bin/Debug-windows-x86_64/Sandbox/Shader/simple_shader.vert.spv",
				"D:/Code/QuasarEngine/bin/Debug-windows-x86_64/Sandbox/Shader/simple_shader.frag.spv",
				pipelineConfig
			);
	}

	void RenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects)
	{
		pipeline->Bind(commandBuffer);

		for (auto& obj : gameObjects)
		{
			obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + .01f, glm::two_pi<float>());
			obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + .005f, glm::two_pi<float>());

			SimplePushConstantData push{};
			push.color = obj.color;
			push.transform = obj.transform.mat4();

			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);

			obj.model->Bind(commandBuffer);
			obj.model->Draw(commandBuffer);
		}
	}
}