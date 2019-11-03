filter.so: filter.o
	$(CC) $< -shared -o $@ -lhdf5

filter.o: filter.c
	$(CC) $< -c -Wall -Wextra -Werror -g -O0

.PHONY: clean
clean:
	@rm -f filter.so filter.o
