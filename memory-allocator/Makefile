COMPILER_FLAGS=--std=c17 -Wall -pedantic -Isrc/ -ggdb -Werror -Wextra -DDEBUG
BUILD_DIR=build
SRC_DIR=src
C_COMPILER=gcc

all: $(BUILD_DIR)/mem.o $(BUILD_DIR)/util.o $(BUILD_DIR)/mem_debug.o $(BUILD_DIR)/test.o $(BUILD_DIR)/main.o
	$(C_COMPILER) -o $(BUILD_DIR)/main $^

build:
	mkdir -p $(BUILD_DIR)


$(BUILD_DIR)/mem.o: $(SRC_DIR)/mem.c $(BUILD_DIR)
	$(C_COMPILER) -c $(COMPILER_FLAGS) $< -o $@

$(BUILD_DIR)/mem_debug.o: $(SRC_DIR)/mem_debug.c $(BUILD_DIR)
	$(C_COMPILER) -c $(COMPILER_FLAGS) $< -o $@

$(BUILD_DIR)/util.o: $(SRC_DIR)/util.c $(BUILD_DIR)
	$(C_COMPILER) -c $(COMPILER_FLAGS) $< -o $@

$(BUILD_DIR)/test.o: $(SRC_DIR)/test.c $(BUILD_DIR)
	$(C_COMPILER) -c $(COMPILER_FLAGS) $< -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(BUILD_DIR)
	$(C_COMPILER) -c $(COMPILER_FLAGS) $< -o $@


clean:
	rm -rf $(BUILD_DIR)
