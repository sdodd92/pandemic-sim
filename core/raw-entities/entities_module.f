module entities


use utils
implicit none


type family_structure
	logical, dimension(:), pointer :: infected, sick, contagious, alive
end type family_structure

type sub_pop
	integer, dimension(:), allocatable :: members
	real :: spreader_factor = 0. 
end type sub_pop

type pathogen
	integer :: latent_period, incubation_period, disease_length, strain_id
	real :: contagiousness, mortality_rate
end type pathogen

type pathogen_ptr
	type(pathogen), pointer :: ptr
end type pathogen_ptr


type population
	logical, dimension(:), allocatable :: alive, immune
	real, dimension(:), allocatable :: contagious, compliant
	integer, dimension(:), allocatable :: infected, sick
	integer :: pop_size
	type(pathogen_ptr), dimension(:), allocatable :: infection
end type population

contains



subroutine define_pop(pop, pop_size)
	integer, intent(in) :: pop_size
	type(population), intent(out) :: pop


	allocate(pop%infected(pop_size))
	pop%infected = 0

	allocate(pop%sick(pop_size))
	pop%sick = 0

	allocate(pop%alive(pop_size))
	pop%alive = .true.

	allocate(pop%immune(pop_size))
	pop%immune = .false.

	
	allocate(pop%contagious(pop_size))
	pop%contagious = 0.0

	allocate(pop%compliant(pop_size))
	pop%compliant = 1.0

	allocate(pop%infection(pop_size))

	pop%pop_size = pop_size

end subroutine define_pop

subroutine allocate_subpop(subpop, lower_bound, upper_bound, n_members)
	type(sub_pop), intent(inout) :: subpop
	integer, intent(in) :: lower_bound, upper_bound, n_members

	real :: seed_val, n_members_real

	integer :: i, j, new_val
	logical :: success

	integer :: rand_members

	n_members_real = n_members

	rand_members = funPoissonSingle(n_members_real)

	allocate(subpop%members(rand_members))

	associate (members => subpop%members)
		members = 0
		
		do i=1, rand_members
			
			success = .false.
			do while (.not. success)
				call random_number(seed_val)
				new_val = (lower_bound + floor((upper_bound+1-lower_bound) * seed_val))
				if (.not. any(members == new_val)) then
					members(i) = new_val
					success = .true.
				end if
			end do
		end do
	end associate
			 

end subroutine allocate_subpop

	


subroutine define_subpops(pop, subpops, avg_pop_size)
	type(population), intent(in) :: pop
	type(sub_pop), dimension(:), intent(inout) :: subpops
	integer, intent(in) :: avg_pop_size

	integer :: i

	!$omp  parallel do
	do i=1, size(subpops)
		call allocate_subpop(subpops(i), 1, pop%pop_size, avg_pop_size)
	end do

end subroutine define_subpops 

subroutine define_families(pop, families, avg_family_size)

	type(population), intent(in) :: pop
	type(sub_pop), dimension(:), allocatable, intent(out) :: families
	real, intent(in) :: avg_family_size

	logical, dimension(:) :: family_boundary(pop%pop_size)

	integer, dimension(:), allocatable :: lower_bounds, upper_bounds	

	integer :: i, j, lower_bound=1, upper_bound=1, num_families=0, family_size


	family_boundary = .false.
	

	do while (upper_bound .lt. pop%pop_size)
		family_size = funPoissonSingle(avg_family_size)
		upper_bound = lower_bound + family_size
		if (upper_bound > pop%pop_size) then 
			upper_bound = pop%pop_size
		end if
		family_boundary(upper_bound) = .true.
		num_families = num_families + 1
		lower_bound = upper_bound + 1
	end do

	allocate(families(num_families))
	allocate(lower_bounds(num_families))
	allocate(upper_bounds(num_families))

	lower_bounds(1) = 1
	j = 1
	do i=1, pop%pop_size
		if (family_boundary(i)) then
			upper_bounds(j) = i
			if (j > 1) then
				lower_bounds(j) = i + 1
			end if
			j = j + 1
		end if
	end do

	
	do i=1, num_families
		associate(lower_bound => lower_bounds(i), upper_bound => upper_bounds(i))
			family_size = upper_bound - lower_bound + 1
			allocate(families(i)%members(family_size))
			do j=1, family_size
				families(i)%members(j) = lower_bound + j - 1
			end do
		end associate
	end do
			


end subroutine define_families
		



function get_num_infected(pop) result(count_infected)
	type(population), intent(in) :: pop
	integer :: count_infected

	count_infected = count(pop%infected .gt. 0)

end function get_num_infected

function get_num_dead(pop) result(count_dead)
	type(population), intent(in) :: pop
	integer :: count_dead

	count_dead = count(.not. pop%alive)
end function get_num_dead

subroutine mingle_subpop(pop, subpop, date)
	type(population), intent(inout) :: pop
	type(sub_pop), intent(in) :: subpop
	integer, intent(in) :: date

	real :: effective_contagious(size(subpop%members))
	real, dimension(:), allocatable :: roll_to_infect
	real :: contagion_factor
	
	type (pathogen_ptr), dimension(:), allocatable :: valid_infections
	real, dimension(:), allocatable :: valid_contagious

	integer :: i, j,  infection_age, disease_age, count_contagious	

	associate ( 												&
			infected => pop%infected(subpop%members(:)), 		&
			contagious => pop%contagious(subpop%members(:)), 	&
			immune => pop%immune(subpop%members(:)),			&
			infections => pop%infection(subpop%members(:)),		&
			alive => pop%alive(subpop%members(:))				&
		)
			!compute overall infection probability by combining all contagiousness factors
			!remember to exclude infections which are still within their latency period

			effective_contagious = contagious * (1 + subpop%spreader_factor)
			contagion_factor = 1 - product(1 - effective_contagious)


			count_contagious = count(effective_contagious > 0)
			allocate(valid_infections(count_contagious))
			valid_infections = pack(infections, contagious > 0)	

			allocate(valid_contagious(count_contagious))
			valid_contagious = pack(effective_contagious, effective_contagious > 0)

			
			if (contagion_factor > 0) then
				allocate(roll_to_infect(size(subpop%members)))
				call random_number(roll_to_infect)
				do i=1, size(subpop%members)
					associate (									&
						member_infected => infected(i), 		&
						member_roll => roll_to_infect(i), 		&
						member_alive => alive(i),				&
						member_immune => immune(i),				&
						member => subpop%members(i)				&
					)
						if ((.not. member_immune) .and. member_alive) then 
							do j=1, count_contagious
								!TODO think about what happens with multiple infections
								if (roll_to_infect(i) < valid_contagious(j)) then
									pop%infected(member) = date !remember subpop is an index vector
									pop%infection(member)%ptr => valid_infections(j)%ptr
									pop%immune(member) = .true.
								end if
							end do
						end if
					end associate
				end do  ! j=1, size(subpop)
				deallocate(roll_to_infect)
			end if  ! contagion_factor > 0

			deallocate(valid_infections)
			deallocate(valid_contagious)

		end associate


end subroutine mingle_subpop


subroutine mingle(pop, subpops, date)
	type(population), intent(inout) :: pop
	type(sub_pop), dimension(:), intent(in), target :: subpops
	integer, intent(in) :: date
	


	integer :: i
	
	!$omp parallel do
	do i=1, size(subpops)
		call mingle_subpop(pop, subpops(i), date)
	end do  ! i=1, size(subpops)



end subroutine mingle

subroutine update(pop, date)
	type(population), intent(inout) :: pop
	integer, intent(in) :: date
	
	real :: roll_to_kill(pop%pop_size)
	integer :: disease_age(pop%pop_size)

	real, dimension(:), allocatable :: effective_mortality_rate
 	real :: non_mortality_rate, disease_length_factor
	
	integer :: i, infection_age

	allocate(effective_mortality_rate(pop%pop_size))
	
	!$omp parallel do
	do i=1, pop%pop_size
		associate(													&
			mortality_rate => pop%infection(i)%ptr%mortality_rate, 	&
			disease_length => pop%infection(i)%ptr%disease_length,	&
			infected => pop%infected(i)								&
		)
			if (infected > 0) then
				non_mortality_rate = 1 - mortality_rate
				disease_length_factor = 1. / disease_length
				effective_mortality_rate(i) = 1- non_mortality_rate ** disease_length_factor
			else
				effective_mortality_rate(i) = 0.
			end if
		end associate
	end do
	
	disease_age = date - pop%infected

	call random_number(roll_to_kill)

	!$omp parallel do
	do i=1, pop%pop_size
		associate (												&
			infected => pop%infected(i),						&
			disease_age => disease_age(i),						&
			infection => pop%infection(i)%ptr,					&
			alive => pop%alive(i),								&
			mortality_rate => effective_mortality_rate(i),		&
			kill_roll => roll_to_kill(i) )
			if (infected > 0) then
												
				if (disease_age > infection%latent_period .and.		&
					disease_age <= infection%disease_length .and.	&
					alive ) then

					pop%alive(i) = kill_roll .ge. mortality_rate
					pop%contagious(i) = infection%contagiousness

				elseif (disease_age > infection%disease_length .and. alive) then
					pop%infected(i) = 0
					pop%contagious(i) = 0.
				end if 
			end if
		end associate
	end do

	where (.not. pop%alive)
		pop%contagious = 0.
		pop%infected = 0
	end where
	
	

end subroutine update

subroutine initiate_infection(pop, i, date, infection)
	type(population), intent(inout) :: pop
	integer, intent(in) :: i, date
	type(pathogen), target, intent(in) :: infection

!	print *, "infecting member", i
	pop%infected(i) = date
	pop%infection(i)%ptr => infection

end subroutine initiate_infection





end module entities
