subroutine wrap_pop_c(pop_c, pop_size) bind(c, name='defineFortranPop')
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

subroutine wrap_subpops_c(subpops_c, pop_c, n_subpops, sociability) bind(c, name='defineFortranSubPops')

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

subroutine wrap_families_c(families_c, pop_c, n_families, avg_family_size) bind(c, name='defineFortranFamilies')

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


subroutine mingle_pop_c(pop_c, subpops_c, n_subpops, c_date, new_infected) bind(c, name='mingleFortranPop')
	
	use entities
	use iso_c_binding

	type(c_ptr), intent(inout) :: pop_c
	type(c_ptr), intent(in) :: subpops_c
	integer(c_int), intent(in) :: c_date, n_subpops
	integer(c_long), intent(out) :: new_infected
	type(population), pointer :: pop_f
	type(sub_pop), dimension(:), pointer :: subpops_f
	integer :: date

	call c_f_pointer(pop_c, pop_f)
	call c_f_pointer(subpops_c, subpops_f, [n_subpops])

	date = c_date + 1 ! convert c date (0-indexed) to fortran date (1-indexed)
	call mingle(pop_f, subpops_f, date)
	call update(pop_f, date)
	new_infected = get_num_infected(pop_f) ! TODO this is WRONG and needs to be updated!!!

!	c_ptr = c_loc(pop_f)

end subroutine mingle_pop_c

subroutine wrap_c_pathogen(pop_c, contagious, mortality, length, latency, date, ind) &
	 bind(c, name='initFortranInfection')


	
	use entities
	use iso_c_binding

	
	type(c_ptr), intent(inout) :: pop_c

	real(kind=c_double), intent(in) :: contagious, mortality
	integer(kind=c_int), intent(in) :: length, latency, date, ind

	type(pathogen), target, save :: pathogen_f
	type(population), pointer :: pop_f

	pathogen_f%contagiousness = contagious
	pathogen_f%mortality_rate = mortality
	pathogen_f%disease_length = length
	pathogen_f%latent_period = latency

	call c_f_pointer(pop_c, pop_f)
	call initiate_infection(pop_f, ind, date, pathogen_f)


end subroutine wrap_c_pathogen
