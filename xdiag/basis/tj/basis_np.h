#pragma once

#include <xdiag/combinatorics/combinations.h>
#include <xdiag/combinatorics/lin_table.h>
#include <xdiag/common.h>

namespace xdiag::basis::tj {

template <typename bit_t> class BasisNp {
public:
  using bit_type = bit_t;

  BasisNp() = default;
  BasisNp(int64_t n_sites, int64_t n_up, int64_t n_dn);

  int64_t dim() const;
  int64_t size() const;

  inline int64_t index(bit_t ups, bit_t dns) const {
    bit_t dncs = bits::extract<bit_t>(dns, (~ups) & sitesmask_);
    return index_ups(ups) * size_dncs_ + index_dncs(dncs);
  }

  int64_t n_sites() const;
  int64_t n_up() const;
  int64_t n_dn() const;
  static constexpr bool np_conserved() { return true; }

private:
  int64_t n_sites_;
  int64_t n_up_;
  int64_t n_dn_;

  int64_t size_ups_;
  int64_t size_dncs_;
  int64_t size_;
  bit_t sitesmask_;

  combinatorics::LinTable<bit_t> lintable_ups_;
  combinatorics::LinTable<bit_t> lintable_dncs_;

public:
  int64_t size_ups() const;
  int64_t size_dncs() const;
  int64_t ups_offset(int64_t idx_ups) const;

  combinatorics::Combinations<bit_t> states_ups() const;
  combinatorics::Combinations<bit_t> states_dncs(bit_t ups) const;
  combinatorics::CombinationsIndex<bit_t> states_indices_ups() const;
  combinatorics::CombinationsIndex<bit_t> states_indices_dncs(bit_t ups) const;

#ifdef _OPENMP
  combinatorics::CombinationsThread<bit_t> states_ups_thread() const;
  combinatorics::CombinationsThread<bit_t> states_dncs_thread(bit_t ups) const;
  combinatorics::CombinationsIndexThread<bit_t>
  states_indices_ups_thread() const;
  combinatorics::CombinationsIndexThread<bit_t>
  states_indices_dncs_thread(bit_t ups) const;
#endif

  inline int64_t index_ups(bit_t ups) const { return lintable_ups_.index(ups); }
  inline int64_t index_dncs(bit_t dncs) const {
    return lintable_dncs_.index(dncs);
  }
};

} // namespace xdiag::basis::tj
