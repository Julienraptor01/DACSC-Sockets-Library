### variables
# library name
LIB_NAME=sockets

### paths
# sources
SRC=./src
# sources sub directories
MAIN=/main
TEST=/test
# sources -> (*)
MAIN_SRC=$(SRC)$(MAIN)
TEST_SRC=$(SRC)$(TEST)
# build
BUILD=./build
# build sub directories
OBJS=/obj
BINS=/bin
# build -> objs -> (*)
OBJ=$(BUILD)$(OBJS)
MAIN_OBJ=$(OBJ)$(MAIN)
TEST_OBJ=$(OBJ)$(TEST)
# build -> bins -> (*)
BIN=$(BUILD)$(BINS)
MAIN_BIN=$(BIN)$(MAIN)
TEST_BIN=$(BIN)$(TEST)
# logs
LOGS=./logs

### lists
# use a generic rule to list all the headers
MAIN_HEADERS=$(wildcard $(MAIN_SRC)/*.h)
TEST_HEADERS=$(wildcard $(TEST_SRC)/*.h)
# use a generic rule to list all the objects
MAIN_OBJECTS=$(patsubst $(MAIN_SRC)/%.cpp,$(MAIN_OBJ)/%.o,$(wildcard $(MAIN_SRC)/*.cpp))
TEST_OBJECTS=$(patsubst $(TEST_SRC)/%.cpp,$(TEST_OBJ)/%.o,$(wildcard $(TEST_SRC)/*.cpp))

### commands
# compiler
CXX=g++
# compiler flags
ifdef DEBUG
CXXFLAGS=-std=gnu++23 -Wall -Wextra -g -Og -save-temps
else
CXXFLAGS=-std=gnu++23 -Wall -Wextra -O3 -s
endif
MAIN_CXXFLAGS=$(CXXFLAGS)
TEST_CXXFLAGS=$(CXXFLAGS)
CPPFLAGS=
MAIN_CPPFLAGS=$(CPPFLAGS) -I$(MAIN_SRC)
TEST_CPPFLAGS=$(CPPFLAGS) -I$(TEST_SRC) -I$(MAIN_SRC)
# compilation
MAIN_COMPILE=$(CXX) $(MAIN_CXXFLAGS) $(MAIN_CPPFLAGS) -c $< -o $@
TEST_COMPILE=$(CXX) $(TEST_CXXFLAGS) $(TEST_CPPFLAGS) -c $< -o $@
# archiver
AR=ar
# archiver flags
ARFLAGS=rcs
# archiving
MAIN_ARCHIVE=$(AR) $(ARFLAGS) $@ $^
# linker flags
LDFLAGS=
MAIN_LDFLAGS=$(LDFLAGS)
TEST_LDFLAGS=$(LDFLAGS) -Wl,-Bstatic -L$(MAIN_BIN) -l$(LIB_NAME) -Wl,-Bdynamic
# linking
MAIN_LINK=$(CXX) $^ -o $@ $(MAIN_LDFLAGS)
TEST_LINK=$(CXX) $^ -o $@ $(TEST_LDFLAGS)
# deletion
RM=rm -rf
# silent log
LOG=@printf

.PHONY: help all build run setup clean full-clean
.SECONDARY: $(OBJECTS)

help:
	$(LOG) 'Usage : make [target]\n'
	$(LOG) '\tall : setup and build the whole project\n'
	$(LOG) '\tbuild : build the static library\n'
	$(LOG) '\tbuild-test : build the test executable\n'
	$(LOG) '\trun-test : run the test executable\n'
	$(LOG) '\tsetup : setup the project directories\n'
	$(LOG) '\tclean : delete all the object files\n'
	$(LOG) '\tfull-clean : delete all the build directories\n'

all: setup build-test

$(MAIN_OBJ)/%.o: $(MAIN_SRC)/%.cpp $(MAIN_HEADERS)
	$(LOG) '\n\033[42mcreation of the $* object file\033[49m\n'
	$(MAIN_COMPILE)

$(MAIN_BIN)/lib%.a: $(MAIN_OBJECTS)
	$(LOG) '\n\033[42mcreation of the $* static binary\033[49m\n'
	$(MAIN_ARCHIVE)

$(MAIN_BIN)/%: $(MAIN_OBJ)/%.o $(MAIN_OBJECTS)
	$(LOG) '\n\033[42mcreation of the $* binary\033[49m\n'
	$(MAIN_LINK)

$(TEST_OBJ)/%.o: $(TEST_SRC)/%.cpp $(TEST_HEADERS) $(MAIN_HEADERS)
	$(LOG) '\n\033[42mcreation of the $* object file\033[49m\n'
	$(TEST_COMPILE)

$(TEST_BIN)/%: $(TEST_OBJ)/%.o $(TEST_OBJECTS)
	$(LOG) '\n\033[42mcreation of the $* binary\033[49m\n'
	$(TEST_LINK)

build:
	$(LOG) '\n\033[42mbuilding the lib$(LIB_NAME) static library\033[49m\n'
	make $(MAIN_BIN)/lib$(LIB_NAME).a
	$(LOG) '\n\033[44mmake build finished\033[49m\n\n'

build-test: build
	$(LOG) '\n\033[42mbuilding the test executable\033[49m\n'
	make $(TEST_BIN)/test
	$(LOG) '\n\033[44mmake build-test finished\033[49m\n\n'

run-test: build-test
	$(LOG) '\n\033[42mrunning the test binary\033[49m\n'
	tmux new-session -d \; \
	split-window -h \; \
	send-keys -t 0 'sleep 1 && clear' C-m \; \
	send-keys -t 1 'sleep 2 && clear' C-m \; \
	send-keys -t 0 '$(TEST_BIN)/test server 127.0.0.1' C-m \; \
	send-keys -t 1 '$(TEST_BIN)/test client localhost' C-m \; \
	attach
	$(LOG) '\n\033[44mmake run-test finished\033[49m\n\n'

setup:
	$(LOG) '\n\033[42msetup of the directories\033[49m\n'
	mkdir \
	-p \
	$(MAIN_OBJ) \
	$(TEST_OBJ) \
	$(MAIN_BIN) \
	$(TEST_BIN) \
	$(LOGS)
	$(LOG) '\n\033[44mmake setup finished\033[49m\n\n'

clean:
	$(LOG) '\n\033[41mdeletion of the object directory\033[49m\n'
	$(RM) $(OBJ)/*
	make setup
	$(LOG) '\n\033[44mmake clean finished\033[49m\n\n'

full-clean:
	$(LOG) '\n\033[41mdeletion of ALL the build directories\033[49m\n'
	$(RM) $(BUILD)/*
	make setup
	$(LOG) '\n\033[44mmake full-clean finished\033[49m\n\n'
