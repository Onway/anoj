GLIB=`pkg-config --libs --cflags glib-2.0`

judger: main.o parse.o command.o judge.o autocode.o global.h
	gcc -o judger main.o parse.o command.o judge.o autocode.o $(GLIB)

main.o: main.c global.h
	gcc -c main.c $(GLIB)

parse.o: parse.c global.h
	gcc -c parse.c $(GLIB)

command.o: command.c global.h
	gcc -c command.c $(GLIB)

judge.o: judge.c global.h
	gcc -c judge.c $(GLIB)

autocode.o: autocode.c global.h
	gcc -c autocode.c $(GLIB)

clean:
	rm -f *.o *.out judger

install:
	cp -f judger /usr/bin/
	chown root:root /usr/bin/judger
	chmod 4755 /usr/bin/judger

uninstall:
	rm -f /usr/bin/judger

