load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
    name = 'file_header',
    srcs = ['file_header.proto'],
)

proto_library(
    name = 'payload_header',
    srcs = ['payload_header.proto'],
)

cc_proto_library(
    name = 'tape_data_headers',
    deps = [
      ':file_header',
      ':payload_header',
    ],
    visibility = ['//visibility:public'],
)
