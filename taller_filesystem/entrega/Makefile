CC=g++
CFLAGS=-g -m32 -I../include/

all: tallerFS

tallerFS: tallerFS.o ../src/hdd.o ../src/mbr.o ../src/pentry.o ../src/ext2fs.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

tallerFS.o: tallerFS.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

../src/%.o: ../src/%.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

clean:
	rm -rf *.o
	rm -rf ../src/*.o
	rm -rf tallerFS