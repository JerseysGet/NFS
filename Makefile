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
DEBUG_FLAGS += -DDEBUG

BUILD_FLAGS = -O0

COMMON_OBJECTS = common/error/error.o 
COMMON_OBJECTS += common/networking/networking.o 
COMMON_OBJECTS += common/print/print.o 
COMMON_OBJECTS += common/print/logging.o
COMMON_OBJECTS += common/networking/nm_ss/ss_connect.o
COMMON_OBJECTS += common/networking/nm_client/client_connect.o
COMMON_OBJECTS += common/signals/cleanup_signal.o
COMMON_OBJECTS += common/networking/ack/ssinfo.o

NM_OBJECTS = naming_server/naming_server.o
NM_OBJECTS += naming_server/threads/for_ss/ss_listener_thread.o
NM_OBJECTS += naming_server/threads/for_ss/ss_alive_thread.o
NM_OBJECTS += naming_server/threads/for_client/client_listener_thread.o
NM_OBJECTS += naming_server/threads/for_client/client_alive_thread.o
NM_OBJECTS += naming_server/trie/trie.o

SS_OBJECTS = storage_server/storage_server.c

CLIENT_OBJECTS = client/client.c

nm: naming_server.out
	./naming_server.out

ss: storage_server.out
	./storage_server.out

client: client.out
	./client.out


naming_server.out: $(COMMON_OBJECTS) $(NM_OBJECTS)
	@$(CC) $(COMMON_C_FLAGS) $(DEBUG_FLAGS) $(BUILD_FLAGS) $(COMMON_OBJECTS) $(NM_OBJECTS) naming_server/main.c -o $@

storage_server.out: $(COMMON_OBJECTS) $(SS_OBJECTS)
	@$(CC) $(COMMON_C_FLAGS) $(DEBUG_FLAGS) $(BUILD_FLAGS) $(COMMON_OBJECTS) $(SS_OBJECTS) storage_server/main.c -o $@

client.out: $(COMMON_OBJECTS) $(CLIENT_OBJECTS)
	@$(CC) $(COMMON_C_FLAGS) $(DEBUG_FLAGS) $(BUILD_FLAGS) $(COMMON_OBJECTS) $(CLIENT_OBJECTS) client/main.c -o $@

%.o:%.c
	@$(CC) $(COMMON_C_FLAGS) $(DEBUG_FLAGS) $(BUILD_FLAGS) -c $^ -o $@

clean_logs:
	@find . -type f -iname \*.log -delete

clean:
	@find . -type f -iname \*.out -delete
	@find . -type f -iname \*.o -delete
	@find . -type f -iname \*.test -delete