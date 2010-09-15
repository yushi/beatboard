#!/bin/bash

autoheader

if [ `uname` = "Darwin" ];then
  glibtoolize
else
  libtoolize
fi

aclocal
automake --add-missing --copy
autoconf
