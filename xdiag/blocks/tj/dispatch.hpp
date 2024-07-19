#pragma once

#include <xdiag/blocks/tj/terms/apply_terms.hpp>
#include <xdiag/common.hpp>

namespace xdiag::tj {

template <typename coeff_t, class fill_f>
inline void dispatch(OpSum const &ops, tJ const &block_in, tJ const &block_out,
                     fill_f &&fill, double zero_precision) try {
  using namespace basis::tj;
  auto const &basis_in = block_in.basis();
  auto const &basis_out = block_out.basis();

  std::visit(
      overload{// uint16_t
               [&](BasisNp<uint16_t> const &idx_in,
                   BasisNp<uint16_t> const &idx_out) {
                 apply_terms<uint16_t, coeff_t, false>(ops, idx_in, idx_out,
                                                       fill, zero_precision);
               },
               [&](BasisSymmetricNp<uint16_t> const &idx_in,
                   BasisSymmetricNp<uint16_t> const &idx_out) {
                 apply_terms<uint16_t, coeff_t, true>(ops, idx_in, idx_out,
                                                      fill, zero_precision);
               },
               // uint32_t
               [&](BasisNp<uint32_t> const &idx_in,
                   BasisNp<uint32_t> const &idx_out) {
                 apply_terms<uint32_t, coeff_t, false>(ops, idx_in, idx_out,
                                                       fill, zero_precision);
               },
               [&](BasisSymmetricNp<uint32_t> const &idx_in,
                   BasisSymmetricNp<uint32_t> const &idx_out) {
                 apply_terms<uint32_t, coeff_t, true>(ops, idx_in, idx_out,
                                                      fill, zero_precision);
               },
               // uint64_t
               [&](BasisNp<uint64_t> const &idx_in,
                   BasisNp<uint64_t> const &idx_out) {
                 apply_terms<uint64_t, coeff_t, false>(ops, idx_in, idx_out,
                                                       fill, zero_precision);
               },
               [&](BasisSymmetricNp<uint64_t> const &idx_in,
                   BasisSymmetricNp<uint64_t> const &idx_out) {
                 apply_terms<uint64_t, coeff_t, true>(ops, idx_in, idx_out,
                                                      fill, zero_precision);
               },
               [&](auto const &idx_in, auto const &idx_out) {
                 XDIAG_THROW("Invalid basis or combination of bases");
                 (void)idx_in;
                 (void)idx_out;
               }},
      basis_in, basis_out);
} catch (Error const &e) {
  XDIAG_RETHROW(e);
}

} // namespace xdiag::tj
