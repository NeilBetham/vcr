/**
 * @brief Some utility functions and classes
 */

#include <string>

#pragma once

namespace vcr {


class ScopeGuard {
public:
  ScopeGuard(void* ptr) : _ptr(ptr) {}
  ~ScopeGuard() { if(_ptr != NULL){ free(_ptr); } }

  const void* get() { return _ptr; };
private:
  void* _ptr;
};

std::string compact_string(const std::string& str);

} // namespace vcr
