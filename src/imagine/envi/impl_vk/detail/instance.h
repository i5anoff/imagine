/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_VK_INSTANCE_H
#define IG_ENVI_VK_INSTANCE_H

#include "imagine/envi/impl_vk/detail/wrapper.h"

#include <vector>

namespace ig {
namespace vk {

enum class hardware;
enum class memory_property : uint32_t; using memory_properties = flags<memory_property>;
enum capability : uint32_t;
using capabilities = flags<capability>;

class iPfn;
class physical;
class IG_API instance : public managed<VkInstance_T*> {
public:
  friend vulkan;
  friend device;

  explicit instance(bool validation = false);
  virtual ~instance();

  bool dbg();
  bool is_supported(const std::string& name) const;

  auto& operator->() const { return ipfn_; }

private:
  void impl_get();
  void physicals_get();

protected:
  bool validation_;
  std::vector
  < std::shared_ptr<physical>
  > physicals_;

  struct impl;
  std::unique_ptr<impl> impl_;
  std::unique_ptr<iPfn> ipfn_;
};

enum class hardware {
  other = 0,
  integrated = 1, discrete = 2, virtualized = 3, cpu = 4 };

enum class memory_property : uint32_t {
  device_local = 0x001,
  host_visible = 0x002, host_coherent = 0x004, host_cached = 0x008, lazily_allocated = 0x010 };

enum class format {
  r8_unorm       = 9,  r8_snorm       = 10, r8_srgb       = 15,
  r8g8_unorm     = 16, r8g8_snorm     = 17, r8g8_srgb     = 22,
  r8g8b8_unorm   = 23, r8g8b8_snorm   = 24, r8g8b8_srgb   = 29,
  b8g8r8_unorm   = 30, b8g8r8_snorm   = 31, b8g8r8_srgb   = 36,
  r8g8b8a8_unorm = 37, r8g8b8a8_snorm = 38, r8g8b8a8_srgb = 43,
  b8g8r8a8_unorm = 44, b8g8r8a8_snorm = 45, b8g8r8a8_srgb = 50,

  r16_unorm          = 70, r16_snorm          = 71, r16_sfloat          = 76,
  r16g16_unorm       = 77, r16g16_snorm       = 78, r16g16_sfloat       = 83,
  r16g16b16_unorm    = 84, r16g16b16_snorm    = 85, r16g16b16_sfloat    = 90,
  r16g16b16a16_unorm = 91, r16g16b16a16_snorm = 92, r16g16b16a16_sfloat = 97,

  r32_sfloat = 100, r32g32_sfloat = 103, r32g32b32_sfloat = 106, r32g32b32a32_sfloat = 109,
  r64_sfloat = 112, r64g64_sfloat = 115, r64g64b64_sfloat = 118, r64g64b64a64_sfloat = 121,

  d16_unorm  = 124,
  d32_sfloat = 126, d16_unorm_s8_uint = 128, d24_unorm_s8_uint = 129, d32_sfloat_s8_uint = 130 };

enum capability : uint32_t {
  graphics = 0x001, compute = 0x002,
  transfer = 0x004, sparse  = 0x008, present = 0x010, universal = graphics | compute | transfer };

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_INSTANCE_H
