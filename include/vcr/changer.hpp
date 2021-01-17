/**
 * @brief Media Changer Interface
 */

#include <optional>
#include <string>
#include <vector>

#pragma once


namespace vcr {


class Changer {
public:
  Changer(const std::string& device_path);
  ~Changer();

  std::vector<std::string> list_media() const;
  std::vector<std::string> list_slots() const;
  std::vector<std::string> list_drives() const;

  bool load_media(std::string media_id);
  bool unload_media(std::string drive_id, std::string slot_id);
  bool move_media(std::string slot_id_from, std::string slot_id_to);

private:
  std::string _device_path;
  int _device_fd = -1;
};


} // namespace VCR
