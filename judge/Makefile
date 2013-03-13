GLIB=`pkg-config --libs --cflags glib-2.0`

main:
	gcc main.c parse.c command.c judge.c autocode.c $(GLIB)

clean:
	rm -f *.o *.out
