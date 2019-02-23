ray_tracer: main.cc
	g++ -std=c++11 -o ray_tracer main.cc

render: ray_tracer
	./ray_tracer > image.ppm

