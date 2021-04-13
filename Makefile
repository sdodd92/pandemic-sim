O_LEVEL=2
WARNINGS=-Wall
DEBUG_FLAG=
OMP=-fopenmp
CXX_FLAGS=-std=c++11 $(DEBUG_FLAG) $(OMP) -O$(O_LEVEL) -Iinclude -Llib $(WARNINGS)
CXX=g++ $(CXX_FLAGS)

FC_FLAGS=-ffree-form -fbounds-check $(OMP) $(DEBUG_FLAG) $(WARNINGS) -Wno-tabs -Jmod -Imod -Llib
FC=gfortran $(FC_FLAGS)

CC_FLAGS=-std=c99 $(DEBUG_FLAG) $(OMP) -O$(O_LEVEL) -Iinclude -Llib $(WARNINGS)
CC=gcc $(CC_FLAGS)

RAW_FILES=core/PandemicEntities/Pathogen.cpp core/PandemicEntities/Person.cpp core/PandemicEntities/Community.cpp core/PandemicEntities/SuperCommunity.cpp core/PandemicEntities/SubPopulation.cpp

CLASS_OBJECTS=build/Pathogen.o build/Person.o build/Community.o build/SuperCommunity.o build/SubPopulation.o

raw-build: 
	$(CXX) -o bin/raw-build cli/src/main.cc $(RAW_FILES)

raw-debug:
	$(CXX) -g -o bin/raw-debug cli/src/main.cc $(RAW_FILES)

raw-profile:
	$(CXX) -pg -o bin/raw-profile cli/src/main.cc $(RAW_FILES)

super-build: lib/libentitymodule.a lib/libpandemicentities.a
	$(CXX) -o bin/supercommunity-test cli/src/super-community-cli.cc -lpandemicentities -lentitymodule -lgfortran

raw-test: lib/libentitymodule.a
	$(FC) -o test/bin/raw-entity-test test/src/test-raw.f -lentitymodule -lgomp

simple-test: lib/libentitymodule.a
	$(FC) -o test/bin/small-test test/src/simple-test.f -lentitymodule -lgomp

run-build: lib/libentitymodule.a
	$(CC) -o bin/run-sim cli/src/super-pop-run.c -lentitymodule -lgfortran  -lm -ldl -lgomp

super-cli: lib/libentitymodule.a
	$(FC) -o bin/pandem-cli cli/src/population-run.f90 -lentitymodule -lgomp
	

static-build: lib/libpandemicentities.a
	$(CXX) -o bin/static-build cli/src/main.cc lib/libpandemicentities.a   
	
linked-build: lib/libpandemicentities.so
	$(CXX) -Llib -Wl,-rpath,'lib/' -o bin/linked-build cli/src/main.cc -lpandemicentities
	
lib/libpandemicentities.a: $(CLASS_OBJECTS)
	ar -rs lib/libpandemicentities.a $(CLASS_OBJECTS) 

lib/libentitymodule.a: build/entities_interface.o
	ar -rs lib/libentitymodule.a build/entities_interface.o build/entities_mod.o build/entity_utils_mod.o
	
build/Pathogen.o:
	$(CXX) -c -o build/Pathogen.o core/PandemicEntities/Pathogen.cpp
	
build/Person.o:
	$(CXX) -c -o build/Person.o core/PandemicEntities/Person.cpp
	
build/Community.o:
	$(CXX) -c -o build/Community.o core/PandemicEntities/Community.cpp

build/SuperCommunity.o:
	$(CXX) -c -o build/SuperCommunity.o core/PandemicEntities/SuperCommunity.cpp

build/SubPopulation.o:
	$(CXX) -c -o build/SubPopulation.o core/PandemicEntities/SubPopulation.cpp

build/entity_utils_mod.o:
	$(FC) -c -o build/entity_utils_mod.o core/utils/entity-utils-module.f 


build/entities_mod.o: build/entity_utils_mod.o
	$(FC) -c -o build/entities_mod.o core/raw-entities/entities_module.f 

build/entities_interface.o: build/entities_mod.o
	$(FC) -c -o build/entities_interface.o core/raw-entities/entities_interface.f 

	
lib/libpandemicentities.so:
	$(CXX) -fPIC -shared -o lib/libpandemicentities.so $(RAW_FILES)
	
all: raw-build static-build linked-build
	
clean:
	rm build/* 
	rm mod/*

clear:
	rm bin/*
	rm lib/*

clean-test:
	rm test/bin/*

cleanup: clean clear
