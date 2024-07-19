#pragma once

#include <xdiag/common.hpp>
#include <xdiag/combinatorics/combinations.hpp>
#include <xdiag/combinatorics/combinations_index.hpp>
#include <xdiag/combinatorics/lin_table.hpp>

namespace xdiag::basis::spinhalf {
template <typename bit_tt> class BasisSz {
public:
  using bit_t = bit_tt;
  using iterator_t = combinatorics::CombinationsIterator<bit_t>;

  BasisSz() = default;
  BasisSz(int64_t n_sites, int64_t nup);

  iterator_t begin() const;
  iterator_t end() const;
  int64_t dim() const;
  int64_t size() const;

  inline int64_t index(bit_t spins) const { return lintable_.index(spins); }
  inline bit_t state(int64_t index) const { return states_[index]; }
  
  int64_t n_sites() const;
  int64_t n_up() const;
  static constexpr bool sz_conserved() { return true; }

  bool operator==(BasisSz const &rhs) const;
  bool operator!=(BasisSz const &rhs) const;

private:
  int64_t n_sites_;
  int64_t n_up_;
  combinatorics::LinTable<bit_t> lintable_;
  std::vector<bit_t> states_;
  int64_t size_;
  iterator_t begin_, end_;
};

} // namespace xdiag::basis::spinhalf
