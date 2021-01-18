/**
 * @brief Representation of media in the library
 */

#pragma once


namespace vcr {

enum class MediumLocation {
  unknown = 0,
  slot = 1,
  ie = 2,
  drive = 3
};


class Medium {
public:
  Medium() {};
  Medium(const std::string& volume_id, uint64_t _element_address, MediumLocation location) :
    _volume_id(volume_id), _element_address(_element_address), _location(location) {};

  std::string id() const { return _volume_id; };
  uint64_t address() const { return _element_address; };
  MediumLocation location() const { return _location; };

private:
  std::string _volume_id;
  uint64_t _element_address = 0;
  MediumLocation _location = MediumLocation::unknown;
};


} // namespace vcr
