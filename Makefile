HEADERS = Camera.h Collidable.h Lambertian.h Material.h Metal.h Ray.h Scene.h Sphere.h Vec3.h

ray_tracer: main.cc $(HEADERS)
	g++ -std=c++11 -o ray_tracer main.cc

debug: main.cc $(HEADERS)
	g++ -g -Wall -std=c++11 -o debug main.cc

