/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_VK_PHYSICAL_H
#define IG_ENVI_VK_PHYSICAL_H

#include "imagine/envi/impl_vk/detail/wrapper.h"
#include "imagine/envi/impl_vk/detail/instance.h"

namespace ig {
namespace vk {

class IG_API physical : public managed<VkPhysicalDevice_T*> {
public:
  explicit physical(const instance& instance, VkPhysicalDevice_T* physical);
  virtual ~physical();

  int32_t queue(capabilities caps) const;
  int32_t heap(uint32_t type, memory_properties properties) const;

  // properties
  uint32_t get_api_version() const;
  uint32_t get_driver_version() const;
  uint32_t get_id() const;
  std::string get_name() const;

  hardware get_type() const;
  // limits
  size_t get_ubo_alignment() const;
  size_t get_ssbo_alignment() const;
  // vulkan
  auto get_features() const -> device_features;

  const instance& inst;

protected:
  void impl_get();

private:
  struct impl;
  std::unique_ptr<impl> impl_;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_PHYSICAL_H
