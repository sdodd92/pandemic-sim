# distutils: language=c++
from entities cimport Pathogen

cdef class PyPathogen:
    cdef Pathogen pathogen
    
    def __cinit__(self, double contagiousness, double mortality_rate, int disease_length, int incubation_period, int latent_period):
        self.pathogen = Pathogen(contagiousness, mortality_rate, disease_length, incubation_period, latent_period)
        
    def kill(self, double host_resistance):
        return self.pathogen.kill(host_resistance)
        
