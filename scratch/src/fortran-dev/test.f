

program test_pops

use entities


!subroutine start_infection(pop, i)
!	type(population), intent(out) :: pop
!	integer, intent(in) :: i
!
!	associate (target => pop%contagious(i)) 
!		call random_number(pop%contagious(i))
!	end associate
!end subroutine initiate_infection


integer, parameter :: POP_SIZE = 1000000
integer, parameter :: SOCIABILITY = 20
integer, parameter :: N_COMMUNITIES = 200000
integer, parameter :: N_DAYS = 30

real, parameter :: COMPLIANCE = 0.5
real, parameter :: RESISTANCE = 0.5

integer :: d, num_infected(N_DAYS), num_died(N_DAYS)

type(pathogen), target :: virus

real, parameter :: CONTAGIOUSNESS=0.5, MORTALITY_RATE=0.02
integer, parameter :: DISEASE_LENGTH=31, LATENT_PERIOD=2

type(population) :: mypop
type(sub_pop), dimension(N_COMMUNITIES) :: subpops

virus%contagiousness = CONTAGIOUSNESS
virus%mortality_rate = MORTALITY_RATE
virus%disease_length = DISEASE_LENGTH
virus%latent_period = LATENT_PERIOD
!virus%incubation_period = INCUBATION_PERIOD


call random_seed


call define_pop(mypop, POP_SIZE)
call define_subpops(mypop, subpops, SOCIABILITY)


!print *, size(mypop%contagious)
!call random_number(mypop%contagious(1))
call initiate_infection(mypop, 1, 1, virus)
!call check_member(mypop, 1)
!
!call initiate_infection(mypop, 100)
!call check_member(mypop, 100)

!print *, count(mypop%infected)
!print *, get_num_infected(mypop)

!$omp parallel
do d = 1, N_DAYS
	call mingle(mypop, subpops, d)
	call update(mypop, d)
	num_infected(d) = get_num_infected(mypop)
	num_died(d) = get_num_dead(mypop)
end do
!$omp end parallel



!call check_member(mypop, 1)

do i=1, N_DAYS
	print *, i, "," , num_infected(i), ",", num_died(i)
end do

!do i=1, N_COMMUNITIES
!	print *, i, ",", size(subpops(i)%members)
!end do

!=====================================================

CONTAINS

subroutine check_member(pop, ind)
	type(population), intent(in) :: pop
	integer, intent(in) :: ind


	print *, pop%infected(ind)
	print *, pop%contagious(ind)

end subroutine check_member



end program test_pops

