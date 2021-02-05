/**
 * @brief A simple buffer repesentation
 */

#include <cstdint>

#pragma once


namespace vcr {

class Buffer {
public:
  Buffer() {};
  Buffer(uint64_t size);
  ~Buffer();

  const uint8_t* get() const { return _buffer; };
  uint8_t* get() { return _buffer; };

  uint64_t size() const { return _size; };

  Buffer concat(const Buffer& other) const;

private:
  uint64_t _size;
  uint8_t* _buffer;
};

} // namespace vcr
