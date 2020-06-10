CC = gcc
FLAGS = -std=gnu99 -Wall -Wextra

traceroute: send.o receive.o error_wrapper.o traceroute.o
	$(CC) $(FLAGS) send.o receive.o error_wrapper.o traceroute.o -o traceroute

send.o: send.c send.h error_wrapper.h
	$(CC) $(FLAGS) send.c -c -o send.o

receive.o: receive.c receive.h error_wrapper.h
	$(CC) $(FLAGS) receive.c -c -o receive.o

error_wrapper.o: error_wrapper.c error_wrapper.h
	$(CC) $(FLAGS) error_wrapper.c -c -o error_wrapper.o

traceroute.o: traceroute.c traceroute.h error_wrapper.h send.h receive.h
	$(CC) $(FLAGS) traceroute.c -c -o traceroute.o

clean:
	rm -f *.o

distclean:
	rm -f *.o traceroute
