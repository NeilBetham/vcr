/**
 * @brief Interface with a tape drive
 */

#include <cstdint>
#include <string>

#pragma once

namespace vcr {


class TapeDrive {
public:
  TapeDrive() {};
  TapeDrive(std::string device_path);

  // Get media info
  bool media_present();
  uint64_t media_size();
  uint64_t media_pos();

  // Tape control
  bool lock_tape();
  bool unlock_tape();
  bool flush();

  // Move around the tape
  bool next();
  bool previous();
  bool rewind(uint64_t bytes = 0);
  bool fastforward(uint64_t bytes = 0);

  // R/W Operations
  uint64_t write();
  uint64_t read(uint64_t byte_count);
  bool write_file_mark();
  bool erase_tape();

private:
  std::string _device_path;
  int _device_fd = -1;
};


} // namespace vcr
