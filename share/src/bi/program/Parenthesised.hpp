/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_PROGRAM_PARENTHESISED_HPP
#define BI_PROGRAM_PARENTHESISED_HPP

#include "Expression.hpp"

namespace biprog {
/**
 * Parenthesised expression.
 *
 * @ingroup program
 */
class Parenthesised {
public:
  /**
   * Constructor.
   *
   * @param parens Expression in parentheses.
   */
  Parenthesised(Expression* parens = NULL);

  /**
   * Destructor.
   */
  virtual ~Parenthesised() = 0;

  /**
   * First statement in in brackets.
   */
  boost::shared_ptr<Expression> parens;
};
}

inline biprog::Parenthesised::Parenthesised(Expression* parens) :
    parens(parens) {
  //
}

inline biprog::Parenthesised::~Parenthesised() {
  //
}

#endif

