#include "hephResult.hpp"

std::string	HephResult::vkResultToString(VkResult result) {
  switch(result) {
    case VK_SUCCESS:
      return ("VK_SUCCESS");
    case VK_TIMEOUT:
      return ("VK_TIMEOUT");
    case VK_NOT_READY:
      return ("VK_NOT_READY");
    case VK_SUBOPTIMAL_KHR:
      return ("VK_SUBOPTIMAL_KHR");
    case VK_EVENT_SET:
      return ("VK_EVENT_SET");
    case VK_EVENT_RESET:
      return ("VK_EVENT_RESET");
    case VK_INCOMPLETE:
      return ("VK_INCOMPLETE");

    case VK_ERROR_OUT_OF_HOST_MEMORY:
      return ("VK_ERROR_OUT_OF_HOST_MEMORY");
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
      return ("VK_ERROR_OUT_OF_DEVICE_MEMORY");
    case VK_ERROR_INITIALIZATION_FAILED:
      return ("VK_ERROR_INITIALIZATION_FAILED");
    case VK_ERROR_DEVICE_LOST:
      return ("VK_ERROR_DEVICE_LOST");
    case VK_ERROR_MEMORY_MAP_FAILED:
      return ("VK_ERROR_MEMORY_MAP_FAILED");
    case VK_ERROR_LAYER_NOT_PRESENT:
      return ("VK_ERROR_LAYER_NOT_PRESENT");
    case VK_ERROR_EXTENSION_NOT_PRESENT:
      return ("VK_ERROR_EXTENSION_NOT_PRESENT");
    case VK_ERROR_FEATURE_NOT_PRESENT:
      return ("VK_ERROR_FEATURE_NOT_PRESENT");
    case VK_ERROR_INCOMPATIBLE_DRIVER:
      return ("VK_ERROR_INCOMPATIBLE_DRIVER");
    case VK_ERROR_TOO_MANY_OBJECTS:
      return ("VK_ERROR_TOO_MANY_OBJECTS");
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
      return ("VK_ERROR_FORMAT_NOT_SUPPORTED");
    case VK_ERROR_FRAGMENTED_POOL:
      return ("VK_ERROR_FRAGMENTED_POOL");
    case VK_ERROR_UNKNOWN:
      return ("VK_ERROR_UNKNOWN");
      // Provided by VK_VERSION_1_1:
    case VK_ERROR_OUT_OF_POOL_MEMORY:
      return ("VK_ERROR_OUT_OF_POOL_MEMORY");
      // Provided by VK_VERSION_1_1:
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
      return ("VK_ERROR_INVALID_EXTERNAL_HANDLE");
      // Provided by VK_VERSION_1_2:
      return ("VK_ERROR_FRAGMENTATION");
      // Provided by VK_VERSION_1_2:
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
      return ("VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS");
      // Provided by VK_VERSION_1_3:
    case VK_PIPELINE_COMPILE_REQUIRED:
      return ("VK_PIPELINE_COMPILE_REQUIRED");
      // Provided by VK_KHR_surface:
    case VK_ERROR_SURFACE_LOST_KHR:
      return ("VK_ERROR_SURFACE_LOST_KHR");
      // Provided by VK_KHR_surface:
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
      return ("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR");
      // Provided by VK_KHR_swapchain:
    case VK_ERROR_OUT_OF_DATE_KHR:
      return ("VK_ERROR_OUT_OF_DATE_KHR");
      // Provided by VK_KHR_display_swapchain:
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
      return ("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR");
      // Provided by VK_EXT_debug_report:
    case VK_ERROR_VALIDATION_FAILED_EXT:
      return ("VK_ERROR_VALIDATION_FAILED_EXT");
      // Provided by VK_NV_glsl_shader:
    case VK_ERROR_INVALID_SHADER_NV:
      return ("VK_ERROR_INVALID_SHADER_NV");
      // Provided by VK_EXT_image_drm_format_modifier:
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
      return ("VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT");
      // Provided by VK_KHR_global_priority:
    case VK_ERROR_NOT_PERMITTED_KHR:
      return ("VK_ERROR_NOT_PERMITTED_KHR");
      // Provided by VK_EXT_full_screen_exclusive:
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
      return ("VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT");
      // Provided by VK_KHR_deferred_host_operations:
    case VK_THREAD_IDLE_KHR:
      return ("VK_THREAD_IDLE_KHR");
      // Provided by VK_KHR_deferred_host_operations:
    case VK_THREAD_DONE_KHR:
      return ("VK_THREAD_DONE_KHR");
      // Provided by VK_KHR_deferred_host_operations:
    case VK_OPERATION_DEFERRED_KHR:
      return ("VK_OPERATION_DEFERRED_KHR");
      // Provided by VK_KHR_deferred_host_operations:
    case VK_OPERATION_NOT_DEFERRED_KHR:
      return ("VK_OPERATION_NOT_DEFERRED_KHR");
      // Provided by VK_EXT_image_compression_control:
    case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
      return ("VK_ERROR_COMPRESSION_EXHAUSTED_EXT");
      // Provided by VK_EXT_descriptor_indexing:
    case VK_ERROR_FRAGMENTATION_EXT:
      return ("VK_ERROR_FRAGMENTATION_EXT");
#ifdef VK_ENABLE_BETA_EXTENSIONS
      return ("VK_ENABLE_BETA_EXTENSIONS");
      // Provided by VK_KHR_video_queue:
    case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
      return ("VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR");
      // Provided by VK_KHR_video_queue:
    case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
      return ("VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR");
      // Provided by VK_KHR_video_queue:
    case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
      return ("VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR");
      // Provided by VK_KHR_video_queue:
    case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
      return ("VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR");
      // Provided by VK_KHR_video_queue:
    case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
      return ("VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR");
      return ("VK_ENABLE_BETA_EXTENSIONS");
      // Provided by VK_KHR_video_queue:
    case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
      return ("VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR");
#endif
    case VK_RESULT_MAX_ENUM:
      return ("VK_RESULT_MAX_ENUM");
		default:
			return ("");
  }
  return ("");
}
