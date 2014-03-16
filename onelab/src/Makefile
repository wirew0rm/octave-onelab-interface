# Makefile for the octave onelab package.
# based on the makefile of the ocatve-forge sockets package
# The intent is to keep this makefile working for
# * Octave>=3.2
# * Windows
# straight out of the box.

OCT := onelab.oct
SRC := $(OCT:.oct=.cc)

MKOCTFILE ?= mkoctfile -Wall
OCTAVE ?= octave

EXTRALIBS := -lGmsh

all: $(OCT)

%.oct: %.cc
	$(MKOCTFILE) $< $(EXTRALIBS)

# We need to generate a PKG_ADD file for this to work. We could also
# get a string with the stuff in PKG_ADD and eval it before the test
# command, but it messes up the quotes.
check: $(OCT)
	grep PKG_ADD $(SRC) | sed 's/\/\/ PKG_ADD: //' > PKG_ADD
	$(OCTAVE) -q --no-window-system --eval "test $(SRC)"
	rm PKG_ADD

clean:
	rm -f *.o octave-core octave-workspace core *.oct *~ PKG_ADD
 
#This creates a package for distribution. It requires the checked out hg
#repo to be named sockets, or that a symlink of name sockets point to the
#checked out repo.
pkgversion:=$(word 2,$(shell grep ^Version ../DESCRIPTION|head -n1))
pkg:
	cd ../.. && tar cvfzh onelab-${pkgversion}.tar.gz --exclude=.hg onelab/

.PHONY: all clean check pkg 

# old command for backup purposes
#		mkoctfile -v -lGmsh -I/usr/include/gmsh	$<
