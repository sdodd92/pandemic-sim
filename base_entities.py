import numpy as np
import seaborn as sns
from matplotlib import pyplot as plt
import pandas as pd


BASE_IMMUNITY = 0.8
CONTAGIOUSNESS = .02
BASE_MORTALITY = 3
MAX_AGE = 100


class Person:
    def __init__(self, compliance, age):
        self.compliance = compliance
        immunity_factor = BASE_IMMUNITY - 0.01 * age
        self.immunity = int(np.random.uniform() < immunity_factor)
        self.age = age
        self.infected = False
        self.alive = True
        self.health = MAX_AGE - self.age

    def update_health(self):
        if self.alive and self.infected:
            self.health -= BASE_MORTALITY - (self.immunity * BASE_MORTALITY)
        if self.health <= 0:
            self.alive = False


class Community:
    @staticmethod
    def pairwise_interaction(person_a, person_b):
        new_infection = np.random.uniform() < CONTAGIOUSNESS
        if person_a.infected and not person_b.infected:
            person_b.infected = new_infection
        elif person_b.infected and not person_a.infected:
            person_a.infected = new_infection

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

    def mingle(self):
        for i in range(self.pop_size):
            interactions = int(np.random.poisson(self.sociability, 1))
            for interaction in range(interactions):
                j = np.random.randint(0, self.pop_size - 1)
                while j == i:
                    j = np.random.randint(0, self.pop_size - 1)
                self.pairwise_interaction(self.population[i], self.population[j])

    def initiate_infection(self, person_id=None):
        if person_id is None:
            person_id = np.random.randint(0, self.pop_size - 1)
        self.population[person_id].infected = True


class Nation(Community):
    def __init__(self, pop_size, avg_compliance, avg_age, sociability):
        super().__init__(sociability)
        self.pop_size = pop_size
        compliances = (np.random.uniform(size=pop_size) < avg_compliance).astype(int)
        ages = np.floor(np.random.uniform(20, MAX_AGE, size=pop_size))
        self.population = [Person(compliance, age) for compliance, age in zip(compliances, ages)]
        self.sociability = sociability
        self.border_open = True

    def get_infection_rate(self):
        return np.mean([person.infected for person in self.population])

    def get_num_infected(self):
        return np.sum([person.infected for person in self.population])

    def get_num_dead(self):
        return np.sum([not person.alive for person in self.population])

    def get_ages(self):
        return [person.age for person in self.population]

    def get_immunities(self):
        return [person.immunity for person in self.population]

    def plot_age_distribution(self):
        sns.distplot(self.get_ages())
        return plt

    def get_avg_immunity(self):
        return np.mean([person.immunity for person in self.population])


class Border:
    @staticmethod
    def exchange(crossing_volume, a, b):
        country_a, country_b = a, b
        for crossing in range(crossing_volume):
            member_to_cross = np.random.randint(0, country_a.pop_size - 1)
            country_b.add_member(country_a.population[member_to_cross])
            country_a.remove_member(member_to_cross)

    def __init__(self, country_a, country_b):
        self.country_a = country_a
        self.country_b = country_b

    def direct_exchange(self, crossing_volume):
        if self.country_b.border_open:
            self.exchange(crossing_volume, self.country_a, self.country_b)
        if self.country_a.border_open:
            self.exchange(crossing_volume, self.country_b, self.country_a)

    def vehicular_exchange(self, crossing_volume, social_exposure):
        vehicle_a = vehicle_b = None
        if self.country_b.border_open:
            vehicle_a = Community(sociability=social_exposure)
            self.exchange(crossing_volume, self.country_a, vehicle_a)
            vehicle_a.mingle()
        if self.country_a.border_open:
            vehicle_b = Community(sociability=social_exposure)
            self.exchange(crossing_volume, self.country_b, vehicle_b)
            vehicle_b.mingle()
        if vehicle_a is not None:
            self.exchange(crossing_volume, vehicle_a, self.country_a)
        if vehicle_b is not None:
            self.exchange(crossing_volume, vehicle_b, self.country_b)


class World:
    def __init__(self):
        self.nations = {}
        self.border_enum = None
        self.borders = None
        self.day = 0
        self.daily_log = pd.DataFrame(columns=['Day', 'Nation', 'num_infected', 'num_died'])

    def init_nation(self, name, nation=None, borders=None, **kwargs):
        if nation is None:
            nation = Nation(**kwargs)
        self.nations[name] = nation
        if borders is not None:
            if self.border_enum is None:
                self.border_enum = {}
            self.border_enum[name] = []
            if np.isscalar(borders):
                borders = [borders]
            for border in borders:
                self.border_enum[name].append(border)

    def init_borders(self):
        if self.border_enum is not None:
            border_pairs = []
            for country_a, all_country_b in self.border_enum.items():
                for country_b in all_country_b:
                    border_pairs.append((country_a, country_b))
            self.borders = [Border(self.nations[a], self.nations[b]) for a, b in border_pairs]

    def new_day(self, days=1, log=True, verbose=False):
        for day in range(days):
            self.day += 1
            for name, nation in self.nations.items():
                nation.mingle()
                if log:
                    log_entry = pd.DataFrame({
                        'Day': [self.day],
                        'Nation': [name],
                        'num_infected': [float(nation.get_num_infected())],
                        'num_died': [float(nation.get_num_dead())]
                    })
                    self.daily_log = self.daily_log.append(log_entry)
                if verbose:
                    print('{} days elapsed'.format(day))
            if self.borders is not None:
                for border in self.borders:
                    border.direct_exchange(50)

    def initiate_infection(self, nation=None):
        if nation is None:
            nations = [key for key in self.nations.keys()]
            i = np.random.randint(0, len(nations) - 1)
            nation = nations[i]
        self.nations[nation].initiate_infection()

    def plot_infections(self):
        sns.lineplot(
            'Day',
            'num_infected',
            hue='Nation',
            data=self.daily_log
        )
        return plt