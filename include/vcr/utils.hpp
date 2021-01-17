/**
 * @brief Some utility functions and classes
 */

#pragma once

namespace vcr {


class ScopeGuard {
public:
  ScopeGuard(void* ptr) : _ptr(ptr) {}
  ~ScopeGuard() { if(_ptr != NULL){ free(_ptr); } }
private:
  void* _ptr;
};


} // namespace vcr
