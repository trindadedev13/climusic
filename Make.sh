#!/usr/bin/bash

# compile

mkdir build
cd build

cmake ..
make

cd ..
cp build/climusic .

# run it at termux
if [ "$1" = "-termux" ]; then
  cp climusic $HOME
  rm climusic

  cd $HOME
  chmod +x climusic
  ./climusic
fi