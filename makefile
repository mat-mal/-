CC      = gcc
CCFLAGS = -Wall -std=c17
LDLIBS  =
SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)
OBJS = $(patsubst %.c, %.o, $(SRCS))
PROG = Astro
$(PROG): $(OBJS)
	@echo [Sources: $(SRCS)]
	@echo [Headers: $(HDRS)]
	@echo [Objects: $(OBJS)]
	@echo [Program: $(PROG)]
	$(CC) -o $@ $^ $(LDLIBS)
%.o: %.c $(HDRS)
	@echo [$< depends on all header files]
	$(CC) $(CCFLAGS) -c $<
%.o: %.c %.h
	@echo [$< has a header file of same name]
	$(CC) $(CCFLAGS) -c $<
%.o: %.c
	@echo [$< does not have a header file]
	$(CC) $(CCFLAGS) -c $<
.PHONY: clean
clean:
	@echo [cleaning up]
	del -f $(OBJS) $(PROG).exe
.PHONY: run
run:
	@echo [executing $(PROG)]
	$(PROG)
