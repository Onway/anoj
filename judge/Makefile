GLIB=`pkg-config --libs --cflags glib-2.0`

main:
	gcc main.c global.c cmdline.c exit.c $(GLIB)

exit:
	gcc -c exit.c $(GLIB)

global:
	gcc -c global.c $(GLIB)

keyfile:
	gcc -c keyfile.c $(GLIB)

cmdline:
	gcc -c cmdline.c $(GLIB)

clean:
	rm *.o *.out
