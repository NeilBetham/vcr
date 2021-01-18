/**
 * @brief Representation of drive state
 */

#include <cstdint>
#include <string>

#pragma once

namespace vcr {


class Drive {
public:
  Drive() {};
  Drive(uint64_t address, bool full, std::string medium_id, uint64_t source_addr) :
    _address(address), _full(full), _medium_id(medium_id), _source_addr(source_addr) {};

  uint64_t address() const { return _address; };
  bool full() const { return _full; };
  const std::string& medium_id() const { return _medium_id; };
  uint64_t source_address() const { return _source_addr; };

private:
  uint64_t _address = 0;
  bool _full = false;
  std::string _medium_id;
  uint64_t _source_addr = 0;
};


} // namespace vcr
