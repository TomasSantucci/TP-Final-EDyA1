TARGET  = proyecto

CC 	    = gcc
CFLAGS  = -Wall -Wextra -Werror -g -std=c99

SRC_DIR = src
HEADER_DIR = head
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c) 		
HEADERS = $(wildcard $(HEADER_DIR)/*.h)			
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

$(TARGET): $(OBJECTS)
	@echo "Generando archivo principal..."
	$(CC) -o $(BIN_DIR)/$@ $^

$(OBJECTS): $(BIN_DIR)/%.o: $(SRC_DIR)/%.c

	@echo "Generando archivo $@..."
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(BIN_DIR)/$(TARGET)
	@echo "Corriendo test..."
	valgrind --tool=memcheck $<

style: $(SOURCES) $(HEADERS)
	@echo "Estilizando codigo..."
	indent -kr -brf -i2 -l80 -nut $^ 

clean:
	@echo "Borrando los archivos generados..."
	rm -rvf $(BIN_DIR) 	

.PHONY: test	