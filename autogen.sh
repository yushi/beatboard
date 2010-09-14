#!/bin/bash

autoheader
glibtoolize
aclocal
automake --add-missing --copy
autoconf
