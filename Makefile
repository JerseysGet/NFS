CC = gcc
CVERSION = -std=c2x

COMMON_C_FLAGS = -Wall
COMMON_C_FLAGS += -Wextra
COMMON_C_FLAGS += -Wshadow
COMMON_C_FLAGS += -Wduplicated-cond
COMMON_C_FLAGS += -Wlogical-op
COMMON_C_FLAGS += -Wfloat-equal
COMMON_C_FLAGS += -Wno-format-overflow

DEBUG_FLAGS = -fsanitize=address,undefined,pointer-compare
DEBUG_FLAGS += -ggdb3
DEBUG_FLAGS += -ftrapv
DEBUG_FLAGS += -fstack-protector
DEBUG_FLAGS += -DDebug

BUILD_FLAGS = -O2

COMMON_OBJECTS = common/error/error.o common/networking/networking.o common/print/print.o

naming_server: naming_server.out
	@./naming_server.out

naming_server.out: $(COMMON_OBJECTS)
	$(CC) $(COMMON_C_FLAGS) $(DEBUG_FLAGS) $(BUILD_FLAGS) $(COMMON_OBJECTS) naming_server/naming_server.c -o $@

%.o:%.c
	@$(CC) $(COMMON_C_FLAGS) $(DEBUG_FLAGS) $(BUILD_FLAGS) -c $^ -o $@

clean:
	@find . -type f -iname \*.out -delete
	@find . -type f -iname \*.o -delete
	@find . -type f -iname \*.test -delete