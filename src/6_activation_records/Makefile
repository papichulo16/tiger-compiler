INTF_DIR := intf/
BUILD_DIR := build/

SRCS := $(shell find . -name '*.c')
OBJS := $(patsubst ./%.c,$(BUILD_DIR)/%.o,$(SRCS))

CFLAGS := -I $(INTF_DIR)
CC := gcc

sem_anal:  y.tab.o lex.yy.o $(OBJS)
	$(CC) -o sem_anal -g y.tab.o lex.yy.o $(OBJS) $(CFLAGS)
	mv *.c *.o *.h $(BUILD_DIR)

y.tab.o: y.tab.c
	$(CC) -g -c y.tab.c $(CFLAGS)

y.tab.c: tiger.grm
	yacc -dv tiger.grm 

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

lex.yy.o: lex.yy.c y.tab.h $(INTF_DIR)errormsg.h $(INTF_DIR)util.h
	$(CC) -g -c lex.yy.c $(CFLAGS)

lex.yy.c: tiger.lex
	lex tiger.lex

$(BUILD_DIR)/%.o: ./%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf sem_anal build *.o y.tab.c y.tab.h lex.yy.c out y.output

