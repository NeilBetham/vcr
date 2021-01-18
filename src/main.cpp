#include <args.hxx>
#include <spdlog/spdlog.h>

#include "args.hxx"
#include "vcr/vcr.hpp"

using namespace std;

int main(int argc, char** argv) {
  args::ArgumentParser parser("Tape Library File Archiver", "");
  args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
  args::ValueFlag<std::string> changer_device(parser, "changer-device", "The device file for the media changer", {'m', "mtx"});

  try {
    parser.ParseCLI(argc, argv);
  } catch (args::Help) {
    std::cerr << parser;
  } catch (args::ParseError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  } catch (args::ValidationError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  }

  if(changer_device) {
    vcr::Changer mtx(args::get(changer_device));
    spdlog::info("Media In Library:");
    for(const auto& medium : mtx.list_media()) {
      spdlog::info("Tape: {}", medium.id());
    }

    spdlog::info("Slots in Library:");
    for(const auto& slot : mtx.list_slots()) {
      spdlog::info("Slot [{}] - Full: {}, Medium ID: '{}'", slot.address(), slot.full(), slot.medium_id());
    }

    spdlog::info("Drives in Library:");
    for(const auto& drive : mtx.list_drives()) {
      spdlog::info("Drive [{}] - Full: {}, Medium ID: '{}', Source Addr: {}", drive.address(), drive.full(), drive.medium_id(), drive.source_address());

    }

    spdlog::info("Robots in Library:");
    for(const auto& robot : mtx.list_robots()) {
      spdlog::info("Robot [{}]", robot.address());

    }

    spdlog::info("Try moving media into and out of drive");
    auto media = mtx.list_media();
    auto drives = mtx.list_drives();
    spdlog::info("Loading media {} into drive {}", media.front().id(), drives.front().address());
    mtx.load_media(media.front(), drives.front());
    spdlog::info("Media loaded");
    sleep(30);
    spdlog::info("Unloading media {} from drive {} to slot {}", media.front().id(), drives.front().address(), media.front().address());
    mtx.unload_media(drives.front(), vcr::Slot(media.front().address(), false, ""));
    spdlog::info("Media unloaded");

  }

  return 0;
}
