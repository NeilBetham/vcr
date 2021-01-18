/**
 * @brief Represntation of slot state
 */


#include <cstdint>
#include <string>

#pragma once


namespace vcr {


class Slot {
public:
  Slot() {};
  Slot(uint64_t address, bool full, std::string medium_id) :
    _address(address), _full(full), _medium_id(medium_id) {};

  uint64_t address() const { return _address; };
  bool full() const { return _full; };
  const std::string& medium_id() const { return _medium_id; };

private:
  uint64_t _address = 0;
  bool _full = false;
  std::string _medium_id;
};



} // namespace
