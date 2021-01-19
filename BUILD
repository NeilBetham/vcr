cc_library(
  name = 'vcr-lib',
  hdrs = glob(['include/**/*.hpp']),
  srcs = glob(['src/**/*.cpp'], exclude = ['src/main.cpp']),
  linkstatic = True,
  strip_include_prefix = 'include/',
  deps = [
    '@spdlog//:spdlog',
    '@libsodium//:libsodium',
  ],
)

cc_binary(
  name = 'vcr',
  srcs = ['src/main.cpp'],
  deps = [
    ':vcr-lib',
    '@args//:args',
  ],
  features = [
    '-pie',
    'fully_static_link',
  ],
  linkopts = [
    '-l:libm.a',
    '-l:libstdc++.a',
    '-pthreads',
  ],
  linkstatic = 1,
)
