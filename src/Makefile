CC     = g++
CFLAGS = -g -std=c++11
OFILES = main.o species.o world.o tile.o

projet: $(OFILES)
	$(CC) $(OFILES) -o $@
	
clean:
	@/bin/rm -f *.o *.x *.c~ *.h~ projet

main.o: main.cc species.h
	$(CC) $(CFLAGS) -c $< -o $@

world.o: world.cc world.h tile.h const.h
	$(CC) $(CFLAGS) -c $< -o $@

tile.o: tile.cc tile.h species.h
	$(CC) $(CFLAGS) -c $< -o $@

species.o: species.cc species.h
	$(CC) $(CFLAGS) -c $< -o $@