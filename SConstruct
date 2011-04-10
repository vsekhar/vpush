release_env = Environment(variant = 'release')
release_env.Append(CCFLAGS=['-O3', '-march=native', '-std=c++0x'])
release_env.Append(LIBS=['boost_serialization'])

# Once GCC 4.5+ is installed, use change above to something like this
# to enable link-time optimization
# release_env.Append(CCFLAGS=['-O3', '-flto', '-march=native', '-std=c++0x'])
# release_env.Append(LDFLAGS=['-O3', '-flto'])

debug_env = Environment(variant = 'debug')
debug_env.Append(CCFLAGS=['-g','-O0', '-std=c++0x'])
debug_env.Append(CPPDEFINES=['_DEBUG'])
debug_env.Append(LIBS=['boost_serialization'])

SConscript('SConscript', variant_dir="Release", exports={'env':release_env}, duplicate=0)
SConscript('SConscript', variant_dir="Debug", exports={'env':debug_env}, duplicate=0)

