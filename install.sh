#!/bin/bash

head="/usr/local/include/dusk.hpp"
shared="/usr/local/lib/libdusk.so"

touch $head
cat "main/internal/dprop.hpp" >> $head
cat "main/external/dbase.hpp" >> $head
cat "main/external/dutil.hpp" >> $head

ln -fs $(pwd)/libdusk.so $shared
