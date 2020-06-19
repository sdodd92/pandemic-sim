
#include <Rcpp.h>

#ifndef _PANDEM_INTERFACE
#define _PANDEM_INTERFACE

template<typename T>
void unpack_ptr(SEXP ptr_wrapper, T** target) {

  Rcpp::XPtr<T> r_ptr(ptr_wrapper);
  *target = r_ptr.get();


}




#endif
