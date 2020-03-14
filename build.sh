#!/bin/bash
export NDK_HOME=/Users/jy/Library/Android/sdk/ndk-bundle
export CMAKE_HOME=/Users/jy/Library/Android/sdk/cmake/3.6.4111459/bin:$PATH
export PATH=$CMAKE_HOME:$PATH

ANDROID_CPU=armeabi-v7a
OUTPUT=$(pwd)/output/$ANDROID_CPU

if [[ “$@“ =~ "-d" ]];then
        echo "----------------------------cmake debug----------------------------"
cmake -DDEBUG=ON -DCMAKE_TOOLCHAIN_FILE=$NDK_HOME/build/cmake/android.toolchain.cmake \
      -DANDROID_NDK=$NDK_HOME \
      -DANDROID_ABI=$ANDROID_CPU \
      -DANDROID_TOOLCHAIN_NAME=clang \
      -DANDROID_NATIVE_API_LEVEL=21 \
      -DANDROID_STL=gnustl_static \
      .
else      
        echo "----------------------------cmake release----------------------------"
cmake -DDEBUG=NO -DCMAKE_TOOLCHAIN_FILE=$NDK_HOME/build/cmake/android.toolchain.cmake \
      -DANDROID_NDK=$NDK_HOME \
      -DANDROID_ABI=$ANDROID_CPU \
      -DANDROID_TOOLCHAIN_NAME=clang \
      -DANDROID_NATIVE_API_LEVEL=21 \
      -DANDROID_STL=gnustl_static \
      .
fi

make -j4

make DESTDIR=$OUTPUT install

rm -rf CMakeCache.txt
rm -rf CMakeFiles
rm -rf cmake_install.cmake
rm -rf Makefile
rm -rf CTestTestfile.cmake
