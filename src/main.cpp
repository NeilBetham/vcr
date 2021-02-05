#include <args.hxx>
#include <spdlog/spdlog.h>
#include <sodium.h>

#include "args.hxx"
#include "vcr/vcr.hpp"

using namespace std;

int main(int argc, char** argv) {
  // This needs to be done before any crypto ops to ensure the lib is in a good state
  if(sodium_init() == -1) {
    spdlog::critical("Failed to initialize cryptography");
    return -1;
  }

  args::ArgumentParser parser("Tape Library File Archiver", "");
  args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
  args::ValueFlag<std::string> changer_device(parser, "changer-device", "The device file for the media changer", {'m', "mtx"});
  args::ValueFlag<std::string> tape_device(parser, "tape-device", "The device file for the tape drive", {'t', "mt"});
  args::ValueFlag<std::string> db_file(parser, "db-file", "The file used as an index to all the archvied files", {'b', "db"});
  args::ValueFlag<std::string> backup_dir(parser, "backup-dir", "The directory to backup", {'d', "dir"});
  args::ValueFlag<std::string> secret_key_file(parser, "secret-key-file", "The secret key used to encrypt and decrypt stored data", {'k', "key"});
  args::Positional<std::string> operation(parser, "operation", "The operation to perform");

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
