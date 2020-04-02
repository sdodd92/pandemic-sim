import numpy as np
from copy import deepcopy


class Pathogen:
    def __init__(self, contagiousness, incubation_period, disease_length, latent_period, base_mortality=None, mortality_rate=None):
        self.contagiousness = contagiousness
        self.base_mortality = base_mortality
        self.mortality_rate = mortality_rate
        self.incubation_period = incubation_period
        self.disease_length = disease_length
        self.latent_period = latent_period

    def replicate(self):
        new_copy = deepcopy(self)
        return new_copy

    def attack_health(self, host_health, host_resistance):
        health_attack = 0
        if self.mortality_rate is not None:
            mortality_chance = self.mortality_rate / self.disease_length
            mortality_chance *= 1 - host_resistance
            lethal = np.random.uniform() < mortality_chance
            if lethal:
                health_attack = host_health
        elif self.base_mortality is not None:
            health_attack = self.base_mortality - (host_resistance * self.base_mortality)
        return health_attack

class Person:
    next_uid = 0

    @classmethod
    def inc_uid(cls):
        cls.next_uid += 1

    def __init__(self, compliance, health, base_immunity, resistance=None, age=None):
        self.uid = self.next_uid
        self.inc_uid()
        self.compliance = compliance
        self.resistance = resistance
        if resistance is None:
            if age is not None:
                resistance_factor = base_immunity - 0.01 * age
                self.resistance = np.maximum(resistance_factor, 0)
            else:
                self.resistance = base_immunity
        self.immune = False
        self.age = age
        self.infected = False
        self.contagious = False
        self.sick = False
        self.alive = True
        self.infection_date = None
        self.health = health
        self.infection = None

    def receive_exposure(self, infection, date, override=False):
        new_infection = 0
        infected = False
        if infection is not None:
            infected = np.random.uniform() < infection.contagiousness
        if (infected and not self.immune) or override:
            new_infection = 1
            self.infected = True
            self.immune = True
            self.infection = infection
            self.infection_date = date
        return new_infection

    def give_exposure(self, date):
        if self.infected:
            if (date - self.infection_date) > self.infection.latent_period:
                return self.infection.replicate()

    def check_health(self, date):
        if self.infected:
            days_elapsed = date - self.infection_date
            if days_elapsed > self.infection.incubation_period:
                days_elapsed = days_elapsed - self.infection.incubation_period
                if days_elapsed < self.infection.disease_length:
                    self.sick = True
                    health_check = self.health - self.infection.attack_health(self.health, self.resistance) * days_elapsed
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
        contagion_a = person_a.give_exposure(date)
        contagion_b = person_b.give_exposure(date)
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

    def initiate_infection(self, pathogen, date=0, person_id=None):
        if person_id is None:
            person_id = np.random.randint(0, self.pop_size - 1)
        self.population[person_id].receive_exposure(pathogen, date, override=True)

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