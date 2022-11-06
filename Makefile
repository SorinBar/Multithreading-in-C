all: build run clean
build:
	gcc main.c -lpthread -o exe
run:
	./exe
testRun:
	valgrind --leak-check=full --track-origins=yes ./exe
clean:
	rm exe