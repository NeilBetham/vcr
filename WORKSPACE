workspace(name = "vcr")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "spdlog",
  urls = ["https://github.com/gabime/spdlog/archive/v1.8.2.tar.gz"],
  sha256 = "e20e6bd8f57e866eaf25a5417f0a38a116e537f1a77ac7b5409ca2b180cec0d5",
  build_file = "spdlog.BUILD",
)

http_archive(
  name = "args",
  urls = ["https://github.com/Taywee/args/archive/6.2.4.tar.gz"],
  sha256 = "dcc6d0d6b941eb40eeeb5741917d557944f3880e0dea9096147d0bdd89c34654",
  build_file = "args.BUILD",
)
