CC     = g++
CFLAGS = -g -std=c++11
OFILES = main.o species.o

projet: $(OFILES)
	$(CC) $(OFILES) -o $@
	
clean:
	@/bin/rm -f *.o *.x *.c~ *.h~ projet

main.o: main.cc species.h
	$(CC) $(CFLAGS) -c $< -o $@

species.o: species.cc species.h
	$(CC) $(CFLAGS) -c $< -o $@