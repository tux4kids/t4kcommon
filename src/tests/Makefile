CC = gcc
LFLAGS = -g -W -Wall -Wmissing-declarations -Wmissing-prototypes -Wredundant-decls -Wshadow -Wbad-function-cast -Wcast-qual
CFLAGS = -g -W -Wall -Wmissing-declarations -Wmissing-prototypes -Wredundant-decls -Wshadow -Wbad-function-cast -Wcast-qual
SRC = main.c test_public_functions.c
OBJ = $(SRC:.c=.o)
EXEC = unittest
CUNITINC = $(CURDIR)/CUnit/include
CUNITLIB = $(CURDIR)/CUnit/lib
T4KCOMMONINC = $(CURDIR)/..
LIBS = -lSDL -lSDL_ttf -lSDL_Pango -lt4k_common -L$(CUNITLIB) -lcunit
INC = -I$(CUNITINC) `pkg-config --cflags sdl` -I$(T4KCOMMONINC)



all : $(EXEC)

unittest : $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LFLAGS)
main.o : main.c
	$(CC) $(INC) $(CFLAGS) -c $<
test_public_functions.o : test_public_functions.c
	$(CC) $(INC) $(CFLAGS) -c $<

clean :
	@ rm -f $(OBJ) *~
mrproper : clean
	@ rm -f $(EXEC)
