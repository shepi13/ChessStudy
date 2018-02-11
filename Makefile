CC          := g++
LD			:= g++
TARGET      := mychess

CFLAGS      := -std=c++11 -MD -MP
LDFLAGS     := 
INC 		:= -Iinclude
SRC     	:= $(shell find src -type f -name *.cpp)
OBJ     	:= $(patsubst src/%,tmp/%,$(SRC:.cpp=.o))
DEP 		:= $(SRC:.cpp=.d)

TARGET_TEST := unittests
CFLAGS_TEST := $(CFLAGS)
LDFLAGS_TEST:= $(LDFLAGS) -pthread -lgtest_main -lgtest
INC_TEST 	:= $(INC)
SRC_TEST 	:= $(shell find test -type f -name *.cpp)
OBJ_TEST 	:= $(filter-out tmp/%main.o, $(OBJ)) $(patsubst test/%,tmp/test/%,$(SRC_TEST:.cpp=.o))

all: $(TARGET)

test: $(TARGET_TEST)

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(TARGET_TEST): $(OBJ_TEST)
	$(LD) $(LDFLAGS_TEST) -o $@ $^

tmp/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

tmp/test/%.o: test/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS_TEST) $(INC_TEST) -c -o $@ $<

clean:
	@$(RM) -rf tmp/*
	@$(RM) -f $(TARGET)
	@$(RM) -f $(TARGET_TEST)

-include $(OBJ:.o=.d)
-include $(OBJ_TEST:.o=.d)

.PHONY: all test clean
