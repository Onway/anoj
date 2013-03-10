GLIB=`pkg-config --libs --cflags glib-2.0`

exit:
	gcc -c exit.c $(GLIB)

global:
	gcc -c global.c $(GLIB)

keyfile:
	gcc -c keyfile.c $(GLIB)

clean:
	rm *.o
