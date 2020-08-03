import numpy as np
import seaborn as sns
import pandas as pd
# np.random.seed(5)


class Network:
    def __init__(self, raw_network: dict):
        self.raw_network = raw_network

    def as_table(self):
        output = pd.DataFrame(columns=['Source', 'Target', 'weight'])
        for node_from in self.raw_network.keys():
            for node_to in self.raw_network[node_from].keys():
                weight = self.raw_network[node_from][node_to]
                if weight > 0:
                    output = output.append(pd.DataFrame({'Source': [node_from], 'Target': [node_to], 'weight': [weight]}))
        return output

class Person:
    uid = 0

    @classmethod
    def inc_uid(cls):
        cls.uid += 1

    def __init__(self):
        self.uuid = self.uid
        self.inc_uid()
        self.immune = False
        self.infected = False
        self.alive = True

    def __eq__(self, other):
        if isinstance(other, Person):
            return self.uuid == other.uuid
        else:
            return other == self

    def __ne__(self, other):
        if isinstance(other, Person):
            return self.uuid != other.uuid
        else:
            return other != self

    def get_disease(self, interactee):
        if interactee.infected:
            self.infected = True

class Community:
    def __init__(self, sociability, pop_size: int=None):
        self.population = []
        self.pop_uids = []
        self.sociability = sociability
        self.pop_size = 0
        if pop_size:
            self.pop_size = pop_size
            for i in range(pop_size):
                self.add_person(Person())

    def interact(self, i, j):
        self.population[i].get_disease(self.population[j])

    def mingle(self):
        pop_size = len(self.population)
        for i in range(pop_size):
            if self.sociability <= 0:
                interactions = pop_size
            else:
                interactions = int(np.random.poisson(self.sociability, 1))
            for interaction in range(interactions):
                j = np.random.randint(0, pop_size - 1)
                while j == i:
                    j = np.random.randint(0, pop_size - 1)
                self.interact(i, j)

    def add_person(self, new_person: Person=None):
        if new_person is None:
            new_person = Person()
        if new_person.uuid in self.pop_uids:
            return False
        else:
            self.population.append(new_person)
            self.pop_uids.append(new_person.uuid)
            return True

    def remove_person(self, member_index: int):
        del self.population[member_index]

    def kill(self, kill_prob: float):
        for person in self.population:
            if np.random.uniform() < kill_prob:
                person.alive = False

    def count_dead(self):
        return sum([not person.alive for person in self.population])


class Border:
    @staticmethod
    def share(country_a: Community, country_b: Community, member_index: int=None):
        if member_index is None:
            member_index = np.random.randint(0, country_a.pop_size - 1)
        result = country_b.add_person(country_a.population[member_index])
        return result

    def __init__(self, country_a: Community, country_b: Community):
        self.country_a = country_a
        self.country_b = country_b

    def exchange(self, member_index=None):
        member_index = self.share(self.country_a, self.country_b, member_index)
        self.country_a.remove_person(member_index)

class SubCommunity(Community):
    """
    special case of the population specifically designed as a sub-community within a larger nation.
    In this case, all members interact with all other members with certain (equal) probability
    """
    def __init__(self, interactiveness: float, pop_size: int=None):
        super().__init__(-1, pop_size)
        self.interactiveness = interactiveness
        self.active = {i: True for i in range(pop_size)}

    def mingle(self):
        pop_size = len(self.population)
        for i in range(pop_size):
            for j in range(pop_size):
                # determine if the alternate interaction selection logic is actually necessary...
                if i != j and self.active[i] and self.active[j] and np.random.uniform() < self.interactiveness:  # room for algorithm optimization here
                    self.interact(i, j)

    def inactivate(self, id: int):
        self.active[id] = False

    def reactivate(self, id: int):
        self.active[id] = True

class SuperCommunity(Community):
    def __init__(self, pop_size: int, clumpiness: float):
        super().__init__(0, pop_size)  # generate "master population" as normal

        # split into sub-communities
        un_clumped = pop_size  # count how much of the population has not yet been allocated
        avg_community_size = int(pop_size * clumpiness)
        subcommunities = []
        clumped_pop = []

        while len(clumped_pop) < pop_size:  # do until the entire population has been allocated
            community_size = np.random.poisson(avg_community_size, 1)[0]  # as a naive method draw pop size from poisson (this is dumb and should be changed)
            new_community = Community(0)  # initialize the new community with 100% interaction
            for n in range(community_size):  # populate the community
                success = False
                while not success:  # re-draw if the person has already been allocated in that community
                    i = np.random.randint(0, pop_size, 1)[0]
                    success = Border.share(self, new_community, i)
                # END while not success

                if i  not in clumped_pop:
                    clumped_pop.append(i)
                un_clumped -= 1  # increment the allocation counter after successful allocation
            # END for n in range(community_size)

            # after allocating all members, solidify the community and move on to the next one
            subcommunities.append(new_community)
            print(len(clumped_pop), end='\r')
        # END while un_clumped > 0

        self.subcommunities = subcommunities
        self.clumped_pop = clumped_pop

    def get_network(self):
        network = {}
        for node_from in self.pop_uids:
            network[node_from] = {}
            for node_to in self.pop_uids:
                if node_to not in network:
                    network[node_from][node_to] = 0
                    for community in self.subcommunities:
                        uids = community.pop_uids
                        if node_from in uids and node_to in uids:
                            network[node_from][node_to] += 1
        return Network(network)

    def mingle(self):
        for subpop in self.subcommunities:
            subpop.mingle()

class CommunityNetwork(Community):
    def __init__(self, pop_size: int):
        super().__init__(0, pop_size)  # generate un-allocated overall population pool

        network = {}

        # iteratively draw connections between players
        for person_1 in self.pop_uids:
            network[person_1] = {}
            for person_2 in self.pop_uids:
                network[person_1][person_2] = np.random.uniform()

        self.interaction_network = network

    def mingle(self):
        for i, source in self.interaction_network.items():
            for j, weight in source.items():
                if np.random.uniform() < weight:
                    self.interact(i, j)

    def get_network(self):
        return Network(self.interaction_network)



nation_test = SuperCommunity(100, .05)

print(sum([len(sub.population) for sub in nation_test.subcommunities]))
print(sum([len(np.unique([member.uuid for member in sub.population])) for sub in nation_test.subcommunities]))


sizes = [len(np.unique([member.uuid for member in sub.population])) for sub in nation_test.subcommunities]
sns.distplot(sizes)

memberships = []
for uid in nation_test.pop_uids:
    membership = 0
    for subcommunity in nation_test.subcommunities:
        if uid in subcommunity.pop_uids:
            membership += 1
    memberships.append(membership)

sns.distplot(memberships)


nation_test.kill(.2)
deaths = [sub.count_dead() for sub in nation_test.subcommunities]
sns.distplot(deaths)



len(np.unique(nation_test.clumped_pop))

"""
network_table = Network(nation_test.get_network()).as_table()
network_table.to_csv('/home/sam/Documents/pandemic-sim/debug/pop-network.csv', index=False)
"""
