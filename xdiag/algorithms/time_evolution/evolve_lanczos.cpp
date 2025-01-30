#include "evolve_lanczos.hpp"

#include <xdiag/algebra/apply.hpp>
#include <xdiag/algorithms/lanczos/lanczos_convergence.hpp>
#include <xdiag/algorithms/time_evolution/exp_sym_v.hpp>
#include <xdiag/operators/logic/hc.hpp>
#include <xdiag/operators/logic/isapprox.hpp>
#include <xdiag/operators/logic/real.hpp>

namespace xdiag {

evolve_lanczos_result_t evolve_lanczos(OpSum const &H, State psi, double tau,
                                       double precision, double shift,
                                       bool normalize, int64_t max_iterations,
                                       double deflation_tol) try {
  auto r = evolve_lanczos_inplace(H, psi, tau, precision, shift, normalize,
                                  max_iterations, deflation_tol);
  return {r.alphas, r.betas, r.eigenvalues, r.niterations, r.criterion, psi};
} catch (Error const &e) {
  XDIAG_RETHROW(e);
}

evolve_lanczos_result_t evolve_lanczos(OpSum const &H, State psi, complex tau,
                                       double precision, double shift,
                                       bool normalize, int64_t max_iterations,
                                       double deflation_tol) try {
  auto r = evolve_lanczos_inplace(H, psi, tau, precision, shift, normalize,
                                  max_iterations, deflation_tol);
  return {r.alphas, r.betas, r.eigenvalues, r.niterations, r.criterion, psi};
} catch (Error const &e) {
  XDIAG_RETHROW(e);
}

evolve_lanczos_inplace_result_t
evolve_lanczos_inplace(OpSum const &H, State &psi, double tau, double precision,
                       double shift, bool normalize, int64_t max_iterations,
                       double deflation_tol) try {
  if (!isapprox(H, hc(H))) {
    XDIAG_THROW("Input OpSum is not hermitian. Evolution using the Lanczos "
                "algorithm requires the operator to be hermitian.");
  }

  auto const &block = psi.block();

  // Real time evolution is possible
  if (psi.isreal() && isreal(H)) {
    int iter = 1;
    auto mult = [&iter, &H, &block](arma::vec const &v, arma::vec &w) {
      auto ta = rightnow();
      apply(H, block, v, block, w);
      Log(2, "Lanczos iteration {}", iter);
      timing(ta, rightnow(), "MVM", 1);
      ++iter;
    };
    arma::vec v = psi.vector(0, false);
    auto r = exp_sym_v(mult, v, tau, precision, shift, normalize,
                       max_iterations, deflation_tol);
    return {r.alphas, r.betas, r.eigenvalues, r.niterations, r.criterion};
    // Refer to complex time evolution
  } else {
    return evolve_lanczos_inplace(H, psi, complex(tau), precision, shift,
                                  normalize, max_iterations, deflation_tol);
  }
} catch (Error const &e) {
  XDIAG_RETHROW(e);
}

evolve_lanczos_inplace_result_t
evolve_lanczos_inplace(OpSum const &H, State &psi, complex tau,
                       double precision, double shift, bool normalize,
                       int64_t max_iterations, double deflation_tol) try {

  if (!isapprox(H, hc(H))) {
    XDIAG_THROW("Input OpSum is not hermitian. Evolution using the Lanczos "
                "algorithm requires the operator to be hermitian.");
  }
  if (psi.isreal()) {
    psi.make_complex();
  }
  auto const &block = psi.block();

  int iter = 1;
  auto mult = [&iter, &H, &block](arma::cx_vec const &v, arma::cx_vec &w) {
    auto ta = rightnow();
    apply(H, block, v, block, w);
    Log(2, "Lanczos iteration {}", iter);
    timing(ta, rightnow(), "MVM", 1);
    ++iter;
  };
  arma::cx_vec v = psi.vectorC(0, false);
  auto r = exp_sym_v(mult, v, tau, precision, shift, normalize, max_iterations,
                     deflation_tol);
  return {r.alphas, r.betas, r.eigenvalues, r.niterations, r.criterion};
} catch (Error const &e) {
  XDIAG_RETHROW(e);
}

} // namespace xdiag
