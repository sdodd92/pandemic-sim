module utils



contains

function funPoissonSingle(lambda) result(randPoisson)
    implicit none
	real, intent(in) :: lambda !input
	real :: exp_lambda !constant for terminating loop
	real :: randUni !uniform variable 
	real :: prodUni !product of uniform variables
	integer :: randPoisson !Poisson variable

    !declare functions
    real funUniformSingle
    
	exp_lambda= exp(-lambda) !calculate constant

	!initialize variables
	randPoisson = -1
	prodUni = 1
	do while (prodUni > exp_lambda)       
		 call random_number(randUni) !generate uniform variable
		 prodUni = prodUni * randUni !update product
		 randPoisson = randPoisson + 1 !increase Poisson variable
	end do  
end function



end module utils
