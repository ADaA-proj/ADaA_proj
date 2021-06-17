test: shape2D.o geometry2D.o test.o
	g++ -Wall test.o shape2D.o geometry2D.o -o test

%.o: %.cpp
	g++ -Wall -c $< -o $@

clean:
	rm -rf *.o test