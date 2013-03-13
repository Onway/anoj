GLIB=`pkg-config --libs --cflags glib-2.0`

main:
	gcc main.c global.c cmdline.c keyfile.c exit.c autorule.c data.c child.c answer.c $(GLIB) -g

exit:
	gcc -c exit.c $(GLIB)

global:
	gcc -c global.c $(GLIB)

keyfile:
	gcc -c keyfile.c $(GLIB)

cmdline:
	gcc -c cmdline.c $(GLIB)

autorule:
	gcc -c autorule.c $(GLIB)

data:
	gcc -c data.c $(GLIB)

child:
	gcc -c child.c $(GLIB)

answer:
	gcc -c answer.c $(GLIB)

clean:
	rm -f *.o *.out
