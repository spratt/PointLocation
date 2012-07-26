###############################################################################
#                       Copyright (c) 2011 - 2012 by                          #
#                               Simon Pratt                                   #
#                         (All rights reserved)                               #
###############################################################################
# Makefile for the Persistent SkipList project                                #
###############################################################################

# don't use built in targets
makepp_no_builtin = 1

###############################################################################
# Quiet Compile                                                               #
###############################################################################
QUIET_CC	=
# comment next line for verbose compile
QUIET_CC	= @echo '    ' CC $@;

%.o: %.cpp
	$(QUIET_CC)$(COMPILE.cpp) $(OUTPUT_OPTION) $<

%: %.cpp
	$(QUIET_CC)$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

###############################################################################
# Global options                                                              #
###############################################################################

CXX = g++

VALGRIND	= valgrind
VGOPS		= --leak-check=full -v --show-reachable=yes

LEDAROOT = /home2bak/spratt/Projects/PrioritySearchTree/LEDA/6.3-x64
LEDAFLAGS= -L$(LEDAROOT) -I$(LEDAROOT)/incl -lleda -lX11 -lm

# if mode is release, don't include debug info
ifeq ($(mode),release)
	CXXFLAGS=-O2 -Wall -DNDEBUG $(LEDAFLAGS)
else
	mode = debug
	CXXFLAGS=-g -std=c++98 -pedantic-errors -Wall -Werror $(LEDAFLAGS)
endif

BAR = "======================================================================"

###############################################################################
# Project specific options                                                    #
###############################################################################

DATA_DIR	= /home2bak/spratt/Projects/CGAL_PointLocation

TEST_DIR	= test

TEST_LS		= ${TEST_DIR}/test_linesegments

TEST_PS		= ${TEST_DIR}/test_polygonal_subdivision

TESTS	 	= ${TEST_LS}

.PHONY:	all run run_tests_mac run_tests clean lines get_libs

.IGNORE: lines

.SUFFIXES: .o .cpp .hpp

.SILENT: run_tests run_tests_mac

#begin actual makefile stuff
tests: ${TESTS} ${TEST_PS}

all: get_libs tests

get_libs:
	cd lib;./get_libs.sh

run: run_tests_mac run_test_ps

test: run_tests_mac run_tests

run_tests_mac:	${TESTS}
	${foreach test,${TESTS},\
echo ${BAR};echo "| " ${test};echo ${BAR};\
cat ${test}_input | ./${test};}

run_tests:	${TESTS}
	${foreach test,${TESTS},\
echo ${BAR};echo ${test};echo ${BAR};\
cat ${test}_input | ${VALGRIND} ${VGOPS} ./${test};}

run_test_ps:	${TEST_PS}
	@echo ${BAR}
	@echo run_test_ps
	@echo ${BAR};
	cp ${DATA_DIR}/points.txt ${TEST_DIR}/gen_points.txt
	test/convert_points.py test/gen_points.txt > test/points.txt
	cp ${DATA_DIR}/segments.txt ${TEST_DIR}/segments.txt
	cp ${DATA_DIR}/naive.txt ${TEST_DIR}/naive.txt
	time ${TEST_DIR}/test_polygonal_subdivision ${TEST_DIR}/segments.txt ${TEST_DIR}/points.txt > ${TEST_DIR}/psl.txt
	diff ${TEST_DIR}/naive.txt ${TEST_DIR}/psl.txt > ${TEST_DIR}/diff.txt
	less ${TEST_DIR}/diff.txt

# specify required libraries
${TEST_LS}:	Point2D.o LineSegment.o lib/PersistentSkipList/PersistentSkipList.o

${TEST_PS}: 	Point2D.o LineSegment.o PolygonalSubdivision.o \
		lib/PersistentSkipList/PersistentSkipList.o \
		lib/CppLog/CppLog.o

# tidy up generated files
clean:
	@rm -f ${TESTS}
	@rm -f *.o *.log core
	@rm -rf *.dSYM

###############################################################################
# Miscellaneous                                                               #
###############################################################################

lines:
	@wc -l *.c *.h *.hpp *.cpp 2> /dev/null || \
	sloccount *.cpp *.c *.h 2> /dev/null | grep Physical || \
	cloc --by-file *.cpp *.c *.hpp *.h 2> /dev/null