/**
 * @brief Media Changer Interface
 */

#include <cstdint>
#include <string>
#include <vector>

#include "vcr/drive.hpp"
#include "vcr/medium.hpp"
#include "vcr/slot.hpp"

#pragma once


namespace vcr {


class Changer {
public:
  Changer(const std::string& device_path);
  ~Changer();

  std::vector<Medium> list_media() const;
  std::vector<Slot> list_slots() const;
  std::vector<Drive> list_drives() const;

  bool load_media(Medium medium, Drive drive);
  bool unload_media(Drive drive, Slot slot);
  bool move_media(Slot slot_from, Slot slot_to);

private:
  std::string _device_path;
  int _device_fd = -1;
};


} // namespace VCR
