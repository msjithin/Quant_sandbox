# Makefile

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin
LD_DIR =  /usr/local/lib 

# Compiler and compiler flags
CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++20 -Wall $(DEBUG)
LDFLAGS = -L./lib 

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Executable name
TARGET = $(BIN_DIR)/main

# Include directories
INC_DIRS = -I$(INC_DIR)

# Build rules
all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean




###################################################################################################
##########                                     NOTES                                 ##############
###################################################################################################
# Advantages Of Makefiles
# 	When it comes to big projects, then using makefiles helps us to represent the project in a systematic and efficient way.
# 	Makefiles make source code more concise and easy to read and debug.
# 	Makefiles automatically compile only those files that are changed. Thus we need not regenerate the entire project when some of the portions of the project are modified.
# 	Make tool allows us to compile multiple files at once so that all the files can be compiled in a single step.

# Example
# # *****************************************************
# # Variables to control Makefile operation
# CC = g++
# CFLAGS = -Wall -g
# 
# # ****************************************************
# # Targets needed to bring the executable up to date
# main: main.o Point.o Square.o
#     $(CC) $(CFLAGS) -o main main.o Point.o Square.o
# # The main.o target can be written more simply
# main.o: main.cpp Point.h Square.h
#     $(CC) $(CFLAGS) -c main.cpp
# Point.o: Point.h
# Square.o: Square.h Point.h


#########################   OPTIONS    ####################################
# Most Common
# These are generally most important options for normal use on your VM and in your assignment Makefiles. You’ll probably end up knowing them by heart.

# -Wall 			: show all warnings. It turns on all standard C++ warnings about code that might cause unexpected or undefined behavior.
# -llibrary-name 	: (Link option) Link in the specified library. See above. (Link option).
# -g 				: provides debugging feature for your program. You will need this when you want to use gdb or valgrind.
# --std=c++<##> 	: uses version <##> of C++ when compiling. This will allow you to use specific features of that C++ version. Typically, we have you use --std=c++17.
# -o 				: <filename> compiles and links files into an executable named <filename>. The default filename is a.out.
# -c 				: compiles and assembles files but doesn’t link them. This is useful when building large projects to separate file compilation and minimize what is re-compiled.

# Bonus Round
# Here are a couple compiler flags that we don’t as often use in this class. They’re still pretty handy to know. You can read about all the options for using g++ here.

# --sys-root=<directory> 	: uses <directory> as root directory for headers and libraries.
# -I /<absolute-path> 		: adds <absolute-path> to the compiler’s search paths. The path must written from the root of the filesystem, /.
# -Werror 					: make all warnings into errors do this before submitting your code to avoid getting points deducted!
# -pedantic 				: issues all warnings demanded by strict ISO C++ rules if you want to be extra safe
# -Wextra 					: enables some extra warnings not turned on by -Wall. These include warnings for bad pointer to integer zero comparisons, base class not initialized in copy constructor of derived class, etc.
# -Wfatal-errors 			: is similar to -Wall but treats an error as fatal and stops before dumping a long list of errors into the terminal.
# -fmax-errors=n 			: tells the compiler to stop after encountering n errors in your code. Usually you want to see all errors in your code some error messages may get too long for you to read.
# -Wshadow 					: warns whenever a local variable or type declaration shadows another variable, parameter, class member, etc.
# -Wsign-conversion 		: or -Wconversion warn if making unsafe, implicit conversions between signed and unsigned types (i.e. unsigned int + (constant expression) )
# -O2 						: optimize code and check for errors in parallel to find potential bugs. (read more about this here)
# --help 					: gets help on specific options for the compiler.
