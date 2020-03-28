import numpy as np

BASE_IMMUNITY = 0.8
CONTAGIOUSNESS = .02
BASE_MORTALITY = 5
NON_INFECTIOUS_PERIOD = 1
INCUBATION_PERIOD = 3
DISEASE_LENGTH = 10
MAX_AGE = 100


class Person:
    next_uid = 0

    @classmethod
    def inc_uid(cls):
        cls.next_uid += 1

    def __init__(self, compliance, age):
        self.uid = self.next_uid
        self.inc_uid()
        self.compliance = compliance
        resistance_factor = BASE_IMMUNITY - 0.01 * age
        self.resistance = np.maximum(resistance_factor, 0)
        self.immune = False
        self.age = age
        self.infected = False
        self.contagious = False
        self.sick = False
        self.alive = True
        self.infection_date = None
        self.health = MAX_AGE - (0.9 * self.age)

    def receive_exposure(self, infected, date):
        new_infection = 0
        if infected and not self.immune:
            new_infection = 1
            self.infected = True
            self.immune = True
            self.infection_date = date
        return new_infection

    def give_exposure(self, date):
        if self.infected:
            if (date - self.infection_date) > NON_INFECTIOUS_PERIOD:
                self.contagious = True
        return self.contagious

    def check_health(self, date):
        if self.infected:
            days_elapsed = date - self.infection_date
            if days_elapsed > INCUBATION_PERIOD:
                days_elapsed = days_elapsed - INCUBATION_PERIOD
                if days_elapsed < DISEASE_LENGTH:
                    self.sick = True
                    health_check = self.health - (BASE_MORTALITY - (self.resistance * BASE_MORTALITY)) * days_elapsed
                    if health_check <= 0:
                        self.alive = False
                else:
                    self.sick = False
                    self.infected = False
                    self.infection_date = None
        return self.alive


class Community:
    @staticmethod
    def pairwise_interaction(person_a, person_b, date):
        new_infections = 0
        contagion_a = bool(person_a.give_exposure(date) * (np.random.uniform() < CONTAGIOUSNESS))
        contagion_b = bool(person_b.give_exposure(date) * (np.random.uniform() < CONTAGIOUSNESS))
        new_infections += person_a.receive_exposure(contagion_b, date)
        new_infections += person_b.receive_exposure(contagion_a, date)
        return new_infections

    def __init__(self, sociability):
        self.population = []
        self.sociability = sociability
        self.pop_size = 0

    def add_member(self, person):
        self.population.append(person)
        self.pop_size += 1

    def remove_member(self, person_id=None):
        if person_id is None:
            person_id = np.random.randint(0, self.pop_size - 1)
        del self.population[person_id]
        self.pop_size -= 1

    def mingle(self, date):
        new_infections = 0
        if self.pop_size > 1:
            for i in range(self.pop_size):
                interactions = int(np.random.poisson(self.sociability, 1))
                for interaction in range(interactions):
                    j = np.random.randint(0, self.pop_size - 1)
                    while j == i:
                        j = np.random.randint(0, self.pop_size - 1)
                    new_infections += self.pairwise_interaction(self.population[i], self.population[j], date)
        return new_infections

    def initiate_infection(self, person_id=None):
        if person_id is None:
            person_id = np.random.randint(0, self.pop_size - 1)
        self.population[person_id].receive_exposure(True, 0)

    def update_health(self, date):
        died = 0
        n = 0
        while n < self.pop_size:
            survived = self.population[n].check_health(date)
            if survived:
                n += 1
            else:
                self.remove_member(n)
                died += 1
        return died