#!/bin/bash
#This script will install the pre-packaged release of AOSSL

if [ "$#" -ne 1 ]; then
  printf "Attempting to install dependencies"

  #Install the dependencies
  printf "This will install multiple libraries to your system, of various licenses including LGPL.  This may pull development versions,"
  printf "and it is therefore inadvisable to use in Production setups."
  printf "Please be sure to review the dependency page found at http://clyman.readthedocs.io/en/latest/pages/deps.html"
  printf "You may find the source code for this application, and the licenses for all dependencies at https://github.com/AO-StreetArt/AOSharedServiceLibrary"
  printf "Do you wish to continue? [y/n]:"
  read deps_confirm
  if [deps_confirm = 'y']; then
    #Install the dependencies
    cd ./deps && sudo ./build_deps.sh

    printf "Attempting to install core library"

    # Install the library
    cd ../ && sudo make -f install

    printf "Finished installing AOSSL"
  fi

else
  OPT=$1
  if [ $OPT = "-r" ]
  then
    printf "Attempting to uninstall core library"
    sudo make uninstall
    printf "Finished uninstalling AOSSL"
  fi
fi
