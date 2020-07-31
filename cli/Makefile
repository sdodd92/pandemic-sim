O_LEVEL=2
WARNINGS=-Wall
BUILD_FLAGS=-std=c++11 -O$(O_LEVEL) -Iinclude $(WARNINGS)
GCC=g++ $(BUILD_FLAGS)

RAW_FILES=src/Pathogen.cpp src/Person.cpp src/Community.cpp

raw-build: 
	$(GCC) -o bin/raw-build src/main.cc $(RAW_FILES)

static-build: lib/libpandemicworld.a
	$(GCC) -o bin/static-build src/main.cc lib/libpandemicworld.a   
	
linked-build: lib/libpandemicworld.so
	$(GCC) -Llib -Wl,-rpath,'lib/' -o bin/linked-build src/main.cc -lpandemicworld
	
lib/libpandemicworld.a: build/Pathogen.o build/Person.o build/Community.o
	ar -rs lib/libpandemicworld.a build/Pathogen.o build/Person.o build/Community.o
	
build/Pathogen.o:
	$(GCC) -c -o build/Pathogen.o src/Pathogen.cpp
	
build/Person.o:
	$(GCC) -c -o build/Person.o src/Person.cpp
	
build/Community.o:
	$(GCC) -c -o build/Community.o src/Community.cpp
	
lib/libpandemicworld.so:
	$(GCC) -fPIC -shared -o lib/libpandemicworld.so $(RAW_FILES)
	
all: raw-build static-build linked-build
	
clean:
	rm build/* bin/*
