#!/bin/bash
if [ -z ${USD_ROOT} ]; then
  echo '$USD_ROOT not set!'
else
  echo "Linking includes and libraries to point at USD_ROOT..."
  if [ -e Libraries/lib ]; then
    rm -rf Libraries/lib
  fi
  ln -s $USD_ROOT/lib Libraries/lib
  if [ -e Includes/include ]; then
    rm -rf Includes/include
  fi
  ln -s $USD_ROOT/include Includes/include
  for a in `python-config --cflags | tr ' ' '\n' | grep "\-I" | uniq`; do
    c=`echo $a | sed -e "s/-I//g"`
    for b in `ls $c`; do
      ln -s $c/$b Includes/include/$b
    done
  done
  echo "Done."
fi
