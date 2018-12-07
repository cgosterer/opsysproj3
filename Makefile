all: driver.o info.o utlity.o
	gcc -o fileSys.exe driver.o info.o utlity.o

driver.o: driver.c fatheader.h
	gcc -c driver.c

info.o: info.c fatheader.h
	gcc -c info.c

utlity.o: utlity.c fatheader.h
	gcc -c utlity.c

clean:
	rm *~ *.o fileSys.exe
	cp ~/Downloads/fat32.img ./
