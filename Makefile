CC=gcc
CFLAGS=-I. -Wall
DEPS=src/headers/fileUtils.h src/headers/choiceUtils.h
OBJ=dist/fileUtils.o dist/main.o dist/choiceUtils.o

ifeq ($(OS),Windows_NT)
    EXE = .exe
    MKDIR = if not exist dist mkdir dist
    RM = del /Q
else
    EXE =
    MKDIR = mkdir -p dist
    RM = rm -f
endif

.PHONY: all clean create_dir

all: create_dir copasMutiple copasMutipleForTest

create_dir:
	$(MKDIR)

copasMutiple: $(OBJ)
	$(CC) -o dist/copasMutiple$(EXE) $^

copasMutipleForTest: $(OBJ)
	$(CC) -o test/copasMutipleTest$(EXE) $^

dist/main.o: main.c $(DEPS)
	$(CC) -c -o $@ main.c $(CFLAGS)

dist/fileUtils.o: src/utils/fileUtils.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dist/choiceUtils.o: src/utils/choiceUtils.c src/headers/choiceUtils.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) dist/*$(EXE) dist/*.o test/*$(EXE)
