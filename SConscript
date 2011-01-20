# add additional user code [sub]directories to this list, relative to the repository root

usrdirs = Split("""
src
src/util
""")

### END USER MODIFIABLES ###

#lib_target  = '_vmgp'
target = 'vpush'
subdirs = usrdirs

Import('env')
env.Append(CPPPATH = ['include'])
env.Append(LIBS = [])
env.Append(CCFLAGS = ['-Wall', '-fmessage-length=0'])

globpatterns = [s+'/*.cpp' for s in subdirs]
filelist = map(Glob, globpatterns)
files = reduce(lambda x,y:x+y, filelist)

#lib = env.SharedLibrary(target = lib_target, source = files, SHLIBPREFIX='')
lib = env.Program(target = target, source = files)
Default(lib)

