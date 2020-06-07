CC     = g++
CFLAGS = -g -std=c++11
OFILES = main.o

projet: $(OFILES)
	$(CC) $(OFILES) -o $@
	
clean:
	@/bin/rm -f *.o *.x *.c~ *.h~ projet

main.o: main.cc
	$(CC) $(CFLAGS) -c $< -o $@