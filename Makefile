all : sample

sample : lab2_cs203.cpp Input.txt
	g++ -std=c++14 lab2_cs203.cpp -o output

clean:
	rm -f *o output

run: output
	./output
