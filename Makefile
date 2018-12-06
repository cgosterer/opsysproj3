all: driver.o info.o
	gcc -o fileSys.exe driver.o info.o

driver.o: driver.c fatheader.h
	gcc -c driver.c

info.o: info.c fatheader.h
	gcc -c info.c

clean:
	rm *~ *.o fileSys.exe
	cp ~/Downloads/fat32.img ./
