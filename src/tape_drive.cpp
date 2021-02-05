#include "vcr/tape_drive.hpp"

#include <cstdint>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <spdlog/spdlog.h>
#include <stdlib.h>

#include "vcr/exceptions.hpp"
#include "vcr/ibm_library_types.hpp"

namespace vcr {


TapeDrive::TapeDrive(std::string device_path) : _device_path(device_path) {
  _device_fd = open(_device_path.c_str(), O_SYNC, O_RDWR);
  if(_device_fd == -1) {
    throw DeviceError(fmt::format("Error opening tape drive device `{}`: {}", _device_path, strerror(errno)));
  }

  // Init the drive with some sane parameters
  // Read the current params
  struct stchgp_s drive_params;
  if(ioctl(_device_fd, STIOCQRYP, &drive_params) == -1) {
    throw DeviceError(fmt::format("Failed to query tape drive parameters: {}", strerror(errno)));
  }
  spdlog::info("[{}] Drive - Min Block Size: {}, Max Block Size: {}", _device_path, drive_params.min_blksize, drive_params.max_blksize);

  // Set the params we care about
  drive_params.rewind_immediate = 0;
  drive_params.compression = 0;
  drive_params.pews[0] = 0;
  drive_params.pews[1] = 0;
  if(ioctl(_device_fd, STIOCSETP, &drive_params) == -1) {
    throw DeviceError(fmt::format("Failed to set tape drive parameters: {}", strerror(errno)));
  }
}

TapeDrive::~TapeDrive() {
  if(close(_device_fd) == -1) {
    spdlog::warn("Error closing tape drive device handle `{}`: {}", _device_path, strerror(errno));
  }
}

// Get media info
bool TapeDrive::media_present() {
  check_drive_device();
  struct report_density_support density_report;
  density_report.media = CURRENT_MEDIA_DENSITY;
  if (ioctl(_device_fd, STIOC_REPORT_DENSITY_SUPPORT, &density_report) == -1) {
    return false;
  }
  return true;
}

uint64_t TapeDrive::media_size() {
  check_drive_device();
  struct report_density_support density_report;
  density_report.media = CURRENT_MEDIA_DENSITY;
  if (ioctl(_device_fd, STIOC_REPORT_DENSITY_SUPPORT, &density_report) == -1) {
    throw DeviceError(fmt::format("Failed to get media info from tape drive `{}`: {}", _device_path, strerror(errno)));
  }

  if(density_report.number_reports > 1) {
    throw DeviceError(fmt::format("Device returned multiple density reports `{}`: {}", _device_path, strerror(errno)));
  }

  struct density_report& dr = density_report.reports[0];
  return (uint64_t)dr.capacity * 1000000;
}

uint64_t TapeDrive::media_pos() {
  check_drive_device();
  uint32_t current_pos;
  if (ioctl(_device_fd, STIOC_READ_POSITION, &current_pos) == -1) {
    throw DeviceError(fmt::format("Failed to get position info from tape drive `{}`: {}", _device_path, strerror(errno)));
  }
  return current_pos;
}

// Tape control
bool TapeDrive::lock_tape() {
  check_drive_device();
  if (ioctl(_device_fd, STIOC_PREVENT_MEDIUM_REMOVAL, NULL) == -1) {
    spdlog::error("Failed to lock tape into drive `{}`: {}", _device_path, strerror(errno));
    return false;
  }

  return true;
}

bool TapeDrive::unlock_tape() {
  check_drive_device();
  if (ioctl(_device_fd, STIOC_ALLOW_MEDIUM_REMOVAL, NULL) == -1) {
    spdlog::error("Failed to unlock tape into drive `{}`: {}", _device_path, strerror(errno));
    return false;
  }

  return true;
}

bool TapeDrive::flush() {
  check_drive_device();
  if(ioctl(_device_fd, STIOCSYNC, NULL) == -1) {
    spdlog::error("Failed to flush drive buffers to tape: {}", strerror(errno));
    return false;
  }
  return true;
}

// Move around the tape
bool TapeDrive::next() {
  check_drive_device();
  return issue_op_command(STFSF, 1);
}

bool TapeDrive::previous() {
  check_drive_device();
  return issue_op_command(STRSF, 1);
}

bool TapeDrive::rewind(uint64_t files) {
  check_drive_device();
  if(files > 0) {
    return issue_op_command(STRSF, files);
  } else {
    return issue_op_command(STREW, 0);
  }
}

bool TapeDrive::fastforward(uint64_t files) {
  check_drive_device();
  if(files > 0) {
    return issue_op_command(STFSF, files);
  } else {
    return issue_op_command(STSEOD, 0);
  }
}

// R/W Operations
uint64_t TapeDrive::write(const Buffer& buffer) {
  check_drive_device();
  return 0;
}

Buffer TapeDrive::read(uint64_t byte_count) {
  check_drive_device();
  return Buffer();
}

bool TapeDrive::write_file_mark() {
  check_drive_device();
  return issue_op_command(STWEOF, 1);
}

bool TapeDrive::erase_tape() {
  check_drive_device();
  return issue_op_command(STERASE, 0);
}

void TapeDrive::check_drive_device() {
  if(_device_fd == -1) {
    throw DriveDeviceNotOpen("Drive fd is -1");
  }
}

bool TapeDrive::issue_op_command(uint16_t operation, uint64_t count) {
  struct stop op_to_perform;
  op_to_perform.st_op = operation;
  op_to_perform.st_count = static_cast<daddr_t>(count);

  if (ioctl(_device_fd, STIOCTOP, &op_to_perform) == -1) {
    spdlog::error("Tape operation failed: OP: {}, Count: {}, Error: {}", operation, count, strerror(errno));
    return false;
  }
  return true;
}

} // namespace vcr
