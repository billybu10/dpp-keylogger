CC=g++
CPPFLAGS=-cpp -Wall -ldpp
KEYLOG_TARGETS=main.o keylogger.o find_event_files.o

all: keylog

keylog: $(KEYLOG_TARGETS)
	$(CC) $^ -ldpp -o $@


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f keylogger.o main.o find_event_files.o
	@echo "Finished cleaning."
