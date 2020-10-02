// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// test_params
List test_params(int days, unsigned long pop_size, int sociability, double resistance, List virusParams);
RcppExport SEXP _Rpandemic_test_params(SEXP daysSEXP, SEXP pop_sizeSEXP, SEXP sociabilitySEXP, SEXP resistanceSEXP, SEXP virusParamsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type days(daysSEXP);
    Rcpp::traits::input_parameter< unsigned long >::type pop_size(pop_sizeSEXP);
    Rcpp::traits::input_parameter< int >::type sociability(sociabilitySEXP);
    Rcpp::traits::input_parameter< double >::type resistance(resistanceSEXP);
    Rcpp::traits::input_parameter< List >::type virusParams(virusParamsSEXP);
    rcpp_result_gen = Rcpp::wrap(test_params(days, pop_size, sociability, resistance, virusParams));
    return rcpp_result_gen;
END_RCPP
}
// try_lockdown
List try_lockdown(int days, List popParams, List virusParams, unsigned long sensitivity, double aggressiveness);
RcppExport SEXP _Rpandemic_try_lockdown(SEXP daysSEXP, SEXP popParamsSEXP, SEXP virusParamsSEXP, SEXP sensitivitySEXP, SEXP aggressivenessSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type days(daysSEXP);
    Rcpp::traits::input_parameter< List >::type popParams(popParamsSEXP);
    Rcpp::traits::input_parameter< List >::type virusParams(virusParamsSEXP);
    Rcpp::traits::input_parameter< unsigned long >::type sensitivity(sensitivitySEXP);
    Rcpp::traits::input_parameter< double >::type aggressiveness(aggressivenessSEXP);
    rcpp_result_gen = Rcpp::wrap(try_lockdown(days, popParams, virusParams, sensitivity, aggressiveness));
    return rcpp_result_gen;
END_RCPP
}
// generate_world
List generate_world(List popParams, List virusParams);
RcppExport SEXP _Rpandemic_generate_world(SEXP popParamsSEXP, SEXP virusParamsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< List >::type popParams(popParamsSEXP);
    Rcpp::traits::input_parameter< List >::type virusParams(virusParamsSEXP);
    rcpp_result_gen = Rcpp::wrap(generate_world(popParams, virusParams));
    return rcpp_result_gen;
END_RCPP
}
// increment_days
List increment_days(int start_date, int end_date, SEXP population);
RcppExport SEXP _Rpandemic_increment_days(SEXP start_dateSEXP, SEXP end_dateSEXP, SEXP populationSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type start_date(start_dateSEXP);
    Rcpp::traits::input_parameter< int >::type end_date(end_dateSEXP);
    Rcpp::traits::input_parameter< SEXP >::type population(populationSEXP);
    rcpp_result_gen = Rcpp::wrap(increment_days(start_date, end_date, population));
    return rcpp_result_gen;
END_RCPP
}
// change_lockdown
SEXP change_lockdown(SEXP population, int new_sociability);
RcppExport SEXP _Rpandemic_change_lockdown(SEXP populationSEXP, SEXP new_sociabilitySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type population(populationSEXP);
    Rcpp::traits::input_parameter< int >::type new_sociability(new_sociabilitySEXP);
    rcpp_result_gen = Rcpp::wrap(change_lockdown(population, new_sociability));
    return rcpp_result_gen;
END_RCPP
}
// generate_population
List generate_population(List popParams, List virusParams);
RcppExport SEXP _Rpandemic_generate_population(SEXP popParamsSEXP, SEXP virusParamsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< List >::type popParams(popParamsSEXP);
    Rcpp::traits::input_parameter< List >::type virusParams(virusParamsSEXP);
    rcpp_result_gen = Rcpp::wrap(generate_population(popParams, virusParams));
    return rcpp_result_gen;
END_RCPP
}
// subpop_memberships
List subpop_memberships(SEXP population);
RcppExport SEXP _Rpandemic_subpop_memberships(SEXP populationSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type population(populationSEXP);
    rcpp_result_gen = Rcpp::wrap(subpop_memberships(population));
    return rcpp_result_gen;
END_RCPP
}
// subpop_connections
List subpop_connections(SEXP population);
RcppExport SEXP _Rpandemic_subpop_connections(SEXP populationSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type population(populationSEXP);
    rcpp_result_gen = Rcpp::wrap(subpop_connections(population));
    return rcpp_result_gen;
END_RCPP
}
// subpop_infections
NumericVector subpop_infections(SEXP population);
RcppExport SEXP _Rpandemic_subpop_infections(SEXP populationSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type population(populationSEXP);
    rcpp_result_gen = Rcpp::wrap(subpop_infections(population));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_Rpandemic_test_params", (DL_FUNC) &_Rpandemic_test_params, 5},
    {"_Rpandemic_try_lockdown", (DL_FUNC) &_Rpandemic_try_lockdown, 5},
    {"_Rpandemic_generate_world", (DL_FUNC) &_Rpandemic_generate_world, 2},
    {"_Rpandemic_increment_days", (DL_FUNC) &_Rpandemic_increment_days, 3},
    {"_Rpandemic_change_lockdown", (DL_FUNC) &_Rpandemic_change_lockdown, 2},
    {"_Rpandemic_generate_population", (DL_FUNC) &_Rpandemic_generate_population, 2},
    {"_Rpandemic_subpop_memberships", (DL_FUNC) &_Rpandemic_subpop_memberships, 1},
    {"_Rpandemic_subpop_connections", (DL_FUNC) &_Rpandemic_subpop_connections, 1},
    {"_Rpandemic_subpop_infections", (DL_FUNC) &_Rpandemic_subpop_infections, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_Rpandemic(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
