/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_PROGRAM_LITERAL_HPP
#define BI_PROGRAM_LITERAL_HPP

#include "Expression.hpp"

namespace biprog {
/**
 * Literal.
 *
 * @ingroup program
 */
template<class T1>
class Literal: public virtual Expression {
public:
  /**
   * Constructor.
   */
  Literal(const T1& value);

  /**
   * Destructor.
   */
  virtual ~Literal();

  virtual Literal<T1>* clone();
  virtual Expression* acceptExpression(Visitor& v);

  virtual bool operator<=(const Expression& o) const;
  virtual bool operator==(const Expression& o) const;

  /**
   * Output.
   */
  virtual void output(std::ostream& out) const;

  /**
   * Value.
   */
  T1 value;
};
}

#include "Reference.hpp"
#include "../visitor/Visitor.hpp"

#include <typeinfo>

template<class T1>
inline biprog::Literal<T1>::Literal(const T1& value) :
    value(value) {
  //
}

template<class T1>
inline biprog::Literal<T1>::~Literal() {
  //
}

template<class T1>
biprog::Literal<T1>* biprog::Literal<T1>::clone() {
  return new Literal<T1>(value);
}

template<class T1>
biprog::Expression* biprog::Literal<T1>::acceptExpression(Visitor& v) {
  return v.visitExpression(this);
}

template<class T1>
bool biprog::Literal<T1>::operator<=(const Expression& o) const {
  try {
    const Literal<T1>& o1 = dynamic_cast<const Literal<T1>&>(o);
    return value == o1.value && *type <= *o1.type;
    // ^ use of == is deliberate, expression comparison, not value comparison
  } catch (std::bad_cast e) {
    //
  }
  try {
    const Reference& o1 = dynamic_cast<const Reference&>(o);
    return !*o1.brackets && !*o1.parens && !*o1.braces && *type <= *o1.type;
  } catch (std::bad_cast e) {
    //
  }
  return false;
}

template<class T1>
bool biprog::Literal<T1>::operator==(const Expression& o) const {
  try {
    const Literal<T1>& o1 = dynamic_cast<const Literal<T1>&>(o);
    return value == o1.value;
  } catch (std::bad_cast e) {
    //
  }
  return false;
}

template<class T1>
void biprog::Literal<T1>::output(std::ostream& out) const {
  out << value;
}

#endif