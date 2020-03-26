"""
from base_entities import *
import pandas as pd

uk = Nation(1000, 0.5, 25, 5)
uk.plot_age_distribution()
plt.show()

print(uk.get_avg_immunity())

ages = uk.get_ages()
immunities = uk.get_immunities()

age_curve = pd.DataFrame(
    {
        'age_group': np.round(ages, -1),
        'immunity': immunities
    }
)

age_curve = age_curve.groupby('age_group').mean()

age_curve.plot()
plt.show()


world = World()
world.init_nation('UK', uk)
world.initiate_infection('UK')
world.new_day(10)
world.plot_infections()
plt.show()
"""


from base_entities import *

world = World()

world.init_nation(
    'Not Sociable',
    pop_size=1000,
    avg_compliance=.5,
    avg_age=20,
    sociability=2
)

world.init_nation(
    'Somewhat Sociable',
    pop_size=1000,
    avg_compliance=.5,
    avg_age=20,
    sociability=5,
    borders=['Not Sociable', 'Very Sociable']
)

world.init_nation(
    'Very Sociable',
    pop_size=1000,
    avg_compliance=.5,
    avg_age=20,
    sociability=20
)

world.init_borders()

# world.initiate_infection('Not Sociable')
world.initiate_infection('Somewhat Sociable')
# world.initiate_infection('Very Sociable')

world.new_day(days=90, verbose=False)
world.plot_infections()
plt.show()