debug:
	gcc src/*.c -c
	gcc ./*.o -o ./bin/test
	rm *.o
	./bin/test
