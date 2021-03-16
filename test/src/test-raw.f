program test_entities

use entities
use omp_lib
implicit none


integer, parameter :: POP_SIZE = 10000000
integer, parameter :: SOCIABILITY = 20
integer, parameter :: N_COMMUNITIES = 2000000
integer, parameter :: N_DAYS = 30

real, parameter :: COMPLIANCE = 0.5
real, parameter :: RESISTANCE = 0.5

integer :: i, d, num_infected(N_DAYS), num_died(N_DAYS)

type(pathogen), target :: virus

real, parameter :: CONTAGIOUSNESS=0.05, MORTALITY_RATE=0.02
integer, parameter :: DISEASE_LENGTH=31, LATENT_PERIOD=2
real :: FAMILY_SIZE = SOCIABILITY

type(population) :: mypop
type(sub_pop), dimension(N_COMMUNITIES) :: subpops
!type(sub_pop), dimension(:), allocatable :: families

virus%contagiousness = CONTAGIOUSNESS
virus%mortality_rate = MORTALITY_RATE
virus%disease_length = DISEASE_LENGTH
virus%latent_period = LATENT_PERIOD
!virus%incubation_period = INCUBATION_PERIOD


call random_seed
call omp_set_num_threads(11)

call define_pop(mypop, POP_SIZE)
call define_subpops(mypop, subpops, SOCIABILITY)
!call define_families(mypop, families, FAMILY_SIZE)



call initiate_infection(mypop, 1, 1, virus)
!
!call initiate_infection(mypop, 100)


do d = 1, N_DAYS
	call mingle(mypop, subpops, d, num_infected(d))
!	call mingle(mypop, families, d, num_infected(d))
	call update(mypop, d, num_died(d))
end do





do i=1, N_DAYS
	print *, i, "," , num_infected(i), ",", num_died(i)
end do



end program test_entities
