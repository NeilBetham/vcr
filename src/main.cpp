#include <args.hxx>
#include <spdlog/spdlog.h>
#include <sodium.h>

#include "args.hxx"
#include "vcr/vcr.hpp"

using namespace std;

int main(int argc, char** argv) {
  // This needs to be done before any crypto ops to ensure the lib is in a good state
  sodium_init();

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
  }

  return 0;
}
