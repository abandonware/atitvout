#!/bin/bash

testit () {
	echo "TEST: Running 'atitvout $FLAG $1' ..."
	./atitvout $FLAG $1
	echo "TEST: ... finished. Return value is $?. Please wait 3 seconds."
	echo
	sleep 3
}

testall () {
	testit vbe
	testit tvout
	testit standard
	testit pal
	testit ntsc
	testit detect
	testit active
	testit auto
	testit off
	testit l
	testit c
	testit t
	testit lc
	testit lt
	testit ct
	testit lct
}


(
	FLAG=-f
	testall
	FLAG=-r
	testall
) 2>&1 | tee test.out
