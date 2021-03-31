subroutine wrap_pop_c(pop_c, pop_size) bind(c, name='allocatePopulation')
	use entities
	use iso_c_binding

	type(c_ptr), intent(out) :: pop_c 
!	type(population), target, save :: base_pop
	type(population), pointer :: pop_f

	integer, intent(in) :: pop_size
	allocate(pop_f)

	call define_pop(pop_f, pop_size)

	pop_c = c_loc(pop_f)

end subroutine wrap_pop_c

subroutine wrap_subpops_c(subpops_c, pop_c, n_subpops, sociability) bind(c, name='allocateSubPops')

	use entities
	use iso_c_binding

	type(c_ptr), intent(in) :: pop_c 
	type(c_ptr), intent(out) :: subpops_c
	
	integer(kind=c_long), intent(in) :: n_subpops
	integer(kind=c_int), intent(in) :: sociability

!	type(sub_pop), dimension(n_subpops) :: subpops
	type(sub_pop), dimension(:), pointer :: subpops_f
	type(population), pointer :: pop_f

	call c_f_pointer(pop_c, pop_f)

	allocate(subpops_f(n_subpops))
	
	call define_subpops(pop_f, subpops_f, sociability)

!	subpops_f => subpops
	subpops_c = c_loc(subpops_f)

end subroutine wrap_subpops_c

subroutine wrap_families_c(families_c, pop_c, n_families, avg_family_size) bind(c, name='allocateFamilies')

	use entities
	use iso_c_binding

	type(c_ptr), intent(in) :: pop_c
	type(c_ptr), intent(out) :: families_c

	integer(kind=c_int), intent(in) :: avg_family_size
	integer(kind=c_long), intent(out) :: n_families

	type(population), pointer :: pop_f
	type(sub_pop), dimension(:), allocatable, target, save :: families_f  
	!TODO decide if save is ok here (should be fine I think)...

	real :: avg_family_size_real

	call c_f_pointer(pop_c, pop_f)
	
	avg_family_size_real=avg_family_size
	call define_families(pop_f, families_f, avg_family_size_real)

!	families_ptr => families_f
	families_c = c_loc(families_f)
	n_families = size(families_f)
	

end subroutine wrap_families_c


subroutine mingle_pop_c(pop_c, subpops_c, n_subpops, c_date, new_infected) bind(c, name='minglePopulation')
	
	use entities
	use iso_c_binding

	type(c_ptr), intent(inout) :: pop_c
	type(c_ptr), intent(in) :: subpops_c
	integer(c_int), intent(in) :: c_date, n_subpops
	integer(c_long), intent(out) :: new_infected
	type(population), pointer :: pop_f
	type(sub_pop), dimension(:), pointer :: subpops_f
	integer :: date, tmp_new_infected

	call c_f_pointer(pop_c, pop_f)
	call c_f_pointer(subpops_c, subpops_f, [n_subpops])

	date = c_date + 1 ! convert c date (0-indexed) to fortran date (1-indexed)
	call mingle(pop_f, subpops_f, date, tmp_new_infected)

	new_infected = tmp_new_infected

!	c_ptr = c_loc(pop_f)

end subroutine mingle_pop_c

subroutine update_pop_c(pop_c, c_date, new_dead, n_recovered) bind(c, name='updatePopulation')

	use entities
	use iso_c_binding

	type(c_ptr), intent(inout) :: pop_c
	integer(c_int), intent(in) :: c_date
	integer(c_long), intent(out) :: new_dead, n_recovered

	type(population), pointer :: pop_f
	integer :: date, tmp_new_dead, tmp_recovered

	call c_f_pointer(pop_c, pop_f)
	date = c_date + 1

	call update(pop_f, date, tmp_new_dead, tmp_recovered)
	
	new_dead = tmp_new_dead
	n_recovered = tmp_recovered	

end subroutine update_pop_c

subroutine wrap_c_pathogen(pop_c, contagious, mortality, length, latency, date, ind) &
	 bind(c, name='initiateInfection')


	
	use entities
	use iso_c_binding

	
	type(c_ptr), intent(inout) :: pop_c

	real(kind=c_double), intent(in) :: contagious, mortality
	integer(kind=c_int), intent(in) :: length, latency, date, ind

	type(pathogen), target, save :: pathogen_f
	type(population), pointer :: pop_f
	integer :: f_date, f_ind

	! convert from zero-indexing to 1-indexing
	f_date = date + 1
	f_ind = ind + 1

	pathogen_f%contagiousness = contagious
	pathogen_f%mortality_rate = mortality
	pathogen_f%disease_length = length
	pathogen_f%latent_period = latency

	call c_f_pointer(pop_c, pop_f)
	call initiate_infection(pop_f, f_ind, f_date, pathogen_f)


end subroutine wrap_c_pathogen

function get_number_infected(pop_c) result(n_infected) bind(c, name='getCurrentInfected')
	use entities
	use iso_c_binding

	type(c_ptr), intent(in) :: pop_c
	integer(kind=c_long) :: n_infected

	type(population), pointer :: pop_f

	call c_f_pointer(pop_c, pop_f)

	n_infected = count(pop_f%infected > 0)	


end function get_number_infected

function get_number_dead(pop_c) result(n_dead) bind(c, name='getNumDied')

	use entities
	use iso_c_binding

	type(c_ptr), intent(in) :: pop_c
	integer(kind=c_long) :: n_infected

	type(population), pointer :: pop_f

	call c_f_pointer(pop_c, pop_f)

	n_dead = count(.not. pop_f%alive)


end function get_number_dead

subroutine update_spreader_factors(subpops_c, new_spreader_factor, n_subpops) bind(c, name='updateSpreaderFactor')
	use entities
	use iso_c_binding

	type(c_ptr), intent(inout) :: subpops_c
	real(kind=c_float), intent(in) :: new_spreader_factor
	integer(kind=c_long), intent(in) :: n_subpops

	type(sub_pop), dimension(:), pointer :: subpops_f

	integer :: i


	call c_f_pointer(subpops_c, subpops_f, [n_subpops])
	
	do i=1, size(subpops_f)
		subpops_f(i)%spreader_factor = new_spreader_factor
	end do


end subroutine update_spreader_factors

subroutine update_sociabilities(subpops_c, new_sociability, n_subpops) bind(c, name='updateSociability')
	use entities
	use iso_c_binding

	type(c_ptr), intent(inout) :: subpops_c
	real(kind=c_float), intent(in) :: new_sociability
	integer(kind=c_long), intent(in) :: n_subpops

	type(sub_pop), dimension(:), pointer :: subpops_f

	integer :: i


	call c_f_pointer(subpops_c, subpops_f, [n_subpops])
	
	do i=1, size(subpops_f)
		subpops_f(i)%sociability = new_sociability
	end do

end subroutine update_sociabilities

subroutine nullify_pop(pop_c) bind(c, name='nullifyPopulation')

	use entities
	use iso_c_binding

	type(c_ptr), intent(inout) :: pop_c
	type(population), pointer :: pop_f

	call c_f_pointer(pop_c, pop_f)

	nullify(pop_f)

end subroutine nullify_pop
