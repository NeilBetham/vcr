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
  }

  return 0;
}
