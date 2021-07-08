CC=g++
EXE=main
FinalName=hhx
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
CXXFLAGS=-g -Wno-deprecated -DDEBUG	#传入参数DEBUG
all:depend $(EXE)
depend:
	@$(CC) -MM $(SRC) > .depend
-include .depend
$(EXE):$(OBJ)
	$(CC) $(OBJ) -o $(FinalName) 
clean:
	@rm $(FinalName) $(OBJ) .depend

