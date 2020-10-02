O_LEVEL=2
WARNINGS=-Wall
BUILD_FLAGS=-std=c++11 -O$(O_LEVEL) -Iinclude $(WARNINGS)
GCC=g++ $(BUILD_FLAGS)

RAW_FILES=core/PandemicEntities/Pathogen.cpp core/PandemicEntities/Person.cpp core/PandemicEntities/Community.cpp

EXTRA_FILES=core/PandemicEntities/Population.cpp core/PandemicEntities/Border.cpp core/PandemicEntities/SubCommunity.cpp

raw-build: 
	$(GCC) -o bin/raw-build cli/src/main.cc $(RAW_FILES)

raw-debug:
	$(GCC) -g -o bin/raw-debug cli/src/main.cc $(RAW_FILES)

raw-profile:
	$(GCC) -pg -o bin/raw-profile cli/src/main.cc $(RAW_FILES)

super-pop:
	$(GCC) -o bin/super-pop cli/src/main-super-pop.cc $(RAW_FILES) $(EXTRA_FILES)

super-pop-debug:
	$(GCC) -g -o bin/super-pop-debug cli/src/main-super-pop.cc $(RAW_FILES) $(EXTRA_FILES)

raw-parallel:
	$(GCC) -fopenmp -DPARALLEL_MINGLE -o bin/parallel-build cli/src/main.cc $(RAW_FILES)

static-build: lib/libpandemicentities.a
	$(GCC) -o bin/static-build cli/src/main.cc lib/libpandemicentities.a   
	
linked-build: lib/libpandemicentities.so
	$(GCC) -Llib -Wl,-rpath,'lib/' -o bin/linked-build cli/src/main.cc -lpandemicworld
	
lib/libpandemicentities.a: build/Pathogen.o build/Person.o build/Community.o
	ar -rs lib/libpandemicentities.a build/Pathogen.o build/Person.o build/Community.o
	
build/Pathogen.o:
	$(GCC) -c -o build/Pathogen.o core/PandemicEntities/Pathogen.cpp
	
build/Person.o:
	$(GCC) -c -o build/Person.o core/PandemicEntities/Person.cpp
	
build/Community.o:
	$(GCC) -c -o build/Community.o core/PandemicEntities/Community.cpp
	
lib/libpandemicentities.so:
	$(GCC) -fPIC -shared -o lib/libpandemicentities.so $(RAW_FILES) $(EXTRA_FILES)
	
all: raw-build static-build linked-build
	
clean:
	rm build/* bin/* lib/*
