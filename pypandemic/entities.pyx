# distutils: language=c++
from entities cimport Pathogen, Person
from cython.operator cimport dereference


cdef class PathogenVector:
    cdef Pathogen* _pathogen_ptr
        
    @staticmethod
    cdef PathogenVector wrap_pathogen(Pathogen* pathogen):
        cdef PathogenVector new_pathogen = PathogenVector.__new__(PathogenVector)
        new_pathogen._pathogen_ptr = pathogen
        return new_pathogen        
        
    @staticmethod
    cdef PathogenVector new_pathogen(double contagiousness, double mortality_rate, int disease_length, int incubation_period, int latent_period):
        cdef Pathogen* pathogen_ptr
        pathogen_ptr = new Pathogen(contagiousness, mortality_rate, disease_length, incubation_period, latent_period)
        cdef PathogenVector new_pathogen = PathogenVector.wrap_pathogen(pathogen_ptr)
        return new_pathogen
        
    def attempt_kill(self, double host_resistance):
        return self._pathogen_ptr.attempt_kill(host_resistance)
        
        
        
cdef class PyPathogen(PathogenVector):
    cdef PathogenVector _py_container
    
    def __cinit__(self, double contagiousness, double mortality_rate, int disease_length, int incubation_period, int latent_period):
        cdef Pathogen* pathogen_ptr
        pathogen_ptr = new Pathogen(contagiousness, mortality_rate, disease_length, incubation_period, latent_period)
        self._pathogen_ptr = pathogen_ptr
        self._py_container = PathogenVector.wrap_pathogen(pathogen_ptr)
        
    @property
    def pathogen(self):
        return self._py_container
        
        
cdef class PyPerson:
    cdef Person* person
    cdef PathogenVector infection
    
    def __cinit__(self):
        self.person = new Person()
        
    def __init__(self, double compliance, double resistance):
        self.person.set_compliance(compliance)
        self.person.set_resistance(resistance)
        self.infection = None
        
    @property
    def infection(self):
        return self.infection
        
    def get_date_infected(self):
        return self.person.get_date_infected()
        
    def is_infected(self):
        return self.person.is_infected()
        
    def get_uid(self):
        return self.person.get_uid()
        
    def catch_infection(self, int date, bint force, PathogenVector infection):
        self.infection = infection
        return self.person.catch_infection(infection._pathogen_ptr, date, force)
        
    def survival_update(self, int date):
        return self.person.survival_update(date)
        
    def is_alive(self):
        return self.person.is_alive()
        
    def pass_infection(self):
        return PathogenVector.wrap_pathogen(self.person.pass_infection())
