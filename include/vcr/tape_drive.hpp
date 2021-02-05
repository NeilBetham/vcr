/**
 * @brief Interface with a tape drive
 */

#include <cstdint>
#include <string>

#include "vcr/buffer.hpp"

#pragma once

namespace vcr {


class TapeDrive {
public:
  TapeDrive() {};
  TapeDrive(std::string device_path);
  ~TapeDrive();

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
  bool rewind(uint64_t files = 0);
  bool fastforward(uint64_t files = 0);

  // R/W Operations
  uint64_t write(const Buffer& buffer);
  Buffer read(uint64_t byte_count);
  bool write_file_mark();
  bool erase_tape();

private:
  void check_drive_device();
  bool issue_op_command(uint16_t operation, uint64_t count = 0);

  std::string _device_path;
  int _device_fd = -1;
};


} // namespace vcr
