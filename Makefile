# -----------------------------------------------------------------------------
# Copyright (C) 2022, Hoge Aerospace Institute
# This software is the confidential and proprietary information of the
# Hoge Aerospace Institute.
#
# ALL RIGHTS RESERVED
#
# Permission is hereby granted to licensees of Hoge Aerospace Institute
# products to use or abstract this computer program for the sole purpose of
# implementing a product based on Hoge Aerospace Institute products.  No
# other rights to reproduce, use, or disseminate this computer program,
# whether in part or in whole, are granted.
#
# Hoge Aerospace Institute makes no representation or warranties with respect
# to the performance of this computer program, and specifically disclaims any
# responsibility for any damages, special or consequential, connected with
# the use of this program.
# -----------------------------------------------------------------------------

.PHONY: predict hamlib sdrpp debian-package

all: hamlib predict sdrpp

hamlib:
	@echo "Making hamlib..."
	cd hamlib; ./bootstrap; ./configure; make

predict:
	@echo "Making predict..."
	cd predict; ./build

sdrpp:
	@echo "Making sdrpp..."
	cd sdrpp; mkdir build; cd build; cmake ..; make
	
debian-package:
	@echo "Making debian package..."
	dpkg-buildpackage -b

clean:
	@echo "Make clean"
	cd hamlib; make clean
	cd predict; make clean
	cd sdrpp; cd build; make clean
