#!/bin/bash

clear
cd build/
cmake ..
status=$?
if test $status -eq 0
then
	make
fi
cd ..
