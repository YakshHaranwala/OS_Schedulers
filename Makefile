# makefile for scheduling program
#
# make rr - for round-robin scheduling
# make fcfs - for FCFS scheduling
# make sjf - for SJF scheduling
# make priority - for priority scheduling
# make priority_rr - for priority with round robin scheduling

CFLAGS=-Wall -Wextra -Wpedantic --debug -lm -lpthread
CC=gcc

clean:
	rm -rf *.o
	rm -rf fcfs
	rm -rf sjf
	rm -rf rr
	rm -rf priority
	rm -rf priority_rr

rr: driver.o list.o CPU.o schedule_rr.o pid_manager.o
	$(CC) $(CFLAGS) -o rr driver.o schedule_rr.o list.o CPU.o pid_manager.o

sjf: driver.o list.o CPU.o schedule_sjf.o pid_manager.o
	$(CC) $(CFLAGS) -o sjf driver.o schedule_sjf.o list.o CPU.o pid_manager.o

fcfs: driver.o list.o CPU.o schedule_fcfs.o pid_manager.o
	$(CC) $(CFLAGS) -o fcfs driver.o schedule_fcfs.o pid_manager.o list.o CPU.o

priority: driver.o list.o CPU.o pid_manager.o schedule_priority.o
	$(CC) $(CFLAGS) -o priority pid_manager.o driver.o schedule_priority.o list.o CPU.o

priority_rr: driver.o list.o CPU.o schedule_priority_rr.o pid_manager.o
	$(CC) $(CFLAGS) -o priority_rr driver.o schedule_priority_rr.o list.o pid_manager.o CPU.o

schedule_fcfs.o: schedule_fcfs.c
	$(CC) $(CFLAGS) -c schedule_fcfs.c

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

pid_manager.o: pid_manager.c pid_manager.h
	$(CC) $(CFLAGS) -c pid_manager.c

schedule_sjf.o: schedule_sjf.c
	$(CC) $(CFLAGS) -c schedule_sjf.c

schedule_priority.o: schedule_priority.c
	$(CC) $(CFLAGS) -c schedule_priority.c

schedule_rr.o: schedule_rr.c
	$(CC) $(CFLAGS) -c schedule_rr.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

CPU.o: CPU.c cpu.h
	$(CC) $(CFLAGS) -c CPU.c
