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
namespace {


class Inventory {
public:
  Inventory(struct element_info element_info, struct element_status* element_status_array) :
    _element_info(element_info), _status_array(element_status_array),
    _robot_statuses((struct element_status*)_status_array.get()),
    _slot_statuses(_robot_statuses + _element_info.robots),
    _ie_statuses(_slot_statuses + _element_info.slots),
    _drive_statuses(_ie_statuses + _element_info.ie_stations) {}

    const element_info& info() const { return _element_info; };
    const struct element_status* robots() const { return _robot_statuses; };
    const struct element_status* slots() const { return _slot_statuses; };
    const struct element_status* ies() const { return _ie_statuses; };
    const struct element_status* drives() const { return _drive_statuses; };

private:
  struct element_info _element_info;
  ScopeGuard _status_array;
  struct element_status* _robot_statuses = NULL;
  struct element_status* _slot_statuses = NULL;
  struct element_status* _ie_statuses = NULL;
  struct element_status* _drive_statuses = NULL;
};


Inventory get_inventory(int device_fd) {
  // Get the changer info
  struct element_info e_info;
  if(ioctl(device_fd, SMCIOC_ELEMENT_INFO, &e_info) == -1) {
    throw DeviceError(fmt::format("Failed to get library element info: {}", strerror(errno)));
  }

  // Allocate some buffers to do the invetory
  uint64_t total_inventory_count = e_info.robots + e_info.slots + e_info.ie_stations + e_info.drives;
  uint64_t status_block_size = sizeof(element_status) * total_inventory_count;
  struct element_status* library_statuses = (struct element_status*)malloc(status_block_size);
  if(library_statuses == NULL) {
    throw VCRException(fmt::format("Failed to allocate memory to inspect library status: {}", strerror(errno)));
  }
  memset((void*)library_statuses, 0, status_block_size);

  // Setup the return inventory object
  Inventory inv(e_info, library_statuses);

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
  if(ioctl(device_fd, SMCIOC_INVENTORY, &library_inventory) == -1) {
    throw DeviceError(fmt::format("Failed to get library inventory: {}", strerror(errno)));
  }

  return inv;
}


} // namespace


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

std::vector<Medium> Changer::list_media() const {
  auto inventory = get_inventory(_device_fd);

  // Start enumerating the different locations for media
  std::vector<Medium> media;
  media.reserve(inventory.info().slots + inventory.info().drives + inventory.info().ie_stations);

  for(uint64_t index = 0; index < inventory.info().slots; index++) {
    if(!inventory.slots()[index].full) { continue; }
    std::string medium_id((char*)inventory.slots()[index].volume, 36);
    media.push_back(Medium(compact_string(medium_id), inventory.slots()[index].address, MediumLocation::slot));
  }

  for(uint64_t index = 0; index < inventory.info().drives; index++) {
    if(!inventory.drives()[index].full) { continue; }
    std::string medium_id((char*)inventory.drives()[index].volume, 36);
    media.push_back(Medium(compact_string(medium_id), inventory.drives()[index].address, MediumLocation::drive));
  }

  for(uint64_t index = 0; index < inventory.info().drives; index++) {
    if(!inventory.ies()[index].full) { continue; }
    std::string medium_id((char*)inventory.ies()[index].volume, 36);
    media.push_back(Medium(compact_string(medium_id), inventory.ies()[index].address, MediumLocation::ie));
  }

  return media;
}

std::vector<Slot> Changer::list_slots() const {
  auto inventory = get_inventory(_device_fd);
  std::vector<Slot> slots;
  slots.reserve(inventory.info().slots);

  for(uint64_t index = 0; index < inventory.info().slots; index++) {
    std::string medium_id;
    if(inventory.slots()[index].full) {
      medium_id = compact_string(std::string((char*)inventory.slots()[index].volume, 36));
    }
    slots.push_back(Slot(
      inventory.slots()[index].address,
      inventory.slots()[index].full,
      medium_id
    ));
  }

  return slots;
}

std::vector<Drive> Changer::list_drives() const {
  auto inventory = get_inventory(_device_fd);
  std::vector<Drive> drives;
  drives.reserve(inventory.info().drives);

  for(uint64_t index = 0; index < inventory.info().drives; index++) {
    std::string medium_id;
    if(inventory.drives()[index].full) {
      medium_id = compact_string(std::string((char*)inventory.drives()[index].volume, 36));
    }
    drives.push_back(Drive(
      inventory.drives()[index].address,
      inventory.drives()[index].full,
      medium_id,
      inventory.drives()[index].source
    ));
  }

  return drives;
}

std::vector<Robot> Changer::list_robots() const {
  auto inventory = get_inventory(_device_fd);
  std::vector<Robot> robots;
  robots.reserve(inventory.info().robots);

  for(uint64_t index = 0; index < inventory.info().robots; index++) {
    std::string medium_id;
    if(inventory.robots()[index].full) {
      medium_id = compact_string(std::string((char*)inventory.robots()[index].volume, 36));
    }
    robots.push_back(Robot(inventory.robots()[index].address));
  }

  return robots;
}

bool Changer::load_media(Medium medium, Drive drive) {
  auto robot_list = list_robots();
  if(robot_list.size() < 1) {
    throw DeviceError("No robots available to move medium");
  }
  struct move_medium mm;
  mm.source = medium.address();
  mm.destination = drive.address();
  mm.robot = robot_list.front().address();
  mm.invert = 0;

  if(ioctl(_device_fd, SMCIOC_MOVE_MEDIUM, &mm) == -1) {
    throw DeviceError(fmt::format("Failed to move medium `{}` to drive {}", medium.id(), drive.address()));
  }

  return true;
}

bool Changer::unload_media(Drive drive, Slot slot) {
  auto robot_list = list_robots();
  if(robot_list.size() < 1) {
    throw DeviceError("No robots available to move medium");
  }
  struct move_medium mm;
  mm.source = drive.address();
  mm.destination = slot.address();
  mm.robot = robot_list.front().address();
  mm.invert = 0;

  if(ioctl(_device_fd, SMCIOC_MOVE_MEDIUM, &mm) == -1) {
    throw DeviceError(fmt::format("Failed to move medium `{}` from drive {} to slot {}", drive.medium_id(), drive.address(), slot.address()));
  }

  return true;
}


} // namespace vcr
