/*
 Imagine v0.1
 [bridge]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/simulation/world/data/signal/image/jpeg.h"
#include "imagine/core/log.h"

#include <cstring>

extern "C"
{
#include "jpeg/jpeglib.h"
#include "jpeg/jerror.h"
}

namespace ig     {
namespace detail {

constexpr uint32_t buffer_size = 4096;
struct jpeg_src {
  jpeg_source_mgr
  jpeg;
  uint8_t* buffer; std::istream* stream; };

struct jpeg_dst {
  jpeg_destination_mgr
  jpeg;
  uint8_t* buffer; std::ostream* stream; };

boolean jpeg_readproc(j_decompress_ptr jpeg_ptr);
boolean jpeg_writeproc(j_compress_ptr jpeg_ptr);
void jpeg_message(j_common_ptr jpeg_ptr);
void jpeg_exit(j_common_ptr jpeg_ptr);

// Jpeg interface implementation - validate - read - write
bool jpeg_validate(std::istream& stream) {
  uint8_t jpeg_sig[] = {0xff, 0xd8};
  uint8_t sig[2]     = {0, 0};

  stream.read(reinterpret_cast<char*>(sig), sizeof(jpeg_sig));
  stream.seekg(0, std::ios::beg);
  return memcmp(jpeg_sig, sig, sizeof(jpeg_sig)) == 0;
}

jpeg jpeg_readp_impl(std::istream& stream, const image_bridge::parameters&) {
  jpeg_decompress_struct
    jpeg_ptr{};
  jpeg_error_mgr
    jerr{};

  jpeg_ptr.err = jpeg_std_error(&jerr);
  jerr.error_exit = jpeg_exit;
  jerr.output_message = jpeg_message;

  jpeg_create_decompress(&jpeg_ptr);

  if (!jpeg_ptr.src) {
    jpeg_ptr.src = reinterpret_cast<jpeg_source_mgr*>((*jpeg_ptr.mem->alloc_small)
                  (reinterpret_cast<j_common_ptr>(&jpeg_ptr), JPOOL_PERMANENT, sizeof(jpeg_src)));

    auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr.src);
    src->buffer = reinterpret_cast<uint8_t*>((*jpeg_ptr.mem->alloc_small)
                 (reinterpret_cast<j_common_ptr>(&jpeg_ptr), JPOOL_PERMANENT, buffer_size * sizeof(uint8_t)));
  }

  auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr.src);
  src->stream = &stream;
  src->jpeg.fill_input_buffer = jpeg_readproc;
  src->jpeg.skip_input_data = [](j_decompress_ptr jpeg_ptr, long bytes) {
    auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr->src);
    if (bytes > 0) {
      while (bytes > static_cast<long>(src->jpeg.bytes_in_buffer)) {
        bytes -= static_cast<long>(src->jpeg.bytes_in_buffer);
        jpeg_readproc(jpeg_ptr);
      }
      src->jpeg.next_input_byte += bytes;
      src->jpeg.bytes_in_buffer -= bytes;
    }
  };

  src->jpeg.init_source = [](j_decompress_ptr) {};
  src->jpeg.term_source = [](j_decompress_ptr) {};

  src->jpeg.resync_to_restart = jpeg_resync_to_restart;
  src->jpeg.bytes_in_buffer = 0;
  src->jpeg.next_input_byte = nullptr;

  jpeg_read_header(&jpeg_ptr, TRUE);
  jpeg_start_decompress(&jpeg_ptr);

  auto channels = static_cast<uint32_t>(jpeg_ptr.output_components);
  auto width    = jpeg_ptr.output_width,
       height   = jpeg_ptr.output_height;

  auto image =
  std::make_unique<jpeg_t>(std::initializer_list<size_t>{channels, width, height});

  while (jpeg_ptr.output_scanline < height) {
    auto r = image->buffer() + (width * channels * jpeg_ptr.output_scanline);
    jpeg_read_scanlines(
      &jpeg_ptr,
      (JSAMPARRAY)&r,
      1);
  }

  jpeg_finish_decompress(&jpeg_ptr);
  jpeg_destroy_decompress(&jpeg_ptr);
  return std::make_pair(true, std::move(image));
}

bool jpeg_write_impl(std::ostream& stream, const image_bridge::parameters&, const image_bridge::resource& image) {
  jpeg_compress_struct
    jpeg_ptr{};
  jpeg_error_mgr
    jerr{};

  jpeg_ptr.err = jpeg_std_error(&jerr);
  jerr.error_exit = jpeg_exit;
  jerr.output_message = jpeg_message;

  jpeg_create_compress(&jpeg_ptr);

  if (!jpeg_ptr.dest) {
    jpeg_ptr.dest = reinterpret_cast<jpeg_destination_mgr*>((*jpeg_ptr.mem->alloc_small)
                   (reinterpret_cast<j_common_ptr>(&jpeg_ptr), JPOOL_PERMANENT, sizeof(jpeg_dst)));
  }

  auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr.dest);
  dst->stream = &stream;
  dst->jpeg.empty_output_buffer = &jpeg_writeproc;
  dst->jpeg.init_destination = [](j_compress_ptr compress) {
    auto dst = reinterpret_cast<jpeg_dst*>(compress->dest);
    dst->buffer = reinterpret_cast<uint8_t*>((*compress->mem->alloc_small)
                 (reinterpret_cast<j_common_ptr>(compress), JPOOL_IMAGE, buffer_size * sizeof(uint8_t)));

    dst->jpeg.next_output_byte = dst->buffer;
    dst->jpeg.free_in_buffer = buffer_size;
  };

  dst->jpeg.term_destination = [](j_compress_ptr jpeg_ptr) {
    auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr->dest);
    auto count = buffer_size - dst->jpeg.free_in_buffer;

    if (count > 0) {
      dst->stream->write(reinterpret_cast<char*>(dst->buffer), count);
    }
  };

  auto channels = image[0],
       width    = image[1],
       height   = image[2];

  jpeg_ptr.image_width = width, jpeg_ptr.image_height = height;

  switch (channels) {
  case 1:
    jpeg_ptr.in_color_space = JCS_GRAYSCALE;
    jpeg_ptr.input_components = 1;
    break;
  case 3:
    jpeg_ptr.in_color_space = JCS_RGB;
    jpeg_ptr.input_components = 3;
    break;
  default:
    return false;
  }

  jpeg_set_defaults(&jpeg_ptr);
  jpeg_set_quality(&jpeg_ptr, 95, TRUE);

  jpeg_start_compress(&jpeg_ptr, TRUE);

  while (jpeg_ptr.next_scanline < height) {
    auto r = image.buffer() + (width * channels * jpeg_ptr.next_scanline);
    jpeg_write_scanlines(
      &jpeg_ptr,
      (JSAMPARRAY)&r,
      1);
  }

  jpeg_finish_compress(&jpeg_ptr);
  jpeg_destroy_compress(&jpeg_ptr);
  return true;
}

boolean jpeg_readproc(j_decompress_ptr jpeg_ptr) {
  auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr->src);
  src->stream->read(reinterpret_cast<char*>(src->buffer), buffer_size);

  src->jpeg.next_input_byte = src->buffer;
  src->jpeg.bytes_in_buffer = (size_t)src->stream->gcount();
  return TRUE;
}

boolean jpeg_writeproc(j_compress_ptr jpeg_ptr) {
  auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr->dest);
  dst->stream->write(reinterpret_cast<char*>(dst->buffer), buffer_size);

  dst->jpeg.next_output_byte = dst->buffer;
  dst->jpeg.free_in_buffer = buffer_size;
  return TRUE;
}

void jpeg_message(j_common_ptr jpeg_ptr) {
  char msg[JMSG_LENGTH_MAX]; jpeg_ptr->err->format_message(jpeg_ptr, msg);
  log_(info, "[libjpeg] {}", msg);
}

void jpeg_exit(j_common_ptr jpeg_ptr) {
  jpeg_ptr->err->output_message(jpeg_ptr);
  if (jpeg_ptr->err->msg_code != JERR_UNKNOWN_MARKER) {
    jpeg_destroy(jpeg_ptr);
  }
}

} // namespace detail
} // namespace ig
