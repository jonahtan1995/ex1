CC = gcc
OBJS = main.o IsraeliQueue.o HackEnrollment.o
EXEC = HackEnrollment
DEBUG_FLAG = #when needed, assign -g for debug
COMP_FLAG = -std=c99 -Wall -Werror -pedantic
INCLUDE = -I/tool -I/new_home/courses/mtm/public/2223b/ex1/
ROAD = /new_home/courses/mtm/public/2223b/ex1/
TOOL = tool/
MATH = -lm

$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAG) $(INCLUDE) $(OBJS) -o $@ $(MATH)
IsraeliQueue.o: IsraeliQueue.c $(ROAD)IsraeliQueue.h
	$(CC) -c $(DEBUG_FLAG) $(INCLUDE) $(COMP_FLAG)$*.c
HackEnrollment.o: $(TOOL)HackEnrollment.c $(TOOL)HackEnrollment.h $(ROAD)IsraeliQueue.h
	$(CC) -c $(DEBUG_FLAG) $(INCLUDE) $(COMP_FLAG) $(TOOL)$*.c
main.o: $(TOOL)main.c $(TOOL)HackEnrollment.h $(ROAD)IsraeliQueue.h
	$(CC) -c $(DEBUG_FLAG) $(INCLUDE) $(COMP_FLAG) $(TOOL)$*.c
clean:
	rm -f $(OBJS) $(EXEC)