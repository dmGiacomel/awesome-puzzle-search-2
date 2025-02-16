# Compiler
CXX := g++
# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra
# Include directories
INCLUDES := -I./Utils -I./Heuristics -I./Algorithms

# List of test files (modify/add as needed)
TEST_FILES := $(wildcard *.cpp)
# List of object files to be generated
OBJECTS := $(TEST_FILES:.cpp=.o)

# Main target (default target)
all: $(OBJECTS)

# Compile rule for each test file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $