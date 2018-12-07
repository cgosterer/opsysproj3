all: driver.o info.o directories.o
	gcc -o fileSys.exe driver.o info.o directories.o

driver.o: driver.c fatheader.h
	gcc -c driver.c

info.o: info.c fatheader.h
	gcc -c info.c

directories.o: directories.c fatheader.h
	gcc -c directories.c

clean:
	rm *~ *.o fileSys.exe
	cp ~/Downloads/fat32.img ./
