CC     = g++
CFLAGS = -g -std=c++11
OFILES = main.o simulation.o species.o world.o tile.o tools.o vegetation.o

projet: $(OFILES)
	$(CC) $(OFILES) -o $@
	
clean:
	@/bin/rm -f *.o *.x *.c~ *.h~ projet

main.o: main.cc simulation.h
	$(CC) $(CFLAGS) -c $< -o $@

simulation.o: simulation.cc simulation.h world.h
	$(CC) $(CFLAGS) -c $< -o $@

world.o: world.cc world.h tile.h const.h species.h tools.h
	$(CC) $(CFLAGS) -c $< -o $@

tile.o: tile.cc tile.h species.h vegetation.h
	$(CC) $(CFLAGS) -c $< -o $@

species.o: species.cc species.h tools.h const.h
	$(CC) $(CFLAGS) -c $< -o $@

vegetation.o: vegetation.cc vegetation.h
	$(CC) $(CFLAGS) -c $< -o $@

tools.o: tools.cc tools.h
	$(CC) $(CFLAGS) -c $< -o $@