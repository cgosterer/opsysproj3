all: driver.o info.o directories.o utility.o user.o
	gcc -o fileSys.exe driver.o info.o directories.o utility.o user.o

driver.o: driver.c fatheader.h
	gcc -c driver.c

info.o: info.c fatheader.h
	gcc -c info.c

directories.o: directories.c fatheader.h
	gcc -c directories.c

utility.o: utility.c fatheader.h
	gcc -c utility.c

user.o:	user.c fatheader.h
	gcc -c user.c

clean:
	rm *~ *.o fileSys.exe
	cp ~/Downloads/fat32.img ./
