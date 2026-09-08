SRCS := $(shell find . -name '*.c')
OBJS := $(patsubst ./%.c,./%.o,$(SRCS))

abs_syn:  y.tab.o lex.yy.o $(OBJS)
	cc -o abs_syn -g y.tab.o lex.yy.o $(OBJS) 

y.tab.o: y.tab.c
	cc -g -c y.tab.c

y.tab.c: tiger.grm
	yacc -dv tiger.grm

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

errormsg.o: errormsg.c errormsg.h util.h
	cc -g -c errormsg.c

lex.yy.o: lex.yy.c y.tab.h errormsg.h util.h
	cc -g -c lex.yy.c

lex.yy.c: tiger.lex
	lex tiger.lex

clean: 
	rm -f abs_syn *.o y.tab.c y.tab.h lex.yy.c

