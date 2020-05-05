# distutils: language=c++
cdef extern from  "../core/PandemicEntities/Pathogen.cpp":
    pass
    
cdef extern from "../core/PandemicEntities/Pathogen.h":
    cdef cppclass Pathogen:
        Pathogen() except +
        Pathogen(double, double, int, int, int) except +
        
        
        bint kill(double)
        bint kill()
        
        double get_contagiousness()
        double get_mortality_rate()
        int get_disease_length()
        int get_incubation_period()
        int get_latent_period()
