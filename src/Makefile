


# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = /usr/X11/include
X11LIB = /usr/X11/lib

# freetype
FREETYPELIBS = -lfontconfig -lXft

# includes and libs
INCS = -I${X11INC} -I${FREETYPEINC}
LIBS = -L${X11LIB} -lX11 ${FREETYPELIBS} -lXrandr -lslankdev

# flags
CXXFLAGS =  -std=c++11
CXXFLAGS += -pedantic
CXXFLAGS += -Wall -Wextra
CXXFLAGS += ${INCS} -g -O0
LDFLAGS  =  ${LIBS}

# compiler and linker
CXX = g++


SRC = main.cc
OBJ = $(SRC:.cc=.o)

all: wm
re: clean all

.cc.o:
	@echo CXX $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ}: config.h

wm: $(OBJ)
	$(CXX) $(OBJ) -o wm 

clean:
	@echo cleaning
	@rm -f wm ${OBJ} 

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f wm ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wm

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/wm

.PHONY: all clean install uninstall

