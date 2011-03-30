/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_UPDATER_RUPDATER_HPP
#define BI_UPDATER_RUPDATER_HPP

#include "../state/State.hpp"
#include "../random/Random.hpp"

namespace bi {
/**
 * @internal
 *
 * Updater for r-net.
 *
 * @ingroup method_updater
 *
 * @tparam B Model type.
 */
template<class B>
class RUpdater {
public:
  /**
   * Constructor.
   *
   * @param rng Random number generator.
   */
  RUpdater(Random& rng);

  /**
   * Update r-net.
   *
   * @param s State to update.
   */
  template<Location L>
  void update(State<L>& s);

  /**
   * Don't make any changes on next update.
   *
   * @todo Hack for now for methods that adjust random variates themselves.
   */
  void skipNext();

private:
  /**
   * Random number generator.
   */
  Random& rng;

  /**
   * Skip next update?
   */
  bool skip;
};
}

#include "RUpdateVisitor.hpp"
#include "../traits/random_traits.hpp"

template<class B>
bi::RUpdater<B>::RUpdater(Random& rng) : rng(rng), skip(false) {
  //
}

template<class B>
template<bi::Location L>
void bi::RUpdater<B>::update(State<L>& s) {
  typedef typename B::RTypeList S;
  typedef BOOST_TYPEOF(s.get(R_NODE)) M1;
  typedef RUpdateVisitor<B,S,M1> Visitor;

  if (!skip) {
    BOOST_AUTO(X, s.get(R_NODE));
    if (X.lead() == X.size1()) {
      if (all_gaussian_variates<S>::value) {
        rng.gaussians(matrix_as_vector(X));
      } else if (all_uniform_variates<S>::value) {
        rng.uniforms(matrix_as_vector(X));
      } else {
        Visitor::accept(rng, s.get(R_NODE));
      }
    } else {
      Visitor::accept(rng, s.get(R_NODE));
    }
  }
  skip = false;
}

template<class B>
inline void bi::RUpdater<B>::skipNext() {
  skip = true;
}

#endif