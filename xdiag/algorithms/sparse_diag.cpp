#include "sparse_diag.h"

#include <xdiag/algorithms/lanczos/eigs_lanczos.h>
#include <xdiag/algorithms/lanczos/eigvals_lanczos.h>
#include <xdiag/utils/logger.h>

namespace xdiag {

double eigval0(BondList const &bonds, block_variant_t const &block,
               double precision, int64_t max_iterations, bool force_complex,
               int64_t random_seed) try {
  if (dim(block) == 0) {
    Log.warn("Warning: block zero dimensional in eigval0");
    return std::nan("");
  }

  auto res = eigvals_lanczos(bonds, block, 1, precision, max_iterations,
                             force_complex, 1e-7, random_seed);
  
  if (res.eigenvalues.size() == 0) {
    Log.warn("Warning: Tmatrix zero dimensional in eig0_cplx");
    return std::nan("");
  } else {
    return res.eigenvalues(0);
  }
} catch (...) {
  XDiagRethrow("Unable to compute ground state energy");
  return std::nan("");
}

std::tuple<double, State> eig0(BondList const &bonds,
                               block_variant_t const &block, double precision,
                               int64_t max_iterations, bool force_complex,
                               int64_t random_seed) try {
  if (dim(block) == 0) {
    Log.warn("Warning: block zero dimensional in eigval0");
    return {std::nan(""), State()};
  }
  auto res = eigs_lanczos(bonds, block, 1, precision, max_iterations,
                          force_complex, 1e-7, random_seed);

  if (res.eigenvalues.size() == 0) {
    Log.warn("Warning: Tmatrix zero dimensional in eig0_cplx");
    return {std::nan(""), State()};
  } else {
    return {res.eigenvalues(0), res.eigenvectors};
  }
} catch (...) {
  XDiagRethrow("Unable to compute ground state");
  return {std::nan(""), State()};
}

} // namespace xdiag
