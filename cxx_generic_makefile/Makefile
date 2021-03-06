# set suffix list, to prevent confusion between different make programs
# line 17 clears an implied suffix list, and line 18 sets a new one
.SUFFIXES:
.SUFFIXES: .cpp .hpp .h .o

# project set up and directories
INCLDIR	= include/
BINDIR	= bin/
OBJDIR	= obj/
SRCDIR	= src/

# final executable name
_BIN    = app
BIN	    = $(addprefix $(BINDIR), $(_BIN))

# files; here all object files will be stored in $(OBJDIR), with the
# same base names as corresponding c files from SRCDIR
SRC		= $(wildcard src/*.cpp)
_OBJS	= $(patsubst src/%.cpp, %.o, $(SRC))
OBJS	= $(addprefix $(OBJDIR), $(_OBJS))

# compilation flags
CXXFLAGS 	= -std=c++17 -march=native -Werror -Wall -Wextra -Wpedantic -I$(INCLDIR) -I$(SRCDIR)
LDFLAGS     =

ifdef DEBUG
	CXXFLAGS 	+= -Og -g3 -ggdb
else
	CXXFLAGS 	+= -O3 -flto
	LDFLAGS     += -flto
endif 



# compile binary and object files
.PHONY: all
all: $(BIN)

$(BIN): $(BINDIR) $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(BIN)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJS): $(OBJDIR) $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# clean entire project directory
.PHONY: clean
clean:
	- rm -rf $(BINDIR) $(OBJDIR) $(DOCDIR)

# check code quality
.PHONY: cppcheck memcheck
cppcheck:
	cppcheck --enable=all --language=c++ --std=c++17 --inconclusive \
	--suppress=missingInclude $(SRC) -i $(INCLDIR)

memcheck: all
	valgrind -v --show-leak-kinds=all --leak-check=full --track-origins=yes	./$(BIN)

# debugging make
print-% :
	@echo $* = $($*)
