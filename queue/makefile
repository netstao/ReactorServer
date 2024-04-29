all:demo3 incache outcache

demo3:demo3.cpp _public.h _public.cpp
	g++ -g -o demo3 demo3.cpp _public.cpp

incache:incache.cpp _public.h _public.cpp
	g++ -g -o incache incache.cpp _public.cpp

outcache:outcache.cpp _public.h _public.cpp
	g++ -g -o outcache outcache.cpp _public.cpp

clean:
	rm -f demo1 demo2 demo3 incache outcache