#!/bin/bash
#This script will install the pre-packaged release of AOSSL

if [ "$#" -ne 1 ]; then
  printf "Attempting to install dependencies"

  #Install the dependencies
  cd ./deps && sudo ./build_deps.sh

  printf "Attempting to install core library"

  # Install the library
  cd ../ && sudo make -f make_install_release

  printf "Finished installing AOSSL"
else
  OPT=$1
  if [ $OPT = "-r" ]
  then
    printf "Attempting to uninstall core library"
    sudo make uninstall
    printf "Finished uninstalling AOSSL"
  fi
fi
