EXECUTABLE = rbtree

SRC = src
INC = include

SOURCE = $(wildcard $(SRC)/*.c)
	
OBJECT = $(patsubst %,$(SRC)/%, $(notdir $(SOURCE:.c=.o)))

CC = gcc
CFLAGS = -Wall -c -g -I$(INC)


$(EXECUTABLE) : $(OBJECT)
	@echo "$ Compiling...$(NC)"
	$(CC) -o $@ $^
	
$(SRC)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean

clean:
	@echo " Cleaning . . ."
	rm -f $(OBJECT) $(EXECUTABLE)
