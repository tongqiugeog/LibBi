/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_PROGRAM_LOOP_HPP
#define BI_PROGRAM_LOOP_HPP

#include "Statement.hpp"
#include "Conditioned.hpp"
#include "Braced.hpp"
#include "Expression.hpp"

namespace biprog {
/**
 * Loop.
 *
 * @ingroup program
 */
class Loop: public Statement,
    public Conditioned,
    public Braced,
    public boost::enable_shared_from_this<Loop> {
public:
  /**
   * Constructor.
   */
  Loop(Expression* cond, Expression* braces = NULL);

  /**
   * Destructor.
   */
  virtual ~Loop();
};
}

inline biprog::Loop::Loop(Expression* cond, Expression* braces) :
    Conditioned(cond), Braced(braces) {
  //
}

inline biprog::Loop::~Loop() {
  //
}

#endif
