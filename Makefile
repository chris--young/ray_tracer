HEADERS = Camera.h Collidable.h Lambertian.h Material.h Metal.h Ray.h Scene.h Sphere.h Vec3.h

ray_tracer: main.cpp $(HEADERS)
	g++ -Wall -std=c++14 -o ray_tracer main.cpp
