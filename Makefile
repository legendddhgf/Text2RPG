#Isaak Cherdak <icherdak>
#August Valera <avalera>


# $Id: Makefile,v 1.24 2016-10-23 00:40:40-07 - - $

CPP       = g++ -g -O0 -Wall -Wextra -std=gnu++14

MODULES   = common ArgParser GameEngine

HDRSRC    = ${MODULES:=.h}
CPPSRC    = ${MODULES:=.cpp} main.cpp
EXECBIN   = Text2RPG
OBJECTS   = ${CPPSRC:.cpp=.o}

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CPP} -o ${EXECBIN} ${OBJECTS}

%.o : %.cpp
	${CPP} -c $<

ci : ${ALLSRC} ${TESTINS} checksource cleaner
	git pull
	git add --all :/
	git commit
	git push
	git log --graph --oneline -10

clean :
	- rm -f ${OBJECTS}

spotless : clean
	- rm -f ${EXECBIN}

cleaner : spotless

.PHONY : ci submit checksource cleaner
