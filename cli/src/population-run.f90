program population_run


use entities
use omp_lib
use iso_fortran_env, only : output_unit

implicit none

integer, parameter :: POP_SIZE = 1000000
!integer, parameter :: POP_SIZE = 66000000
integer, parameter :: SOCIABILITY = 20
integer, parameter :: N_COMMUNITIES = 200000
!integer, parameter :: N_COMMUNITIES = 13000000
integer, parameter :: WORK_SIZE = 100
integer, parameter :: N_DAYS = 30
integer, parameter :: N_THREADS = 11

real, parameter :: COMPLIANCE = 0.5
real, parameter :: RESISTANCE = 0.5

integer :: date, n_offices, num_infected(N_DAYS), num_died(N_DAYS), new_infections(N_DAYS), recovered(N_DAYS), output_handle

type(pathogen), target :: virus

real, parameter :: CONTAGIOUSNESS=0.05, MORTALITY_RATE=0.02
integer, parameter :: DISEASE_LENGTH=31, LATENT_PERIOD=2
real :: FAMILY_SIZE = SOCIABILITY

type(population) :: mypop
type(sub_pop) :: social(N_COMMUNITIES), work(POP_SIZE / WORK_SIZE)
type(sub_pop), dimension(:), allocatable :: families

logical :: is_weekend = .false.


virus%contagiousness = CONTAGIOUSNESS
virus%mortality_rate = MORTALITY_RATE
virus%disease_length = DISEASE_LENGTH
virus%latent_period = LATENT_PERIOD
!virus%incubation_period = INCUBATION_PERIOD


call random_seed
call omp_set_num_threads(N_THREADS)

call define_pop(mypop, POP_SIZE)
call define_subpops(mypop, social, SOCIABILITY)
call define_subpops(mypop, work, WORK_SIZE)
call define_families(mypop, families, FAMILY_SIZE)


call initiate_infection(mypop, 1, 1, virus)

date = 1
do while (date <= N_DAYS)
	call step_week
end do

!open(1, file='cli-output.csv', status='replace')
!output_handle = 1
output_handle = output_unit

call write_output(output_handle)

!close(1)

CONTAINS !============================================================================================ 


subroutine step_week

if (is_weekend) then
!	print *, 'Starting Weekend'
	call process_step(social, 2, date)
	is_weekend = .false.
else
!	print *, 'Starting Work Week'
	call process_step(work, 5, date)
	is_weekend = .true.
end if	


end subroutine step_week

subroutine process_step(key_subpop, n_steps, date)

	type(sub_pop), dimension(:), intent(in) :: key_subpop
	integer, intent(in) :: n_steps
	integer, intent(inout) :: date

	integer :: i, infected_key_subpop, infected_families

	i = 1

	do while (i <= n_steps .and. date <= N_DAYS)
		!mingle the key-subpop of choice first
		call mingle(mypop, key_subpop, date, infected_key_subpop)
		call mingle(mypop, families, date, infected_families)

		
		call update(mypop, date, num_died(date), recovered(date))
		num_infected(date) = count(mypop%infected .gt. 0)
		new_infections(date) = infected_key_subpop + infected_families

		i = i + 1
		date = date + 1

	end do 
	
	

end subroutine process_step

subroutine write_output(u)

	integer, intent(in) :: u

	integer :: d

	write(u,*) 'DATE,NUM_INFECTED,NEW_INFECTIONS,NUM_DEAD'
	do d=1, N_DAYS
		write(u,'(4(I0 : ","))') d, num_infected(d), new_infections(d), num_died(d)
	end do


end subroutine write_output



end program population_run
