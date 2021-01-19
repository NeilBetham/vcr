load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
    name = 'tape_headers',
    srcs = [
      'file_header.proto',
      'payload_header.proto',
    ],
)

cc_proto_library(
    name = 'tape_data_headers',
    deps = [':tape_headers'],
    visibility = ['//visibility:public'],
)
