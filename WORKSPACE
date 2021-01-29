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
  sha256 = "308eb09a9656315b38c2d1f7526b22b42695b352da316fa4ac17a8b86cdd4663",
  build_file = "args.BUILD",
)

http_archive(
    name = "rules_foreign_cc",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/d54c78ab86b40770ee19f0949db9d74a831ab9f0.tar.gz",
    sha256 = "e7446144277c9578141821fc91c55a61df7ae01bda890902f7286f5fd2f6ae46",
    strip_prefix="rules_foreign_cc-d54c78ab86b40770ee19f0949db9d74a831ab9f0",
)
load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

http_archive(
  name="libsodium",
  url="https://github.com/jedisct1/libsodium/archive/89916bbe11cfb2a40f45521c2c7187d042e5023f.tar.gz",
  sha256="1afcca21f0a8d50d668801e280787cd953f26527adcec89b75d5159cabd9222f",
  strip_prefix = "libsodium-89916bbe11cfb2a40f45521c2c7187d042e5023f",
  build_file = "libsodium.BUILD",
)

# rules_cc defines rules for generating C++ code from Protocol Buffers.
http_archive(
    name = "rules_cc",
    sha256 = "71d037168733f26d2a9648ad066ee8da4a34a13f51d24843a42efa6b65c2420f",
    strip_prefix = "rules_cc-b1c40e1de81913a3c40e5948f78719c28152486d",
    url = "https://github.com/bazelbuild/rules_cc/archive/b1c40e1de81913a3c40e5948f78719c28152486d.tar.gz",
)

# rules_proto defines abstract rules for building Protocol Buffers.
http_archive(
    name = "rules_proto",
    sha256 = "d8992e6eeec276d49f1d4e63cfa05bbed6d4a26cfe6ca63c972827a0d141ea3b",
    strip_prefix = "rules_proto-cfdc2fa31879c0aebe31ce7702b1a9c8a4be02d2",
    url = "https://github.com/bazelbuild/rules_proto/archive/cfdc2fa31879c0aebe31ce7702b1a9c8a4be02d2.tar.gz",
)

load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")
rules_cc_dependencies()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()
