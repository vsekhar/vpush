# add additional user code [sub]directories to this list, relative to the repository root

usrdirs = Split("""
src
src/util
src/detail
src/library
""")

### END USER MODIFIABLES ###

lib_target  = 'cvpush'
subdirs = usrdirs

Import('env')
env.Append(CPPPATH = ['/usr/include/python2.7', 'include'])
env.Append(LIBS = ['boost_serialization', 'boost_python'])
env.Append(CCFLAGS = ['-std=c++0x', '-Wall', '-fmessage-length=0'])

globpatterns = [s+'/*.cpp' for s in subdirs]
filelist = map(Glob, globpatterns)
files = reduce(lambda x,y:x+y, filelist)

lib = env.SharedLibrary(target = lib_target, source = files, SHLIBPREFIX='')
Default(lib)

