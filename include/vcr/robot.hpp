/**
 * @brief Representation of robot state
 */

#include <cstdint>

#pragma once


namespace vcr {


class Robot {
public:
  Robot() {};
  Robot(uint64_t address) : _address(address) {};

  uint64_t address() const { return _address; };

private:
  uint64_t _address;
};


} // namespace vcr
