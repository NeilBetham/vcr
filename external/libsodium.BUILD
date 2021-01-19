load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

filegroup(
  name = "all",
  srcs = glob(["**"]),
  visibility = ["//visibility:public"]
)

configure_make(
    name = "libsodium",
    lib_source = ':all',
    out_lib_dir = "lib",
    static_libraries = ["libsodium.a"],
    configure_in_place = True,
    autogen = True,
    autogen_options = ['-s'],
    visibility = ["//visibility:public"],
)
