#include "vk_common.h"

namespace vulkan {

    VkMemoryRequirements GpuDevice::getAccelerationStructureMemorySize(VkAccelerationStructureNV accel)
    {
        VkAccelerationStructureMemoryRequirementsInfoNV memoryRequirementsInfo;
        memoryRequirementsInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV;
        memoryRequirementsInfo.pNext = nullptr;
        memoryRequirementsInfo.accelerationStructure = accel;
        memoryRequirementsInfo.type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_OBJECT_NV;
        VkMemoryRequirements2 memoryRequirements = {};
        if (this->__GetAccelerationStructureMemoryRequirementsNV) {
            this->__GetAccelerationStructureMemoryRequirementsNV(device_, &memoryRequirementsInfo, &memoryRequirements);
        }
        return memoryRequirements.memoryRequirements;
    }

    GpuRaytracingAccelerationStructure::GpuRaytracingAccelerationStructure(
		const ngfx::RaytracingASDesc& desc,
		VkAccelerationStructureNV inAs,
		GpuAllocator::MemoryItem const& memItem, GpuDevice* device)
        : acceleration_structure_(inAs)
		, mem_item_(memItem)
		, desc_(desc)
        , device_(device)
    {
        create_info_ = {
            VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV
        };
    }

    GpuRaytracingAccelerationStructure::~GpuRaytracingAccelerationStructure()
    {
        // destroy
		if (acceleration_structure_ != VK_NULL_HANDLE) {
			device_->getAllocator().freeAccelerationStructure(acceleration_structure_, mem_item_);
			device_->destroyAccelerationStructure(acceleration_structure_, NGFXVK_ALLOCATOR);
			acceleration_structure_ = VK_NULL_HANDLE;
		}
    }

    void GpuRaytracingAccelerationStructure::setLabel(const char * label)
    {
    }

    const char * GpuRaytracingAccelerationStructure::label() const
    {
        return nullptr;
    }

    ngfx::RaytracingAS* GpuDevice::newRaytracingAS(const ngfx::RaytracingASDesc* rtDesc, ngfx::Result * result)
    {
		VkAccelerationStructureCreateInfoNV as_info = { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV };
		VkAccelerationStructureNV as = VK_NULL_HANDLE;
		createAccelerationStructure(&as_info, NGFXVK_ALLOCATOR, &as);
		auto memoryRequirements = getAccelerationStructureMemorySize(as);
		VkMemoryAllocateInfo memoryAllocateInfo;
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.pNext = nullptr;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		GpuAllocator::MemoryItem memItem;
		mem_alloc_.allocateForAccelerationStructure(as, ngfx::StorageMode::OnGpu, memItem);
        return new GpuRaytracingAccelerationStructure(*rtDesc, as, memItem, this);
    }

    GpuRaytracingPipeline::GpuRaytracingPipeline(const ngfx::RaytracePipelineDesc * desc, GpuDevice * device)
        : GpuPipelineBase(device)
    {
        std::vector<VkRayTracingShaderGroupCreateInfoNV> shaderGroups({
            // group0 = [ raygen ]
            { VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV, nullptr, VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_NV, 0, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV },
            // group1 = [ miss ]
            { VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV, nullptr, VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_NV, 1, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV },
            // group2 = [ chit ]
            { VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV, nullptr, VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_NV, VK_SHADER_UNUSED_NV, 2, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV },
            // group3 = [ chit ]
            { VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV, nullptr, VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_NV, VK_SHADER_UNUSED_NV, 3, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV },
            });

        create_info_ = {
            VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV
        };
        //create_info_.stageCount = (uint32_t)shaderStages.size();
        //create_info_.pStages = shaderStages.data();
        create_info_.groupCount = (uint32_t)shaderGroups.size();
        create_info_.pGroups = shaderGroups.data();
        create_info_.maxRecursionDepth = 1;
        //create_info_.layout = _rtPipelineLayout;
        create_info_.basePipelineHandle = VK_NULL_HANDLE;
        create_info_.basePipelineIndex = 0;

        //vkCreateRayTracingPipelinesNV(_device, nullptr, 1, &rayPipelineInfo, nullptr, &_rtPipeline);
    }
    GpuRaytracingPipeline::~GpuRaytracingPipeline()
    {
    }
    ngfx::RaytracePipeline* GpuDevice::newRaytracePipeline(const ngfx::RaytracePipelineDesc * desc, ngfx::Result * result)
    {
        return nullptr;
    }
    void GpuRaytracingEncoder::buildAS(ngfx::RaytracingAS * src, ngfx::RaytracingAS * dest, ngfx::Buffer * scratch)
    {
    }
    void GpuRaytracingEncoder::copyAS(ngfx::RaytracingAS * src, ngfx::RaytracingAS * dest, ngfx::AccelerationStructureCopyMode mode)
    {
    }
    void GpuRaytracingEncoder::traceRay(ngfx::Buffer * rayGen, ngfx::BufferStride miss, ngfx::BufferStride hit, int width, int height)
    {
    }
}