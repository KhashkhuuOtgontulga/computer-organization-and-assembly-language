CC:=gcc
CFLAGS:=-Wall -Wno-unused-variable -Wno-unused-but-set-variable -std=c99
LIBS:=-lm

all: lab6

lab6-soln: lab6-soln.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $@.c

lab6: lab6.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $@.c

test: lab6
	@./test_harness lab6 tests lab6-soln

handin:
	@echo "Turning in the following files..."
	@tar cvzf `whoami`-lab6.tgz lab6.c lab6.h Makefile
	@cp `whoami`-lab6.tgz /home/khale/HANDIN/lab6
	@echo "You can run 'ls /home/khale/HANDIN/lab6/<yourid>-lab6.tgz' to make sure your file made it to the HANDIN dir"

clean:
	rm -f lab6 

.PHONY: clean test handin
