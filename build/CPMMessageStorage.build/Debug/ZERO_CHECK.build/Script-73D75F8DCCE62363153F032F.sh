#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd "/Users/quentin/Desktop/CPM message storage/CPMLibrary/build"
  make -f /Users/quentin/Desktop/CPM\ message\ storage/CPMLibrary/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd "/Users/quentin/Desktop/CPM message storage/CPMLibrary/build"
  make -f /Users/quentin/Desktop/CPM\ message\ storage/CPMLibrary/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd "/Users/quentin/Desktop/CPM message storage/CPMLibrary/build"
  make -f /Users/quentin/Desktop/CPM\ message\ storage/CPMLibrary/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd "/Users/quentin/Desktop/CPM message storage/CPMLibrary/build"
  make -f /Users/quentin/Desktop/CPM\ message\ storage/CPMLibrary/build/CMakeScripts/ReRunCMake.make
fi

