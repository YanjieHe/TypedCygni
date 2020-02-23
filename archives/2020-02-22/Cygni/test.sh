#!/bin/bash

clear
cd "test/cmake-build-test/"
cmake ..
status=$?
if test $status -eq 0
then
	make
	status=$?
	if test $status -eq 0
	then
		./Cygni_Test
	fi
fi
cd ..
cd ..
read -p 'Press [Enter] key to continue...'
