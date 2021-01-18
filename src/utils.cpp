#include "vcr/utils.hpp"

#include <algorithm>

namespace vcr {


std::string compact_string(const std::string& str) {
  return str.substr(0, str.find(' ', 1));
}


} // namespace vcr
