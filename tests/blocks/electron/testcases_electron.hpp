#pragma once

#include <xdiag/operators/opsum.hpp>
#include <xdiag/symmetries/operations/symmetry_operations.hpp>
#include <xdiag/symmetries/permutation.hpp>
#include <xdiag/symmetries/permutation_group.hpp>
#include <xdiag/symmetries/representation.hpp>

#include <random>

namespace xdiag::testcases::electron {

OpSum get_linear_chain(int64_t n_sites, double t, double U);

OpSum get_linear_chain_hb(int64_t n_sites, double J);

std::vector<Representation> get_cyclic_group_irreps(int64_t n_sites);

std::tuple<std::vector<Representation>, std::vector<int64_t>>
get_cyclic_group_irreps_mult(int64_t n_sites);

OpSum heisenberg_triangle();

OpSum heisenberg_alltoall(int64_t n_sites);

OpSum heisenberg_kagome15();
OpSum heisenberg_kagome39();

OpSum freefermion_alltoall(int64_t n_sites);

OpSum freefermion_alltoall_complex_updn(int64_t n_sites);

// OpSum tJchain(int64_t n_sites, double t, double J);

std::tuple<OpSum, arma::Col<double>> randomAlltoAll4NoU();
std::tuple<OpSum, arma::Col<double>> randomAlltoAll4();

OpSum randomAlltoAll3();

OpSum square2x2(double t, double J);

OpSum square3x3(double t, double J);

} // namespace xdiag::testcases::electron
