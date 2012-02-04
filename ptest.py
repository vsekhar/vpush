#!/usr/bin/python

# Cannot statically link libc and stdc++ into cvpush.so because standard libs
# are not commonly compiled with -fPIC (due to 5% performance hit)

# Cannot send local dynamic libraries because LD search order on remote machine
# starts from standard locations before getting to local directory

# Can build remotely and upload .so library to cloud.files (or S3?), but need
# to install scons (not installed by default). Workers can then download lib
# and import it from the current directory.

# Can then setup a work flow of 1 job building, N workers (depending on first job), etc.

# Build job only needed when vpush changes, otherwise library is cached
# - can this auto-detect somehow? based on git commit hashes on the deploy branch? 

import os
import sys
import tempfile
import tarfile
import subprocess
import shlex
import shutil

import cloud

def run_cmd(command, cwd=None):
    args = shlex.split(command)
    p = subprocess.Popen(args, stdout=sys.stdout, stderr=sys.stderr, cwd=cwd)
    p.wait()

def build(git_string, build_command, output_path, upload_dest=None):
    ' Download a git repository, build it, and upload the result to cloud files '
    tdir = tempfile.mkdtemp()
    run_cmd('git clone %s .' % git_string, cwd=tdir)
    run_cmd('git checkout deploy', cwd=tdir)
    run_cmd(build_command, cwd=tdir)
    if upload_dest is None:
        upload_dest = os.path.basename(output_path)
    cloud.files.put(os.path.join(tdir, output_path), upload_dest)
    shutil.rmtree(tdir)

if __name__ == '__main__':
    jid = cloud.call(build, 'git://github.com/vsekhar/vpush.git', 'scons -j4', 'Release/cvpush.so', _env='vpush')
    cloud.result(jid)
