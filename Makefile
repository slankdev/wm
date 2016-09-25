

TARGET = wm

SRC = main.cc
OBJ = $(SRC:.cc=.o)

all: $(TARGET)


wm: $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) 
