## Authors: 
## Steffen Viken Valvaag <steffenv@cs.uit.no> 
## Morten Grønnesby <morten.gronnesby@uit.no>

LIST_SRC=linkedlist.c # Add your .c file implementing the list ADT here
SORTWORDS_SRC=sortwords.c common.c $(LIST_SRC)
REVERSEWORDS_SRC=reversewords.c common.c $(LIST_SRC)
HEADERS=include

SORTWORDS_SRC:=$(patsubst %.c,src/%.c, $(SORTWORDS_SRC))
REVERSEWORDS_SRC:=$(patsubst %.c,src/%.c, $(REVERSEWORDS_SRC))

CFLAGS=-Wall -Wextra -g
LDFLAGS=-lm -DLOG_LEVEL=0

all: sortwords reversewords

sortwords: $(SORTWORDS_SRC) $(HEADERS) Makefile
	gcc $(CFLAGS) -o $@ $(SORTWORDS_SRC) -I$(HEADERS)

reversewords: $(REVERSEWORDS_SRC) $(HEADERS) Makefile
	gcc -o $@ $(REVERSEWORDS_SRC) -I$(HEADERS)

clean:
	rm -f *~ *.o *.exe sortwords reversewords
