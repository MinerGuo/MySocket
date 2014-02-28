#!/bin/bash
cur_pat=$(cd `dirname $0`; pwd)
echo ${cur_pat}
export C_INCLUDE_PATH=${cur_pat}/otherlibs/include/Tcmalloc:${cur_pat}/otherlibs/include/Lua:${cur_pat}/otherlibs/include/:${cur_pat}/firtexlib/include:$C_INCLUDE_PATH:
export CPLUS_INCLUDE_PATH=${cur_pat}/otherlibs/include/Tcmalloc:${cur_pat}/otherlibs/include/Lua:${cur_pat}/otherlibs/include/:${cur_pat}/firtexlib/include:$CPLUS_INCLUDE_PATH

export LD_LIBRARY_PATH=${cur_pat}/sharedlib/tcmalloc:${cur_pat}/sharedlib/poco:${cur_pat}/gmock/lib:${cur_pat}/sharedlib/lua:/home/wmin/otherlib/poco:$LD_LIBRARY_PATH
export LIBRARY_PATH=${cur_pat}/sharedlib/tcmalloc:${cur_pat}/sharedlib/poco:${cur_pat}/gmock/lib:${cur_pat}/sharedlib/lua:/home/wmin/otherlib/poco:$LIBRARY_PATH
 
if [ ! -d "build" ]; then
mkdir "build"
fi

cd build
cmake -DCMAKE_BUILD_TYPE=DEBUG ..


make 
#make -j4


