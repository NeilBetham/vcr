#include "vcr/changer.hpp"

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
#include "vcr/utils.hpp"


namespace vcr {


Changer::Changer(const std::string& device_path) : _device_path(device_path) {
  _device_fd = open(_device_path.c_str(), O_SYNC, O_RDWR);
  if(_device_fd == -1) {
    throw DeviceError(fmt::format("Error opening changer device `{}`: {}", _device_path, strerror(errno)));
  }
}

Changer::~Changer() {
  if(close(_device_fd) == -1) {
    spdlog::warn("Failed to close changer device handle `{}`: {}", _device_path, strerror(errno));
  }
}

std::vector<std::string> Changer::list_media() const {
  // Get the changer info
  struct element_info e_info;
  if(ioctl(_device_fd, SMCIOC_ELEMENT_INFO, &e_info) == -1) {
    spdlog::error("Changer IOCTL failed `{}`: {}", _device_path, strerror(errno));
    return {};
  }

  // Allocate some buffers to do the invetory
  uint64_t status_block_size = sizeof(element_status) * e_info.robots + e_info.slots + e_info.ie_stations + e_info.drives;
  struct element_status* library_statuses = (struct element_status*)malloc(status_block_size);
  if(library_statuses == NULL) {
    throw VCRException(fmt::format("Failed to allocate memory to inspect library status: {}", strerror(errno)));
  }
  memset((void*)library_statuses, 0, status_block_size);
  ScopeGuard sg((void*)library_statuses);

  // Setup some other pointers to the individual status array blocks
  struct element_status* robot_statuses = library_statuses;
  struct element_status* slot_statuses  = library_statuses + e_info.robots;
  struct element_status* ie_statuses    = library_statuses + e_info.robots + e_info.slots;
  struct element_status* drive_statuses = library_statuses + e_info.robots + e_info.slots + e_info.ie_stations;

  // Setup inventory struct
  struct inventory library_inventory;
  library_inventory.robot_status = robot_statuses;
  library_inventory.slot_status = slot_statuses;
  library_inventory.ie_status = ie_statuses;
  library_inventory.drive_status = drive_statuses;

  // Get the library inventory
  if(ioctl(_device_fd, SMCIOC_INVENTORY, &library_inventory) == -1) {
    throw DeviceError(fmt::format("Failed to get library inventory `{}`: {}", _device_path, strerror(errno)));
  }

  // Start enumerating the slot info looking for media
  std::vector<std::string> media_ids;
  media_ids.reserve(e_info.slots);
  for(uint64_t index = 0; index < e_info.slots; index++) {
    if(!slot_statuses[index].full) { continue; }
    media_ids.push_back(std::string((char*)(&slot_statuses[index].volume), 36));
  }

  return media_ids;
}

std::vector<std::string> Changer::list_slots() const {
  return {};
}

std::vector<std::string> Changer::list_drives() const {
  return {};
}

bool Changer::load_media(std::string media_id) {
  return false;
}

bool Changer::unload_media(std::string drive_id, std::string slot_id) {
  return false;
}

bool Changer::move_media(std::string slot_id_from, std::string slot_id_to) {
  return false;
}


} // namespace vcr
