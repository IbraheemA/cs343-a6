# VIMPL:=EXT
# OUTPUT:=OUTPUT
# BCHECK:=NOBARGINGCHECK

CXX = u++					# compiler
CXXFLAGS = -g -multi -O2 -Wall -Wextra -MMD # -D"${BCHECK}" # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = nameserver.o main.o bank.o config.o convert.o groupoff.o parent.o printer.o student.o truck.o vendingmachine.o watcard.o watcardoffice.o bottlingplant.o
EXEC = soda

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean

all : ${EXEC}					# build all executables

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

# -include SodaImpl

# ifeq (${shell if [ "${LOCKVIMPL}" = "${VIMPL}" -a "${OUTPUTTYPE}" = "${OUTPUT}" -a \
# 			"${BCHECKIMPL}" = "${BCHECK}" ] ; then echo true ; fi },true)
# ${EXEC} : ${OBJECTS}
# 	${CXX} ${CXXFLAGS} $^ -o $@
# else						# implementation type has changed => rebuilt
# .PHONY : ${EXEC}
# ${EXEC} :
# 	rm -f SodaImpl
# 	touch q2tallyVotes.h
# 	${MAKE} ${EXEC} VIMPL="${VIMPL}" OUTPUT="${OUTPUT}" BCHECK="${BCHECK}"
# endif

# SodaImpl :
# 	echo "LOCKVIMPL=${VIMPL}\nOUTPUTTYPE=${OUTPUT}\nBCHECKIMPL=${BCHECK}" > SodaImpl
# 	sleep 1

# #############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d ${OBJECTS} ${EXEC} 
# SodaImpl # in line above
