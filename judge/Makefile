GLIB=`pkg-config --libs --cflags glib-2.0`

judger: main.o parse.o command.o jcommand.o judge.o conf.o global.h
	gcc -o judger main.o parse.o command.o jcommand.o judge.o conf.o $(GLIB)

main.o: main.c global.h
	gcc -c main.c $(GLIB)

parse.o: parse.c global.h
	gcc -c parse.c $(GLIB)

command.o: command.c global.h
	gcc -c command.c $(GLIB)

judge.o: judge.c global.h
	gcc -c judge.c $(GLIB)

conf.o: conf.c global.h
	gcc -c conf.c $(GLIB)

jcommand.o: jcommand.c global.h
	gcc -c jcommand.c $(GLIB)

clean:
	rm -f *.o judger

install:
	cp -f judger /usr/bin/
	chown root:root /usr/bin/judger
	chmod 4755 /usr/bin/judger
	mkdir -p /etc/wyuoj
	cp -f java.policy /etc/wyuoj

uninstall:
	rm -f /usr/bin/judger
	rm /etc/wyuoj/java.policy
	rmdir --ignore-fail-on-non-empty /etc/wyuoj

