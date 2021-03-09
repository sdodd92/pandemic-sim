O_LEVEL=0
DEBUG_LEVEL=-ggdb3
WARNINGS=-Wall
FORT_STD=gnu
EXTRA_FLAGS=
BUILD_FLAGS=-std=c++11 -O$(O_LEVEL) -I../include $(WARNINGS)
GCC=g++ $(DEBUG_LEVEL) $(BUILD_FLAGS) $(EXTRA_FLAGS)
GFORT=gfortran  $(DEBUG_LEVEL) -O$(O_LEVEL) $(WARNINGS) -Wno-tabs -fbounds-check -ffree-form  $(EXTRA_FLAGS)

RAW_FILES=../core/PandemicEntities/Pathogen.cpp ../core/PandemicEntities/Person.cpp ../core/PandemicEntities/Community.cpp ../core/PandemicEntities/Population.cpp ../core/PandemicEntities/Border.cpp


uid-check:
	$(GCC) src/testing/uid-check.cc ../lib/libpandemicentities.a -o bin/uid-check

nation-check:
	$(GCC) src/testing/nation-check.cc $(RAW_FILES) -o bin/nation-check

uid-singular:
	$(GCC) src/testing/uid-check-singular.cc ../lib/libpandemicentities.a -o bin/uid-singular

uid-debug:
	$(GCC) -g src/testing/uid-check.cc ../lib/libpandemicentities.a -o bin/uid-check-debug

singular-debug:
	$(GCC) -g src/testing/uid-check-singular.cc $(RAW_FILES) -o tmp/singular-debug

test: fortran-test cpp-test wrapper-test
	
fortran-test: entities.o utilities.o
	$(GFORT) src/fortran-dev/test.f entities.o utilities.o -o fortran-test

cpp-test:
	$(GCC) -L../lib -Wl,-rpath=../lib src/fortran-dev/control.cc -lpandemicentities -o cpp-test

wrapper-test: FastPopulation.o entities.o wrap-entities.o utilities.o
	$(GCC) -L../lib -Wl,-rpath=../lib src/fortran-dev/fastpop-test.cc FastPopulation.o wrap-entities.o entities.o utilities.o -o wrapper-test  -lgfortran -lpandemicentities -lgomp

FastPopulation.o: wrap-entities.o
	$(GCC) -c src/fortran-dev/FastPopulation.cpp

entities.o: utilities.o
	$(GFORT) -c src/fortran-dev/entities.f

utilities.o:
	$(GFORT) -c src/fortran-dev/utilities.f

wrap-entities.o: entities.o
	$(GFORT) -c src/fortran-dev/wrap-entities.f

clean-test:
	rm entities.mod entities.o utilities.o utils.mod fortran-test cpp-test wrap-entities.o FastPopulation.o wrapper-test
