flags = -Wall -Werror -pthread

all: problem0.c
	$(CC) problem0.c -o run $(flags)

test: problem0.c
	$(CC) problem0.c -o run $(flags) -fsanitize=address
	sh test.sh