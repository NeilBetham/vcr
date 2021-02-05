#include "vcr/buffer.hpp"

#include <spdlog/spdlog.h>
#include <stdlib.h>

#include "vcr/exceptions.hpp"


namespace vcr {


Buffer::Buffer(uint64_t size) : _size(size) {
  _buffer = (uint8_t*)malloc(size);
  if(_buffer == NULL) {
    throw VCRException(fmt::format("Failed to allocated memory of size: {}", _size));
  }
}

Buffer::~Buffer() {
  free(_buffer);
}

Buffer Buffer::concat(const Buffer& other) const {
  Buffer ret(size() + other.size());
  memcpy(ret.get(), get(), size());
  memcpy(ret.get() + size(), other.get(), other.size());
  return ret;
}


} // namespace vcr
