# distutils: language=c++
cdef extern from  "../core/PandemicEntities/Pathogen.cpp":
    pass
    
cdef extern from "../core/PandemicEntities/Pathogen.h":
    cdef cppclass Pathogen:
        Pathogen() except +
        Pathogen(double, double, int, int, int) except +
        
        
        bint attempt_kill(double)
        bint kill()
        
        double get_contagiousness()
        double get_mortality_rate()
        int get_disease_length()
        int get_incubation_period()
        int get_latent_period()
        

cdef extern from "../core/PandemicEntities/Person.cpp":
    pass
    

cdef extern from "../core/PandemicEntities/Pathogen.h":
    cdef cppclass Person:
        Person() except +
        Person(double, double) except +
        
        int get_date_infected()
        bint is_infected()
        
        void set_compliance(double)
        void set_resistance(double)
        
        int get_uid()
        
        bint catch_infection(Pathogen*, int, bint)
        bint survival_update(int)
        bint is_alive()
        
        Pathogen* pass_infection()
