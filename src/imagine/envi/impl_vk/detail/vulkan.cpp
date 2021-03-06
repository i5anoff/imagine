/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_vk/detail/vulkan.h"
#include "imagine/envi/impl_vk/detail/instance.h"
#include "imagine/envi/impl_vk/detail/device.h"

namespace ig {
namespace vk {

void vulkan::discover() {
  if (vulkan_.is_open())
    return;
  if (!vulkan_.open(VK_DYN_LOADER)) {
    throw std::runtime_error{
      "Vulkan loader not found, make sure your graphics driver is up-to-date"};
  }

  // VK_core
  vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>
    (vulkan_.resolve("vkGetInstanceProcAddr"));

  vkCreateInstance                       = reinterpret_cast<PFN_vkCreateInstance>                      (vkGetInstanceProcAddr(nullptr, "vkCreateInstance"));
  vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceExtensionProperties"));
  vkEnumerateInstanceLayerProperties     = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>    (vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceLayerProperties"));
}

void vulkan::acquire_fn(instance& i) {
  // VK_instance_core
  i->vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(vkGetInstanceProcAddr(i, "vkGetInstanceProcAddr"));
  i->vkGetDeviceProcAddr   = reinterpret_cast<PFN_vkGetDeviceProcAddr>(i->vkGetInstanceProcAddr(i, "vkGetDeviceProcAddr"));

  i->vkDestroyInstance                              = reinterpret_cast<PFN_vkDestroyInstance>                             (i->vkGetInstanceProcAddr(i, "vkDestroyInstance"));
  i->vkEnumeratePhysicalDevices                     = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>                    (i->vkGetInstanceProcAddr(i, "vkEnumeratePhysicalDevices"));
  i->vkGetPhysicalDeviceFeatures                    = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures>                   (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceFeatures"));
  i->vkGetPhysicalDeviceFormatProperties            = reinterpret_cast<PFN_vkGetPhysicalDeviceFormatProperties>           (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceFormatProperties"));
  i->vkGetPhysicalDeviceImageFormatProperties       = reinterpret_cast<PFN_vkGetPhysicalDeviceImageFormatProperties>      (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceImageFormatProperties"));
  i->vkGetPhysicalDeviceProperties                  = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>                 (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceProperties"));
  i->vkGetPhysicalDeviceQueueFamilyProperties       = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>      (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceQueueFamilyProperties"));
  i->vkGetPhysicalDeviceMemoryProperties            = reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties>           (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceMemoryProperties"));
  i->vkCreateDevice                                 = reinterpret_cast<PFN_vkCreateDevice>                                (i->vkGetInstanceProcAddr(i, "vkCreateDevice"));
  i->vkEnumerateDeviceExtensionProperties           = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>          (i->vkGetInstanceProcAddr(i, "vkEnumerateDeviceExtensionProperties"));
  i->vkEnumerateDeviceLayerProperties               = reinterpret_cast<PFN_vkEnumerateDeviceLayerProperties>              (i->vkGetInstanceProcAddr(i, "vkEnumerateDeviceLayerProperties"));
  i->vkGetPhysicalDeviceSparseImageFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceSparseImageFormatProperties>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceSparseImageFormatProperties"));

  // VK_KHR_surface
  i->vkDestroySurfaceKHR                       = reinterpret_cast<PFN_vkDestroySurfaceKHR>                      (i->vkGetInstanceProcAddr(i, "vkDestroySurfaceKHR"));
  i->vkGetPhysicalDeviceSurfaceSupportKHR      = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>     (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceSurfaceSupportKHR"));
  i->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
  i->vkGetPhysicalDeviceSurfaceFormatsKHR      = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>     (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
  i->vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceSurfacePresentModesKHR"));
  // VK_KHR_display
  i->vkGetPhysicalDeviceDisplayPropertiesKHR      = reinterpret_cast<PFN_vkGetPhysicalDeviceDisplayPropertiesKHR>     (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceDisplayPropertiesKHR"));
  i->vkGetPhysicalDeviceDisplayPlanePropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR"));
  i->vkGetDisplayPlaneSupportedDisplaysKHR        = reinterpret_cast<PFN_vkGetDisplayPlaneSupportedDisplaysKHR>       (i->vkGetInstanceProcAddr(i, "vkGetDisplayPlaneSupportedDisplaysKHR"));
  i->vkGetDisplayModePropertiesKHR                = reinterpret_cast<PFN_vkGetDisplayModePropertiesKHR>               (i->vkGetInstanceProcAddr(i, "vkGetDisplayModePropertiesKHR"));
  i->vkCreateDisplayModeKHR                       = reinterpret_cast<PFN_vkCreateDisplayModeKHR>                      (i->vkGetInstanceProcAddr(i, "vkCreateDisplayModeKHR"));
  i->vkGetDisplayPlaneCapabilitiesKHR             = reinterpret_cast<PFN_vkGetDisplayPlaneCapabilitiesKHR>            (i->vkGetInstanceProcAddr(i, "vkGetDisplayPlaneCapabilitiesKHR"));
  i->vkCreateDisplayPlaneSurfaceKHR               = reinterpret_cast<PFN_vkCreateDisplayPlaneSurfaceKHR>              (i->vkGetInstanceProcAddr(i, "vkCreateDisplayPlaneSurfaceKHR"));
  // VK_KHR_get_physical_device_properties2
  i->vkGetPhysicalDeviceFeatures2KHR                    = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures2KHR>                   (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceFeatures2KHR"));
  i->vkGetPhysicalDeviceProperties2KHR                  = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties2KHR>                 (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceProperties2KHR"));
  i->vkGetPhysicalDeviceFormatProperties2KHR            = reinterpret_cast<PFN_vkGetPhysicalDeviceFormatProperties2KHR>           (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceFormatProperties2KHR"));
  i->vkGetPhysicalDeviceImageFormatProperties2KHR       = reinterpret_cast<PFN_vkGetPhysicalDeviceImageFormatProperties2KHR>      (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceImageFormatProperties2KHR"));
  i->vkGetPhysicalDeviceQueueFamilyProperties2KHR       = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR>      (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceQueueFamilyProperties2KHR"));
  i->vkGetPhysicalDeviceMemoryProperties2KHR            = reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties2KHR>           (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceMemoryProperties2KHR"));
  i->vkGetPhysicalDeviceSparseImageFormatProperties2KHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR"));
  // VK_KHR_device_group_creation
  i->vkEnumeratePhysicalDeviceGroupsKHR = reinterpret_cast<PFN_vkEnumeratePhysicalDeviceGroupsKHR>(i->vkGetInstanceProcAddr(i, "vkEnumeratePhysicalDeviceGroupsKHR"));
  // VK_KHR_external_memory_capabilities
  i->vkGetPhysicalDeviceExternalBufferPropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceExternalBufferPropertiesKHR"));
  // VK_KHR_external_semaphore_capabilities
  i->vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR"));
  // VK_KHR_external_fence_capabilities
  i->vkGetPhysicalDeviceExternalFencePropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceExternalFencePropertiesKHR"));
  // VK_KHR_get_surface_capabilities2
  i->vkGetPhysicalDeviceSurfaceCapabilities2KHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceSurfaceCapabilities2KHR"));
  i->vkGetPhysicalDeviceSurfaceFormats2KHR      = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormats2KHR>     (i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceSurfaceFormats2KHR"));
  // VK_EXT_debug_report
  i->vkCreateDebugReportCallbackEXT  = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT> (i->vkGetInstanceProcAddr(i, "vkCreateDebugReportCallbackEXT"));
  i->vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(i->vkGetInstanceProcAddr(i, "vkDestroyDebugReportCallbackEXT"));
  i->vkDebugReportMessageEXT         = reinterpret_cast<PFN_vkDebugReportMessageEXT>        (i->vkGetInstanceProcAddr(i, "vkDebugReportMessageEXT"));
  // VK_EXT_direct_mode_display
  i->vkReleaseDisplayEXT = reinterpret_cast<PFN_vkReleaseDisplayEXT>(i->vkGetInstanceProcAddr(i, "vkReleaseDisplayEXT"));
  // VK_EXT_display_surface_counter
  i->vkGetPhysicalDeviceSurfaceCapabilities2EXT = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceSurfaceCapabilities2EXT"));
  // VK_EXT_debug_utils
  i->vkSetDebugUtilsObjectNameEXT    = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>   (i->vkGetInstanceProcAddr(i, "vkSetDebugUtilsObjectNameEXT"));
  i->vkSetDebugUtilsObjectTagEXT     = reinterpret_cast<PFN_vkSetDebugUtilsObjectTagEXT>    (i->vkGetInstanceProcAddr(i, "vkSetDebugUtilsObjectTagEXT"));
  i->vkQueueBeginDebugUtilsLabelEXT  = reinterpret_cast<PFN_vkQueueBeginDebugUtilsLabelEXT> (i->vkGetInstanceProcAddr(i, "vkQueueBeginDebugUtilsLabelEXT"));
  i->vkQueueEndDebugUtilsLabelEXT    = reinterpret_cast<PFN_vkQueueEndDebugUtilsLabelEXT>   (i->vkGetInstanceProcAddr(i, "vkQueueEndDebugUtilsLabelEXT"));
  i->vkQueueInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkQueueInsertDebugUtilsLabelEXT>(i->vkGetInstanceProcAddr(i, "vkQueueInsertDebugUtilsLabelEXT"));
  i->vkCmdBeginDebugUtilsLabelEXT    = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>   (i->vkGetInstanceProcAddr(i, "vkCmdBeginDebugUtilsLabelEXT"));
  i->vkCmdEndDebugUtilsLabelEXT      = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>     (i->vkGetInstanceProcAddr(i, "vkCmdEndDebugUtilsLabelEXT"));
  i->vkCmdInsertDebugUtilsLabelEXT   = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>  (i->vkGetInstanceProcAddr(i, "vkCmdInsertDebugUtilsLabelEXT"));
  i->vkCreateDebugUtilsMessengerEXT  = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT> (i->vkGetInstanceProcAddr(i, "vkCreateDebugUtilsMessengerEXT"));
  i->vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(i->vkGetInstanceProcAddr(i, "vkDestroyDebugUtilsMessengerEXT"));
  i->vkSubmitDebugUtilsMessageEXT    = reinterpret_cast<PFN_vkSubmitDebugUtilsMessageEXT>   (i->vkGetInstanceProcAddr(i, "vkSubmitDebugUtilsMessageEXT"));

  #ifdef VK_USE_PLATFORM_XLIB_KHR
  // VK_KHR_xlib_surface
  i->vkCreateXlibSurfaceKHR                        = reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>                       (i->vkGetInstanceProcAddr(i, "vkCreateXlibSurfaceKHR"));
  i->vkGetPhysicalDeviceXlibPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceXlibPresentationSupportKHR"));
  #endif
  #ifdef VK_USE_PLATFORM_XCB_KHR
  // VK_KHR_xcb_surface
  i->vkCreateXcbSurfaceKHR                        = reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>                       (i->vkGetInstanceProcAddr(i, "vkCreateXcbSurfaceKHR"));
  i->vkGetPhysicalDeviceXcbPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceXcbPresentationSupportKHR"));
  #endif
  #ifdef VK_USE_PLATFORM_WAYLAND_KHR
  // VK_KHR_wayland_surface
  i->vkCreateWaylandSurfaceKHR                        = reinterpret_cast<PFN_vkCreateWaylandSurfaceKHR>                       (i->vkGetInstanceProcAddr(i, "vkCreateWaylandSurfaceKHR"));
  i->vkGetPhysicalDeviceWaylandPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceWaylandPresentationSupportKHR"));
  #endif
  #ifdef VK_USE_PLATFORM_MIR_KHR
  // VK_KHR_mir_surface
  i->vkCreateMirSurfaceKHR                        = reinterpret_cast<PFN_vkCreateMirSurfaceKHR>                       (i->vkGetInstanceProcAddr(i, "vkCreateMirSurfaceKHR"));
  i->vkGetPhysicalDeviceMirPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceMirPresentationSupportKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceMirPresentationSupportKHR"));
  #endif
  #ifdef VK_USE_PLATFORM_WIN32_KHR
  // VK_KHR_win32_surface
  i->vkCreateWin32SurfaceKHR                        = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>                       (i->vkGetInstanceProcAddr(i, "vkCreateWin32SurfaceKHR"));
  i->vkGetPhysicalDeviceWin32PresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR>(i->vkGetInstanceProcAddr(i, "vkGetPhysicalDeviceWin32PresentationSupportKHR"));
  #endif
  #ifdef VK_USE_PLATFORM_ANDROID_KHR
  // VK_KHR_android_surface
  i->vkCreateAndroidSurfaceKHR = reinterpret_cast<PFN_vkCreateAndroidSurfaceKHR>(i->vkGetInstanceProcAddr(i, "vkCreateAndroidSurfaceKHR"));
  #endif
  #if defined(VK_USE_PLATFORM_MACOS_MVK)
  // VK_MVK_macos_surface
  i->vkCreateMacOSSurfaceMVK = reinterpret_cast<PFN_vkCreateMacOSSurfaceMVK>(i->vkGetInstanceProcAddr(i, "vkCreateMacOSSurfaceMVK"));
  #endif
  #if defined(VK_USE_PLATFORM_IOS_MVK)
  // VK_MVK_ios_surface
  i->vkCreateIOSSurfaceMVK = reinterpret_cast<PFN_vkCreateIOSSurfaceMVK>(i->vkGetInstanceProcAddr(i, "vkCreateIOSSurfaceMVK"));
  #endif
  #if defined(VK_USE_PLATFORM_VI_NN)
  // VK_NN_vi_surface
  i->vkCreateViSurfaceNN = reinterpret_cast<PFN_vkCreateViSurfaceNN>(i->vkGetInstanceProcAddr(i, "vkCreateViSurfaceNN"));
  #endif
}

void vulkan::acquire_fn(device& d) {
  // VK_device_core
  d->vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(d.inst->vkGetDeviceProcAddr(d, "vkGetDeviceProcAddr"));

  d->vkDestroyDevice  = reinterpret_cast<PFN_vkDestroyDevice> (d->vkGetDeviceProcAddr(d, "vkDestroyDevice"));
  d->vkGetDeviceQueue = reinterpret_cast<PFN_vkGetDeviceQueue>(d->vkGetDeviceProcAddr(d, "vkGetDeviceQueue"));
  d->vkQueueSubmit    = reinterpret_cast<PFN_vkQueueSubmit>   (d->vkGetDeviceProcAddr(d, "vkQueueSubmit"));
  d->vkQueueWaitIdle  = reinterpret_cast<PFN_vkQueueWaitIdle> (d->vkGetDeviceProcAddr(d, "vkQueueWaitIdle"));
  d->vkDeviceWaitIdle = reinterpret_cast<PFN_vkDeviceWaitIdle>(d->vkGetDeviceProcAddr(d, "vkDeviceWaitIdle"));

  d->vkAllocateMemory                   = reinterpret_cast<PFN_vkAllocateMemory>                  (d->vkGetDeviceProcAddr(d, "vkAllocateMemory"));
  d->vkFreeMemory                       = reinterpret_cast<PFN_vkFreeMemory>                      (d->vkGetDeviceProcAddr(d, "vkFreeMemory"));
  d->vkMapMemory                        = reinterpret_cast<PFN_vkMapMemory>                       (d->vkGetDeviceProcAddr(d, "vkMapMemory"));
  d->vkUnmapMemory                      = reinterpret_cast<PFN_vkUnmapMemory>                     (d->vkGetDeviceProcAddr(d, "vkUnmapMemory"));
  d->vkFlushMappedMemoryRanges          = reinterpret_cast<PFN_vkFlushMappedMemoryRanges>         (d->vkGetDeviceProcAddr(d, "vkFlushMappedMemoryRanges"));
  d->vkInvalidateMappedMemoryRanges     = reinterpret_cast<PFN_vkInvalidateMappedMemoryRanges>    (d->vkGetDeviceProcAddr(d, "vkInvalidateMappedMemoryRanges"));
  d->vkGetDeviceMemoryCommitment        = reinterpret_cast<PFN_vkGetDeviceMemoryCommitment>       (d->vkGetDeviceProcAddr(d, "vkGetDeviceMemoryCommitment"));
  d->vkBindBufferMemory                 = reinterpret_cast<PFN_vkBindBufferMemory>                (d->vkGetDeviceProcAddr(d, "vkBindBufferMemory"));
  d->vkBindImageMemory                  = reinterpret_cast<PFN_vkBindImageMemory>                 (d->vkGetDeviceProcAddr(d, "vkBindImageMemory"));
  d->vkGetBufferMemoryRequirements      = reinterpret_cast<PFN_vkGetBufferMemoryRequirements>     (d->vkGetDeviceProcAddr(d, "vkGetBufferMemoryRequirements"));
  d->vkGetImageMemoryRequirements       = reinterpret_cast<PFN_vkGetImageMemoryRequirements>      (d->vkGetDeviceProcAddr(d, "vkGetImageMemoryRequirements"));
  d->vkGetImageSparseMemoryRequirements = reinterpret_cast<PFN_vkGetImageSparseMemoryRequirements>(d->vkGetDeviceProcAddr(d, "vkGetImageSparseMemoryRequirements"));
  d->vkQueueBindSparse                  = reinterpret_cast<PFN_vkQueueBindSparse>                 (d->vkGetDeviceProcAddr(d, "vkQueueBindSparse"));

  d->vkCreateFence      = reinterpret_cast<PFN_vkCreateFence>     (d->vkGetDeviceProcAddr(d, "vkCreateFence"));
  d->vkDestroyFence     = reinterpret_cast<PFN_vkDestroyFence>    (d->vkGetDeviceProcAddr(d, "vkDestroyFence"));
  d->vkResetFences      = reinterpret_cast<PFN_vkResetFences>     (d->vkGetDeviceProcAddr(d, "vkResetFences"));
  d->vkGetFenceStatus   = reinterpret_cast<PFN_vkGetFenceStatus>  (d->vkGetDeviceProcAddr(d, "vkGetFenceStatus"));
  d->vkWaitForFences    = reinterpret_cast<PFN_vkWaitForFences>   (d->vkGetDeviceProcAddr(d, "vkWaitForFences"));
  d->vkCreateSemaphore  = reinterpret_cast<PFN_vkCreateSemaphore> (d->vkGetDeviceProcAddr(d, "vkCreateSemaphore"));
  d->vkDestroySemaphore = reinterpret_cast<PFN_vkDestroySemaphore>(d->vkGetDeviceProcAddr(d, "vkDestroySemaphore"));
  d->vkCreateEvent      = reinterpret_cast<PFN_vkCreateEvent>     (d->vkGetDeviceProcAddr(d, "vkCreateEvent"));
  d->vkDestroyEvent     = reinterpret_cast<PFN_vkDestroyEvent>    (d->vkGetDeviceProcAddr(d, "vkDestroyEvent"));
  d->vkGetEventStatus   = reinterpret_cast<PFN_vkGetEventStatus>  (d->vkGetDeviceProcAddr(d, "vkGetEventStatus"));
  d->vkSetEvent         = reinterpret_cast<PFN_vkSetEvent>        (d->vkGetDeviceProcAddr(d, "vkSetEvent"));
  d->vkResetEvent       = reinterpret_cast<PFN_vkResetEvent>      (d->vkGetDeviceProcAddr(d, "vkResetEvent"));

  d->vkCreateQueryPool     = reinterpret_cast<PFN_vkCreateQueryPool>    (d->vkGetDeviceProcAddr(d, "vkCreateQueryPool"));
  d->vkDestroyQueryPool    = reinterpret_cast<PFN_vkDestroyQueryPool>   (d->vkGetDeviceProcAddr(d, "vkDestroyQueryPool"));
  d->vkGetQueryPoolResults = reinterpret_cast<PFN_vkGetQueryPoolResults>(d->vkGetDeviceProcAddr(d, "vkGetQueryPoolResults"));

  d->vkCreateBuffer              = reinterpret_cast<PFN_vkCreateBuffer>             (d->vkGetDeviceProcAddr(d, "vkCreateBuffer"));
  d->vkDestroyBuffer             = reinterpret_cast<PFN_vkDestroyBuffer>            (d->vkGetDeviceProcAddr(d, "vkDestroyBuffer"));
  d->vkCreateBufferView          = reinterpret_cast<PFN_vkCreateBufferView>         (d->vkGetDeviceProcAddr(d, "vkCreateBufferView"));
  d->vkDestroyBufferView         = reinterpret_cast<PFN_vkDestroyBufferView>        (d->vkGetDeviceProcAddr(d, "vkDestroyBufferView"));
  d->vkCreateImage               = reinterpret_cast<PFN_vkCreateImage>              (d->vkGetDeviceProcAddr(d, "vkCreateImage"));
  d->vkDestroyImage              = reinterpret_cast<PFN_vkDestroyImage>             (d->vkGetDeviceProcAddr(d, "vkDestroyImage"));
  d->vkGetImageSubresourceLayout = reinterpret_cast<PFN_vkGetImageSubresourceLayout>(d->vkGetDeviceProcAddr(d, "vkGetImageSubresourceLayout"));
  d->vkCreateImageView           = reinterpret_cast<PFN_vkCreateImageView>          (d->vkGetDeviceProcAddr(d, "vkCreateImageView"));
  d->vkDestroyImageView          = reinterpret_cast<PFN_vkDestroyImageView>         (d->vkGetDeviceProcAddr(d, "vkDestroyImageView"));
  d->vkCreateShaderModule        = reinterpret_cast<PFN_vkCreateShaderModule>       (d->vkGetDeviceProcAddr(d, "vkCreateShaderModule"));
  d->vkDestroyShaderModule       = reinterpret_cast<PFN_vkDestroyShaderModule>      (d->vkGetDeviceProcAddr(d, "vkDestroyShaderModule"));

  d->vkCreatePipelineCache     = reinterpret_cast<PFN_vkCreatePipelineCache>    (d->vkGetDeviceProcAddr(d, "vkCreatePipelineCache"));
  d->vkDestroyPipelineCache    = reinterpret_cast<PFN_vkDestroyPipelineCache>   (d->vkGetDeviceProcAddr(d, "vkDestroyPipelineCache"));
  d->vkGetPipelineCacheData    = reinterpret_cast<PFN_vkGetPipelineCacheData>   (d->vkGetDeviceProcAddr(d, "vkGetPipelineCacheData"));
  d->vkMergePipelineCaches     = reinterpret_cast<PFN_vkMergePipelineCaches>    (d->vkGetDeviceProcAddr(d, "vkMergePipelineCaches"));
  d->vkCreateGraphicsPipelines = reinterpret_cast<PFN_vkCreateGraphicsPipelines>(d->vkGetDeviceProcAddr(d, "vkCreateGraphicsPipelines"));
  d->vkCreateComputePipelines  = reinterpret_cast<PFN_vkCreateComputePipelines> (d->vkGetDeviceProcAddr(d, "vkCreateComputePipelines"));
  d->vkDestroyPipeline         = reinterpret_cast<PFN_vkDestroyPipeline>        (d->vkGetDeviceProcAddr(d, "vkDestroyPipeline"));
  d->vkCreatePipelineLayout    = reinterpret_cast<PFN_vkCreatePipelineLayout>   (d->vkGetDeviceProcAddr(d, "vkCreatePipelineLayout"));
  d->vkDestroyPipelineLayout   = reinterpret_cast<PFN_vkDestroyPipelineLayout>  (d->vkGetDeviceProcAddr(d, "vkDestroyPipelineLayout"));

  d->vkCreateSampler              = reinterpret_cast<PFN_vkCreateSampler>             (d->vkGetDeviceProcAddr(d, "vkCreateSampler"));
  d->vkDestroySampler             = reinterpret_cast<PFN_vkDestroySampler>            (d->vkGetDeviceProcAddr(d, "vkDestroySampler"));
  d->vkCreateDescriptorSetLayout  = reinterpret_cast<PFN_vkCreateDescriptorSetLayout> (d->vkGetDeviceProcAddr(d, "vkCreateDescriptorSetLayout"));
  d->vkDestroyDescriptorSetLayout = reinterpret_cast<PFN_vkDestroyDescriptorSetLayout>(d->vkGetDeviceProcAddr(d, "vkDestroyDescriptorSetLayout"));
  d->vkCreateDescriptorPool       = reinterpret_cast<PFN_vkCreateDescriptorPool>      (d->vkGetDeviceProcAddr(d, "vkCreateDescriptorPool"));
  d->vkDestroyDescriptorPool      = reinterpret_cast<PFN_vkDestroyDescriptorPool>     (d->vkGetDeviceProcAddr(d, "vkDestroyDescriptorPool"));
  d->vkResetDescriptorPool        = reinterpret_cast<PFN_vkResetDescriptorPool>       (d->vkGetDeviceProcAddr(d, "vkResetDescriptorPool"));
  d->vkAllocateDescriptorSets     = reinterpret_cast<PFN_vkAllocateDescriptorSets>    (d->vkGetDeviceProcAddr(d, "vkAllocateDescriptorSets"));
  d->vkFreeDescriptorSets         = reinterpret_cast<PFN_vkFreeDescriptorSets>        (d->vkGetDeviceProcAddr(d, "vkFreeDescriptorSets"));
  d->vkUpdateDescriptorSets       = reinterpret_cast<PFN_vkUpdateDescriptorSets>      (d->vkGetDeviceProcAddr(d, "vkUpdateDescriptorSets"));

  d->vkCreateFramebuffer        = reinterpret_cast<PFN_vkCreateFramebuffer>       (d->vkGetDeviceProcAddr(d, "vkCreateFramebuffer"));
  d->vkDestroyFramebuffer       = reinterpret_cast<PFN_vkDestroyFramebuffer>      (d->vkGetDeviceProcAddr(d, "vkDestroyFramebuffer"));
  d->vkCreateRenderPass         = reinterpret_cast<PFN_vkCreateRenderPass>        (d->vkGetDeviceProcAddr(d, "vkCreateRenderPass"));
  d->vkDestroyRenderPass        = reinterpret_cast<PFN_vkDestroyRenderPass>       (d->vkGetDeviceProcAddr(d, "vkDestroyRenderPass"));
  d->vkGetRenderAreaGranularity = reinterpret_cast<PFN_vkGetRenderAreaGranularity>(d->vkGetDeviceProcAddr(d, "vkGetRenderAreaGranularity"));

  d->vkCreateCommandPool      = reinterpret_cast<PFN_vkCreateCommandPool>     (d->vkGetDeviceProcAddr(d, "vkCreateCommandPool"));
  d->vkDestroyCommandPool     = reinterpret_cast<PFN_vkDestroyCommandPool>    (d->vkGetDeviceProcAddr(d, "vkDestroyCommandPool"));
  d->vkResetCommandPool       = reinterpret_cast<PFN_vkResetCommandPool>      (d->vkGetDeviceProcAddr(d, "vkResetCommandPool"));
  d->vkAllocateCommandBuffers = reinterpret_cast<PFN_vkAllocateCommandBuffers>(d->vkGetDeviceProcAddr(d, "vkAllocateCommandBuffers"));
  d->vkFreeCommandBuffers     = reinterpret_cast<PFN_vkFreeCommandBuffers>    (d->vkGetDeviceProcAddr(d, "vkFreeCommandBuffers"));
  d->vkBeginCommandBuffer     = reinterpret_cast<PFN_vkBeginCommandBuffer>    (d->vkGetDeviceProcAddr(d, "vkBeginCommandBuffer"));
  d->vkEndCommandBuffer       = reinterpret_cast<PFN_vkEndCommandBuffer>      (d->vkGetDeviceProcAddr(d, "vkEndCommandBuffer"));
  d->vkResetCommandBuffer     = reinterpret_cast<PFN_vkResetCommandBuffer>    (d->vkGetDeviceProcAddr(d, "vkResetCommandBuffer"));

  // VK_cmd
  d->vkCmdBindPipeline           = reinterpret_cast<PFN_vkCmdBindPipeline>          (d->vkGetDeviceProcAddr(d, "vkCmdBindPipeline"));
  d->vkCmdSetViewport            = reinterpret_cast<PFN_vkCmdSetViewport>           (d->vkGetDeviceProcAddr(d, "vkCmdSetViewport"));
  d->vkCmdSetScissor             = reinterpret_cast<PFN_vkCmdSetScissor>            (d->vkGetDeviceProcAddr(d, "vkCmdSetScissor"));
  d->vkCmdSetLineWidth           = reinterpret_cast<PFN_vkCmdSetLineWidth>          (d->vkGetDeviceProcAddr(d, "vkCmdSetLineWidth"));
  d->vkCmdSetDepthBias           = reinterpret_cast<PFN_vkCmdSetDepthBias>          (d->vkGetDeviceProcAddr(d, "vkCmdSetDepthBias"));
  d->vkCmdSetBlendConstants      = reinterpret_cast<PFN_vkCmdSetBlendConstants>     (d->vkGetDeviceProcAddr(d, "vkCmdSetBlendConstants"));
  d->vkCmdSetDepthBounds         = reinterpret_cast<PFN_vkCmdSetDepthBounds>        (d->vkGetDeviceProcAddr(d, "vkCmdSetDepthBounds"));
  d->vkCmdSetStencilCompareMask  = reinterpret_cast<PFN_vkCmdSetStencilCompareMask> (d->vkGetDeviceProcAddr(d, "vkCmdSetStencilCompareMask"));
  d->vkCmdSetStencilWriteMask    = reinterpret_cast<PFN_vkCmdSetStencilWriteMask>   (d->vkGetDeviceProcAddr(d, "vkCmdSetStencilWriteMask"));
  d->vkCmdSetStencilReference    = reinterpret_cast<PFN_vkCmdSetStencilReference>   (d->vkGetDeviceProcAddr(d, "vkCmdSetStencilReference"));
  d->vkCmdBindDescriptorSets     = reinterpret_cast<PFN_vkCmdBindDescriptorSets>    (d->vkGetDeviceProcAddr(d, "vkCmdBindDescriptorSets"));
  d->vkCmdBindIndexBuffer        = reinterpret_cast<PFN_vkCmdBindIndexBuffer>       (d->vkGetDeviceProcAddr(d, "vkCmdBindIndexBuffer"));
  d->vkCmdBindVertexBuffers      = reinterpret_cast<PFN_vkCmdBindVertexBuffers>     (d->vkGetDeviceProcAddr(d, "vkCmdBindVertexBuffers"));
  d->vkCmdDraw                   = reinterpret_cast<PFN_vkCmdDraw>                  (d->vkGetDeviceProcAddr(d, "vkCmdDraw"));
  d->vkCmdDrawIndexed            = reinterpret_cast<PFN_vkCmdDrawIndexed>           (d->vkGetDeviceProcAddr(d, "vkCmdDrawIndexed"));
  d->vkCmdDrawIndirect           = reinterpret_cast<PFN_vkCmdDrawIndirect>          (d->vkGetDeviceProcAddr(d, "vkCmdDrawIndirect"));
  d->vkCmdDrawIndexedIndirect    = reinterpret_cast<PFN_vkCmdDrawIndexedIndirect>   (d->vkGetDeviceProcAddr(d, "vkCmdDrawIndexedIndirect"));
  d->vkCmdDispatch               = reinterpret_cast<PFN_vkCmdDispatch>              (d->vkGetDeviceProcAddr(d, "vkCmdDispatch"));
  d->vkCmdDispatchIndirect       = reinterpret_cast<PFN_vkCmdDispatchIndirect>      (d->vkGetDeviceProcAddr(d, "vkCmdDispatchIndirect"));
  d->vkCmdCopyBuffer             = reinterpret_cast<PFN_vkCmdCopyBuffer>            (d->vkGetDeviceProcAddr(d, "vkCmdCopyBuffer"));
  d->vkCmdCopyImage              = reinterpret_cast<PFN_vkCmdCopyImage>             (d->vkGetDeviceProcAddr(d, "vkCmdCopyImage"));
  d->vkCmdBlitImage              = reinterpret_cast<PFN_vkCmdBlitImage>             (d->vkGetDeviceProcAddr(d, "vkCmdBlitImage"));
  d->vkCmdCopyBufferToImage      = reinterpret_cast<PFN_vkCmdCopyBufferToImage>     (d->vkGetDeviceProcAddr(d, "vkCmdCopyBufferToImage"));
  d->vkCmdCopyImageToBuffer      = reinterpret_cast<PFN_vkCmdCopyImageToBuffer>     (d->vkGetDeviceProcAddr(d, "vkCmdCopyImageToBuffer"));
  d->vkCmdUpdateBuffer           = reinterpret_cast<PFN_vkCmdUpdateBuffer>          (d->vkGetDeviceProcAddr(d, "vkCmdUpdateBuffer"));
  d->vkCmdFillBuffer             = reinterpret_cast<PFN_vkCmdFillBuffer>            (d->vkGetDeviceProcAddr(d, "vkCmdFillBuffer"));
  d->vkCmdClearColorImage        = reinterpret_cast<PFN_vkCmdClearColorImage>       (d->vkGetDeviceProcAddr(d, "vkCmdClearColorImage"));
  d->vkCmdClearDepthStencilImage = reinterpret_cast<PFN_vkCmdClearDepthStencilImage>(d->vkGetDeviceProcAddr(d, "vkCmdClearDepthStencilImage"));
  d->vkCmdClearAttachments       = reinterpret_cast<PFN_vkCmdClearAttachments>      (d->vkGetDeviceProcAddr(d, "vkCmdClearAttachments"));
  d->vkCmdResolveImage           = reinterpret_cast<PFN_vkCmdResolveImage>          (d->vkGetDeviceProcAddr(d, "vkCmdResolveImage"));
  d->vkCmdSetEvent               = reinterpret_cast<PFN_vkCmdSetEvent>              (d->vkGetDeviceProcAddr(d, "vkCmdSetEvent"));
  d->vkCmdResetEvent             = reinterpret_cast<PFN_vkCmdResetEvent>            (d->vkGetDeviceProcAddr(d, "vkCmdResetEvent"));
  d->vkCmdWaitEvents             = reinterpret_cast<PFN_vkCmdWaitEvents>            (d->vkGetDeviceProcAddr(d, "vkCmdWaitEvents"));
  d->vkCmdPipelineBarrier        = reinterpret_cast<PFN_vkCmdPipelineBarrier>       (d->vkGetDeviceProcAddr(d, "vkCmdPipelineBarrier"));
  d->vkCmdBeginQuery             = reinterpret_cast<PFN_vkCmdBeginQuery>            (d->vkGetDeviceProcAddr(d, "vkCmdBeginQuery"));
  d->vkCmdEndQuery               = reinterpret_cast<PFN_vkCmdEndQuery>              (d->vkGetDeviceProcAddr(d, "vkCmdEndQuery"));
  d->vkCmdResetQueryPool         = reinterpret_cast<PFN_vkCmdResetQueryPool>        (d->vkGetDeviceProcAddr(d, "vkCmdResetQueryPool"));
  d->vkCmdWriteTimestamp         = reinterpret_cast<PFN_vkCmdWriteTimestamp>        (d->vkGetDeviceProcAddr(d, "vkCmdWriteTimestamp"));
  d->vkCmdCopyQueryPoolResults   = reinterpret_cast<PFN_vkCmdCopyQueryPoolResults>  (d->vkGetDeviceProcAddr(d, "vkCmdCopyQueryPoolResults"));
  d->vkCmdPushConstants          = reinterpret_cast<PFN_vkCmdPushConstants>         (d->vkGetDeviceProcAddr(d, "vkCmdPushConstants"));
  d->vkCmdBeginRenderPass        = reinterpret_cast<PFN_vkCmdBeginRenderPass>       (d->vkGetDeviceProcAddr(d, "vkCmdBeginRenderPass"));
  d->vkCmdNextSubpass            = reinterpret_cast<PFN_vkCmdNextSubpass>           (d->vkGetDeviceProcAddr(d, "vkCmdNextSubpass"));
  d->vkCmdEndRenderPass          = reinterpret_cast<PFN_vkCmdEndRenderPass>         (d->vkGetDeviceProcAddr(d, "vkCmdEndRenderPass"));
  d->vkCmdExecuteCommands        = reinterpret_cast<PFN_vkCmdExecuteCommands>       (d->vkGetDeviceProcAddr(d, "vkCmdExecuteCommands"));

  // VK_KHR_swapchain
  d->vkCreateSwapchainKHR                    = reinterpret_cast<PFN_vkCreateSwapchainKHR>                   (d->vkGetDeviceProcAddr(d, "vkCreateSwapchainKHR"));
  d->vkDestroySwapchainKHR                   = reinterpret_cast<PFN_vkDestroySwapchainKHR>                  (d->vkGetDeviceProcAddr(d, "vkDestroySwapchainKHR"));
  d->vkGetSwapchainImagesKHR                 = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>                (d->vkGetDeviceProcAddr(d, "vkGetSwapchainImagesKHR"));
  d->vkAcquireNextImageKHR                   = reinterpret_cast<PFN_vkAcquireNextImageKHR>                  (d->vkGetDeviceProcAddr(d, "vkAcquireNextImageKHR"));
  d->vkQueuePresentKHR                       = reinterpret_cast<PFN_vkQueuePresentKHR>                      (d->vkGetDeviceProcAddr(d, "vkQueuePresentKHR"));
  d->vkGetDeviceGroupPresentCapabilitiesKHR  = reinterpret_cast<PFN_vkGetDeviceGroupPresentCapabilitiesKHR> (d->vkGetDeviceProcAddr(d, "vkGetDeviceGroupPresentCapabilitiesKHR"));
  d->vkGetDeviceGroupSurfacePresentModesKHR  = reinterpret_cast<PFN_vkGetDeviceGroupSurfacePresentModesKHR> (d->vkGetDeviceProcAddr(d, "vkGetDeviceGroupSurfacePresentModesKHR"));
  d->vkGetPhysicalDevicePresentRectanglesKHR = reinterpret_cast<PFN_vkGetPhysicalDevicePresentRectanglesKHR>(d->vkGetDeviceProcAddr(d, "vkGetPhysicalDevicePresentRectanglesKHR"));
  d->vkAcquireNextImage2KHR                  = reinterpret_cast<PFN_vkAcquireNextImage2KHR>                 (d->vkGetDeviceProcAddr(d, "vkAcquireNextImage2KHR"));
  // VK_KHR_display_swapchain
  d->vkCreateSharedSwapchainsKHR = reinterpret_cast<PFN_vkCreateSharedSwapchainsKHR>(d->vkGetDeviceProcAddr(d, "vkCreateSharedSwapchainsKHR"));
  // VK_KHR_device_group
  d->vkGetDeviceGroupPeerMemoryFeaturesKHR = reinterpret_cast<PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR>(d->vkGetDeviceProcAddr(d, "vkGetDeviceGroupPeerMemoryFeaturesKHR"));
  d->vkCmdSetDeviceMaskKHR                 = reinterpret_cast<PFN_vkCmdSetDeviceMaskKHR>                (d->vkGetDeviceProcAddr(d, "vkCmdSetDeviceMaskKHR"));
  d->vkCmdDispatchBaseKHR                  = reinterpret_cast<PFN_vkCmdDispatchBaseKHR>                 (d->vkGetDeviceProcAddr(d, "vkCmdDispatchBaseKHR"));
  // VK_KHR_maintenance1
  d->vkTrimCommandPoolKHR = reinterpret_cast<PFN_vkTrimCommandPoolKHR>(d->vkGetDeviceProcAddr(d, "vkTrimCommandPoolKHR"));
  // VK_KHR_external_memory_fd
  d->vkGetMemoryFdKHR           = reinterpret_cast<PFN_vkGetMemoryFdKHR>          (d->vkGetDeviceProcAddr(d, "vkGetMemoryFdKHR"));
  d->vkGetMemoryFdPropertiesKHR = reinterpret_cast<PFN_vkGetMemoryFdPropertiesKHR>(d->vkGetDeviceProcAddr(d, "vkGetMemoryFdPropertiesKHR"));
  // VK_KHR_external_semaphore_fd
  d->vkImportSemaphoreFdKHR = reinterpret_cast<PFN_vkImportSemaphoreFdKHR>(d->vkGetDeviceProcAddr(d, "vkImportSemaphoreFdKHR"));
  d->vkGetSemaphoreFdKHR    = reinterpret_cast<PFN_vkGetSemaphoreFdKHR>   (d->vkGetDeviceProcAddr(d, "vkGetSemaphoreFdKHR"));
  // VK_KHR_push_descriptor
  d->vkCmdPushDescriptorSetKHR             = reinterpret_cast<PFN_vkCmdPushDescriptorSetKHR>            (d->vkGetDeviceProcAddr(d, "vkCmdPushDescriptorSetKHR"));
  d->vkCmdPushDescriptorSetWithTemplateKHR = reinterpret_cast<PFN_vkCmdPushDescriptorSetWithTemplateKHR>(d->vkGetDeviceProcAddr(d, "vkCmdPushDescriptorSetWithTemplateKHR"));
  // VK_KHR_descriptor_update_template
  d->vkCreateDescriptorUpdateTemplateKHR  = reinterpret_cast<PFN_vkCreateDescriptorUpdateTemplateKHR> (d->vkGetDeviceProcAddr(d, "vkCreateDescriptorUpdateTemplateKHR"));
  d->vkDestroyDescriptorUpdateTemplateKHR = reinterpret_cast<PFN_vkDestroyDescriptorUpdateTemplateKHR>(d->vkGetDeviceProcAddr(d, "vkDestroyDescriptorUpdateTemplateKHR"));
  d->vkUpdateDescriptorSetWithTemplateKHR = reinterpret_cast<PFN_vkUpdateDescriptorSetWithTemplateKHR>(d->vkGetDeviceProcAddr(d, "vkUpdateDescriptorSetWithTemplateKHR"));
  // VK_KHR_shared_presentable_image
  d->vkGetSwapchainStatusKHR = reinterpret_cast<PFN_vkGetSwapchainStatusKHR>(d->vkGetDeviceProcAddr(d, "vkGetSwapchainStatusKHR"));
  // VK_KHR_external_fence_fd
  d->vkImportFenceFdKHR = reinterpret_cast<PFN_vkImportFenceFdKHR>(d->vkGetDeviceProcAddr(d, "vkImportFenceFdKHR"));
  d->vkGetFenceFdKHR    = reinterpret_cast<PFN_vkGetFenceFdKHR>   (d->vkGetDeviceProcAddr(d, "vkGetFenceFdKHR"));
  // VK_KHR_get_memory_requirements2
  d->vkGetImageMemoryRequirements2KHR       = reinterpret_cast<PFN_vkGetImageMemoryRequirements2KHR>      (d->vkGetDeviceProcAddr(d, "vkGetImageMemoryRequirements2KHR"));
  d->vkGetBufferMemoryRequirements2KHR      = reinterpret_cast<PFN_vkGetBufferMemoryRequirements2KHR>     (d->vkGetDeviceProcAddr(d, "vkGetBufferMemoryRequirements2KHR"));
  d->vkGetImageSparseMemoryRequirements2KHR = reinterpret_cast<PFN_vkGetImageSparseMemoryRequirements2KHR>(d->vkGetDeviceProcAddr(d, "vkGetImageSparseMemoryRequirements2KHR"));
  // VK_KHR_sampler_ycbcr_conversion
  d->vkCreateSamplerYcbcrConversionKHR  = reinterpret_cast<PFN_vkCreateSamplerYcbcrConversionKHR> (d->vkGetDeviceProcAddr(d, "vkCreateSamplerYcbcrConversionKHR"));
  d->vkDestroySamplerYcbcrConversionKHR = reinterpret_cast<PFN_vkDestroySamplerYcbcrConversionKHR>(d->vkGetDeviceProcAddr(d, "vkDestroySamplerYcbcrConversionKHR"));
  // VK_KHR_bind_memory2
  d->vkBindBufferMemory2KHR = reinterpret_cast<PFN_vkBindBufferMemory2KHR>(d->vkGetDeviceProcAddr(d, "vkBindBufferMemory2KHR"));
  d->vkBindImageMemory2KHR  = reinterpret_cast<PFN_vkBindImageMemory2KHR> (d->vkGetDeviceProcAddr(d, "vkBindImageMemory2KHR"));
  // VK_KHR_maintenance3
  d->vkGetDescriptorSetLayoutSupportKHR = reinterpret_cast<PFN_vkGetDescriptorSetLayoutSupportKHR>(d->vkGetDeviceProcAddr(d, "vkGetDescriptorSetLayoutSupportKHR"));
  // VK_EXT_debug_marker
  d->vkDebugMarkerSetObjectTagEXT  = reinterpret_cast<PFN_vkDebugMarkerSetObjectTagEXT> (d->vkGetDeviceProcAddr(d, "vkDebugMarkerSetObjectTagEXT"));
  d->vkDebugMarkerSetObjectNameEXT = reinterpret_cast<PFN_vkDebugMarkerSetObjectNameEXT>(d->vkGetDeviceProcAddr(d, "vkDebugMarkerSetObjectNameEXT"));
  d->vkCmdDebugMarkerBeginEXT      = reinterpret_cast<PFN_vkCmdDebugMarkerBeginEXT>     (d->vkGetDeviceProcAddr(d, "vkCmdDebugMarkerBeginEXT"));
  d->vkCmdDebugMarkerEndEXT        = reinterpret_cast<PFN_vkCmdDebugMarkerEndEXT>       (d->vkGetDeviceProcAddr(d, "vkCmdDebugMarkerEndEXT"));
  d->vkCmdDebugMarkerInsertEXT     = reinterpret_cast<PFN_vkCmdDebugMarkerInsertEXT>    (d->vkGetDeviceProcAddr(d, "vkCmdDebugMarkerInsertEXT"));
  // VK_EXT_display_control
  d->vkDisplayPowerControlEXT  = reinterpret_cast<PFN_vkDisplayPowerControlEXT> (d->vkGetDeviceProcAddr(d, "vkDisplayPowerControlEXT"));
  d->vkRegisterDeviceEventEXT  = reinterpret_cast<PFN_vkRegisterDeviceEventEXT> (d->vkGetDeviceProcAddr(d, "vkRegisterDeviceEventEXT"));
  d->vkRegisterDisplayEventEXT = reinterpret_cast<PFN_vkRegisterDisplayEventEXT>(d->vkGetDeviceProcAddr(d, "vkRegisterDisplayEventEXT"));
  d->vkGetSwapchainCounterEXT  = reinterpret_cast<PFN_vkGetSwapchainCounterEXT> (d->vkGetDeviceProcAddr(d, "vkGetSwapchainCounterEXT"));
  // VK_EXT_discard_rectangles
  d->vkCmdSetDiscardRectangleEXT = reinterpret_cast<PFN_vkCmdSetDiscardRectangleEXT>(d->vkGetDeviceProcAddr(d, "vkCmdSetDiscardRectangleEXT"));
  // VK_EXT_hdr_metadata
  d->vkSetHdrMetadata = reinterpret_cast<PFN_vkSetHdrMetadataEXT>(d->vkGetDeviceProcAddr(d, "vkSetHdrMetadataEXT"));
  // VK_EXT_sample_locations
  d->vkCmdSetSampleLocationsEXT                  = reinterpret_cast<PFN_vkCmdSetSampleLocationsEXT>                 (d->vkGetDeviceProcAddr(d, "vkCmdSetSampleLocationsEXT"));
  d->vkGetPhysicalDeviceMultisamplePropertiesEXT = reinterpret_cast<PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT>(d->vkGetDeviceProcAddr(d, "vkGetPhysicalDeviceMultisamplePropertiesEXT"));
  // VK_EXT_validation_cache
  d->vkCreateValidationCacheEXT  = reinterpret_cast<PFN_vkCreateValidationCacheEXT> (d->vkGetDeviceProcAddr(d, "vkCreateValidationCacheEXT"));
  d->vkDestroyValidationCacheEXT = reinterpret_cast<PFN_vkDestroyValidationCacheEXT>(d->vkGetDeviceProcAddr(d, "vkDestroyValidationCacheEXT"));
  d->vkMergeValidationCachesEXT  = reinterpret_cast<PFN_vkMergeValidationCachesEXT> (d->vkGetDeviceProcAddr(d, "vkMergeValidationCachesEXT"));
  d->vkGetValidationCacheDataEXT = reinterpret_cast<PFN_vkGetValidationCacheDataEXT>(d->vkGetDeviceProcAddr(d, "vkGetValidationCacheDataEXT"));
  // VK_EXT_external_memory_host
  d->vkGetMemoryHostPointerPropertiesEXT = reinterpret_cast<PFN_vkGetMemoryHostPointerPropertiesEXT>(d->vkGetDeviceProcAddr(d, "vkGetMemoryHostPointerPropertiesEXT"));
}

std::string vulkan::to_string(VkResult res) {
  switch (res) {
    case VK_NOT_READY:   return "A fence or query has not yet completed";
    case VK_TIMEOUT:     return "A wait operation has not completed in the specified time";
    case VK_EVENT_SET:   return "An event is signaled";
    case VK_EVENT_RESET: return "An event is unsignaled";
    case VK_INCOMPLETE:  return "A return array was too small for the result";

    case VK_ERROR_OUT_OF_HOST_MEMORY:    return "A host memory allocation has failed";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:  return "A device memory allocation has failed";
    case VK_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";
    case VK_ERROR_DEVICE_LOST:           return "The logical or physical device has been lost";
    case VK_ERROR_MEMORY_MAP_FAILED:     return "Mapping of the memory object has failed";
    case VK_ERROR_LAYER_NOT_PRESENT:     return "A requested layer is not present or could not be loaded";
    case VK_ERROR_EXTENSION_NOT_PRESENT: return "A requested extension is not supported";
    case VK_ERROR_FEATURE_NOT_PRESENT:   return "A requested feature is not supported";
    case VK_ERROR_INCOMPATIBLE_DRIVER:   return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible for implementation-specific reasons";
    case VK_ERROR_TOO_MANY_OBJECTS:      return "Too many objects of the type have already been created";
    case VK_ERROR_FORMAT_NOT_SUPPORTED:  return "A requested format is not supported on this device";
    case VK_ERROR_FRAGMENTED_POOL:       return "A requested pool allocation has failed due to fragmentation of the pool's memory";

    case VK_SUCCESS: return "Success";
    default:         return "Unhandled Vulkan result"; }
}

library vulkan::vulkan_;

// VK_core
PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr = nullptr;
PFN_vkCreateInstance                       vkCreateInstance = nullptr;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = nullptr;
PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties = nullptr;

} // namespace vk
} // namespace ig
