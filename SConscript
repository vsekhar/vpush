# add additional user code [sub]directories to this list, relative to the repository root

usrdirs = Split("""
src
src/util
src/detail
src/library
""")

### END USER MODIFIABLES ###

lib_target  = 'vpush'
#target = 'vpush'
subdirs = usrdirs

Import('env')
env.Append(CPPPATH = ['/usr/include/python3.1', 'include'])
#env.Append(LIBS = [])
env.Append(CCFLAGS = ['-std=c++0x', '-Wall', '-fmessage-length=0'])

globpatterns = [s+'/*.cpp' for s in subdirs]
filelist = map(Glob, globpatterns)
files = reduce(lambda x,y:x+y, filelist)

lib = env.SharedLibrary(target = lib_target, source = files, SHLIBPREFIX='')
#lib = env.Program(target = target, source = files)
Default(lib)

