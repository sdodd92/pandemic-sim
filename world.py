import seaborn as sns
from matplotlib import pyplot as plt
import pandas as pd
from core import *


class Nation(Community):
    def __init__(self, pop_size, avg_compliance, avg_age, sociability, base_resistance, gdp, hospital_beds):
        super().__init__(sociability)
        self.gdp = gdp
        compliances = (np.random.uniform(size=pop_size) < avg_compliance).astype(int)
        self.population = [Person(compliance, base_resistance) for compliance in compliances]
        self.sociability = sociability
        self.border_open = True
        self.avg_age = avg_age
        self.avg_compliance = avg_compliance
        self.hospital_beds = hospital_beds
        self.available_hospital_beds = self.hospital_beds

    def plot_age_distribution(self):
        sns.distplot(self.get_ages())
        return plt

    def impose_lockdown(self, mandated_reduction):
        sociability_reduction = mandated_reduction * self.avg_compliance
        self.gdp *= 1 - mandated_reduction
        self.sociability *= 1 - sociability_reduction

    def hospitalize_sick(self):
        for person in self.population:
            if person.sick and self.available_hospital_beds > 0:
                person.resistance *= 1.2
                self.available_hospital_beds -= 1

class Border:
    @staticmethod
    def exchange(crossing_volume, a, b):
        country_a, country_b = a, b
        for crossing in range(crossing_volume):
            pop_size = country_a.pop_size
            if pop_size > 0:
                try:  # handle case of population == 1
                    member_to_cross = np.random.randint(0, country_a.pop_size - 1)
                except ValueError:
                    member_to_cross = 0
                country_b.add_member(country_a.population[member_to_cross])
                country_a.remove_member(member_to_cross)

    @staticmethod
    def share(person_id, a, b):
        country_a, country_b = a, b
        person_to_share = country_a.population[person_id]
        country_b.add_member(person_to_share)

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
                num_infected = nation.mingle(self.day)
                num_died = nation.update_health(self.day)
                # num_died = 0
                if log:
                    log_entry = pd.DataFrame({
                        'Day': [self.day],
                        'Nation': [name],
                        'num_infected': [float(num_infected)],
                        'num_died': [float(num_died)]
                    })
                    self.daily_log = self.daily_log.append(log_entry)
                if verbose:
                    print('{} days elapsed'.format(day))
            if self.borders is not None:
                for border in self.borders:
                    border.direct_exchange(10)

    # todo refactor plotting logic to put on a nation-by-nation level
    def initiate_infection(self, pathogen, nation=None):
        if nation is None:
            nations = [key for key in self.nations.keys()]
            i = np.random.randint(0, len(nations))
            nation = nations[i]
        self.nations[nation].initiate_infection(pathogen, date=self.day)

    def plot_daily_infections(self, ax=None, nation=None):
        if nation is None:
            sns.lineplot(
                'Day',
                'num_infected',
                hue='Nation',
                data=self.daily_log,
                ax=ax
            )
        else:
            sns.lineplot(
                'Day',
                'num_infected',
                data=self.daily_log.loc[self.daily_log['Nation'] == nation],
                ax=ax
            )
        return plt

    def plot_total_infections(self, ax=None):
        daily_log = self.daily_log.copy()
        daily_log['cumu_infected'] = daily_log.groupby('Nation')['num_infected'].cumsum()
        sns.lineplot(
            'Day',
            'cumu_infected',
            hue='Nation',
            data=daily_log,
            ax=ax
        )
        return plt

    def plot_daily_deaths(self, ax=None):
        sns.lineplot(
            'Day',
            'num_died',
            hue='Nation',
            data=self.daily_log,
            ax=ax
        )
        return plt

    def plot_total_deaths(self, ax=None, nation=None, legend=True):
        daily_log = self.daily_log.copy()
        daily_log['cumu_died'] = daily_log.groupby('Nation')['num_died'].cumsum()
        if nation is None:
            sns.lineplot(
                'Day',
                'cumu_died',
                hue='Nation',
                data=daily_log,
                ax=ax
            )
        else:
            daily_log.loc[daily_log['Nation'] == nation].plot('Day', 'cumu_died', legend=legend, ax=ax)
        return plt