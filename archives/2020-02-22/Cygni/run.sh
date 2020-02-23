#!/bin/bash

clear
cd build/
cmake ..
status=$?
if test $status -eq 0
then
	make
	status=$?
	if test $status -eq 0
	then
		./Cygni
	fi
fi
cd ..
read -p 'Press [Enter] key to continue...'
