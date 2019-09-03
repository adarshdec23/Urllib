all: tests

tests: urllib_test.cpp ../Urllib.hpp
	g++ -I../ urllib_test.cpp ../Urllib.hpp -lgtest -lgtest_main -o tests

run: all
	./tests

