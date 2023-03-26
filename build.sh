#!/bin/bash
function findAndCopyDLL() 
{
    for i in "${paths[@]}"
    do
        FILE="$i/$1"
        if [ -f $FILE ]; then
           cp $FILE build/
           return 0
        fi
    done

    return 1
}

function mergeLibs()
{
  cd $1

  for lib in *.a
  do
      mkdir "$lib-o"
      ar -x $lib
      mv *.o "$lib-o"
      ar -r libHopMerged.a "$lib-o/"*
      rm -rf "$lib-o"
  done

  mv libHopMerged.a libHop.a

  cd ..
}

function buildAndroid()
{

  cmake -E make_directory build-$1
  cmake -E chdir build-$1 cmake .. -D ANDROID=ON -D ANDROID_PLATFORM=21 -D ANDROID_ABI=$1 -D BUILD_DEMOS=$DEMO -D RELEASE=$RELEASE -D TEST_SUITE=$TEST -D SYNTAX_ONLY=$SYNTAX -D SANITISE=$SANITISE -D CMAKE_TOOLCHAIN_FILE=$TOOL_CHAIN && make -j 8 -C build-$1

  mergeLibs "build-$1"
  mv build-$1/libHop.a build/libHop-$1.a
  rm -rf build-$1

}

WINDOWS=1
OSX=1
RELEASE=0
TEST=0
SYNTAX=0
SANITISE=0
DEMO=0
ANDROID_NDK=""

while [[ $# -gt 0 ]]; do
  case $1 in
    -w|--windows)
      WINDOWS=0
      shift # past argument
      ;;
    -o|--osx)
      OSX=0
      shift
      ;;
    -r|--release)
      RELEASE=1
      shift
      ;;
    -t|--test)
      TEST=1
      shift
      ;;
    -s|--syntax)
      SYNTAX=1
      shift
      ;;
    -d|--demo)
      DEMO=1
      shift
      ;;
    --sanitise)
      SANITISE=1
      shift
      ;;
    --android)
      ANDROID_NDK=$2
      shift
      shift
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
    *)
      POSITIONAL_ARGS+=("$1") # save positional arg
      shift # past argument
      ;;
  esac
done

for file in build CMakeFiles cmake_install.cmake CMakeCache.txt Makefile Jerboa
do
  if [ -d $file ];
  then
    rm -rf $file
  fi
  if [ -f $file ];
  then
    rm $file
  fi
done

echo "release ${RELEASE}"

if [[ $WINDOWS -eq 0 ]];
then 
  cmake -E make_directory build
  cmake -E chdir build cmake .. -D WINDOWS=ON -D BUILD_DEMOS=$DEMO -D RELEASE=$RELEASE -D TEST_SUITE=$TEST -D SYNTAX_ONLY=$SYNTAX -D SANITISE=$SANITISE -D CMAKE_TOOLCHAIN_FILE=./windows.cmake && make -j 8 -C build
  # now copy dlls
  PREFIX="x86_64-w64-mingw32"

  paths=("/usr/local/mingw64/bin"
    "/usr/local/mingw64/bin/x64"
     "/usr/$PREFIX/bin"
    "/usr/$PREFIX/lib"
  )

  for p in /usr/lib/gcc/$PREFIX/*
  do 
    paths+=($p)
  done

  echo -e "\n###############\nChecking Paths: \n"
  for p in "${paths[@]}"
  do
    echo -e "$p\n"
  done 
  echo -e "###############\n"

  dll=("libgcc_s_seh-1.dll"
    "libstdc++-6.dll"
    "libwinpthread-1.dll"
  )

  for j in "${dll[@]}"
  do
    findAndCopyDLL $j || echo "Could not find $j"
  done

  if [[ $RELEASE -eq 1 ]];
  then
      cp demo/desktop/include/SFML-2.5.1-mingw64/bin/sfml-system-2.dll build/
      cp demo/desktop/include/SFML-2.5.1-mingw64/bin/sfml-window-2.dll build/
      cp demo/desktop/include/SFML-2.5.1-mingw64/bin/sfml-graphics-2.dll build/
  else
      cp demo/desktop/include/SFML-2.5.1-mingw64/bin/sfml-system-d-2.dll build/
      cp demo/desktop/include/SFML-2.5.1-mingw64/bin/sfml-window-d-2.dll build/
      cp demo/desktop/include/SFML-2.5.1-mingw64/bin/sfml-graphics-d-2.dll build/
  fi
elif [[ $OSX -eq 0 ]];
then
  cmake -E make_directory build
  cmake -E chdir build cmake .. -D OSX=ON -D BUILD_DEMOS=$DEMO -D RELEASE=$RELEASE -D TEST_SUITE=$TEST -D SYNTAX_ONLY=$SYNTAX -D SANITISE=$SANITISE -D CMAKE_TOOLCHAIN_FILE=./osx.cmake && make -j 8 -C build
elif [[ ! -z "$ANDROID_NDK" ]]
then
  TOOL_CHAIN="$ANDROID_NDK/build/cmake/android.toolchain.cmake"
  if [[ ! -f "$TOOL_CHAIN" ]]
  then
    echo -e "\nCould not find tool chain file at $TOOL_CHAIN\n"
    exit 1
  fi
  echo -e "\nFound Android tool chain $TOOL_CHAIN\n"
  mkdir build

  buildAndroid arm64-v8a
  buildAndroid armeabi-v7a
  buildAndroid x86
  buildAndroid x86_64

else
  cmake -E make_directory build
  cmake -E chdir build cmake -D BUILD_DEMOS=$DEMO -D RELEASE=$RELEASE -D TEST_SUITE=$TEST -D SANITISE=$SANITISE -D SYNTAX_ONLY=$SYNTAX .. && make -j 8 -C build
fi


if [[ -z "$ANDROID_NDK" ]]
then 
  mergeLibs "build"
fi
