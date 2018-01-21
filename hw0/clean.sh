#!/usr/bin/env bash
binary="./homework_0"
folder="./build"

if [ -f $binary ] ; then
  rm $binary
fi

if [ -d $folder ] ; then
  rm -r $folder
fi
