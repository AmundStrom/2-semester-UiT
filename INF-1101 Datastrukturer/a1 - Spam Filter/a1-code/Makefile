## Author: Steffen Viken Valvaag <steffenv@cs.uit.no> 
LIST_SRC=linkedlist.c
SET_SRC=tree.c   # Insert the file name of your set implementation here
SPAMFILTER_SRC=spamfilter.c common.c $(LIST_SRC) $(SET_SRC)
NUMBERS_SRC=numbers.c common.c $(LIST_SRC) $(SET_SRC)
ASSERT_SRC=assert_set.c common.c $(LIST_SRC) $(SET_SRC)

TIMESET_SRC=timeset.c common.c time.c $(LIST_SRC) $(SET_SRC) ## PERSONAL CODE

INCLUDE=include

NUMBERS_SRC:=$(patsubst %.c,src/%.c, $(NUMBERS_SRC))
SPAMFILTER_SRC:=$(patsubst %.c,src/%.c, $(SPAMFILTER_SRC))
ASSERT_SRC:=$(patsubst %.c,src/%.c, $(ASSERT_SRC))

TIMESET_SRC:=$(patsubst %.c,src/%.c, $(TIMESET_SRC)) ## PERSONAL CODE

CFLAGS=-Wall -Wextra -g -Wpedantic
LDFLAGS=-lm -DLOG_LEVEL=0 -DERROR_FATAL

all: spamfilter numbers

spamfilter: $(SPAMFILTER_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(SPAMFILTER_SRC) -I$(INCLUDE) $(LDFLAGS)

numbers: $(NUMBERS_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(NUMBERS_SRC) -I$(INCLUDE) $(LDFLAGS)

assert: $(ASSERT_SRC) Makefile
	gcc -o $@ $(CFLAGS) $(ASSERT_SRC) -I$(INCLUDE) $(LDFLAGS)

timeset: $(TIMESET_SRC) Makefile ## PERSONAL CODE
	gcc -o $@ $(CFLAGS) $(TIMESET_SRC) -I$(INCLUDE) $(LDFLAGS) 

clean:
	rm -f *~ *.o *.exe spamfilter numbers assert timeset