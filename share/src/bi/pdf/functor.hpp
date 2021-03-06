/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 */
#ifndef BI_PDF_FUNCTOR_HPP
#define BI_PDF_FUNCTOR_HPP

#include "../math/scalar.hpp"
#include "../math/function.hpp"
#include "../math/misc.hpp"

namespace bi {

/**
 * @ingroup math_pdf
 *
 * Gamma log-density functor.
 */
template<class T>
struct gamma_log_density_functor: public std::unary_function<T,T> {
  const T alpha, beta, logZ;

  CUDA_FUNC_HOST
  gamma_log_density_functor(const T alpha, const T beta) :
      alpha(alpha), beta(beta), logZ(
          bi::lgamma(alpha) + alpha * bi::log(beta)) {
    //
  }

  CUDA_FUNC_BOTH
  T operator()(const T& x) const {
    return (alpha - static_cast<T>(1.0)) * bi::log(x) - x / beta - logZ;
  }
};

/**
 * @ingroup math_pdf
 *
 * Gamma density functor.
 */
template<class T>
struct gamma_density_functor: private gamma_log_density_functor<T> {
  CUDA_FUNC_HOST
  gamma_density_functor(const T alpha, const T beta) :
      gamma_log_density_functor<T>(alpha, beta) {
    //
  }

  CUDA_FUNC_BOTH
  T operator()(const T& x) const {
    return bi::exp(gamma_log_density_functor<T>::operator()(x));
  }
};

/**
 * @ingroup math_pdf
 *
 * Inverse gamma log-density functor.
 */
template<class T>
struct inverse_gamma_log_density_functor: private gamma_log_density_functor<T> {
  CUDA_FUNC_HOST
  inverse_gamma_log_density_functor(const T alpha, const T beta) :
      gamma_log_density_functor<T>(alpha, static_cast<T>(1.0) / beta) {
    //
  }

  CUDA_FUNC_BOTH
  T operator()(const T& x) const {
    return gamma_log_density_functor<T>::operator()(static_cast<T>(1.0) / x)
        - static_cast<T>(2.0) * bi::log(x);
  }
};

/**
 * @ingroup math_pdf
 *
 * Inverse gamma density functor.
 */
template<class T>
struct inverse_gamma_density_functor: private gamma_log_density_functor<T> {
  CUDA_FUNC_HOST
  inverse_gamma_density_functor(const T alpha, const T beta) :
      gamma_log_density_functor<T>(alpha, static_cast<T>(1.0) / beta) {
    //
  }

  CUDA_FUNC_BOTH
  T operator()(const T& x) const {
    return bi::exp(
        gamma_log_density_functor<T>::operator()(static_cast<T>(1.0) / x)
            - static_cast<T>(2.0) * bi::log(x));
  }
};

/**
 * Specialised functor for Gaussian density evaluations.
 *
 * @tparam T1 Scalar type.
 */
template<class T1>
struct gaussian_density_functor {
  /**
   * Constructor.
   *
   * @param logZ Log of normalising constant.
   */
  gaussian_density_functor(const T1 logZ) :
      logZ(logZ) {
    //
  }

  /**
   * Apply.
   */
  T1 operator()(const T1 p) {
    const T1 a = -0.5;

    return bi::exp(a * p - logZ);
  }

  /**
   * Log of normalising constant.
   */
  const T1 logZ;
};

/**
 * Specialised functor for Gaussian density evaluations.
 *
 * @tparam T1 Scalar type.
 */
template<class T1>
struct gaussian_density_update_functor {
  /**
   * Constructor.
   *
   * @param logZ Log of normalising constant.
   */
  gaussian_density_update_functor(const T1 logZ) :
      logZ(logZ) {
    //
  }

  /**
   * Apply.
   */
  T1 operator()(const T1 p1, const T1 p2) {
    const T1 a = -0.5;

    return p1 * bi::exp(a * p2 - logZ);
  }

  /**
   * Log of normalising constant.
   */
  const T1 logZ;
};

/**
 * Specialised functor for Gaussian log-density evaluations.
 *
 * @tparam T1 Scalar type.
 */
template<class T1>
struct gaussian_log_density_functor {
  /**
   * Constructor.
   *
   * @param logZ Log of normalising constant.
   */
  gaussian_log_density_functor(const T1 logZ) :
      logZ(logZ) {
    //
  }

  /**
   * Apply.
   */
  T1 operator()(const T1 p) {
    const T1 a = -0.5;

    return a * p - logZ;
  }

  /**
   * Log of normalising constant.
   */
  const T1 logZ;
};

/**
 * Specialised functor for Gaussian log-density evaluations.
 *
 * @tparam T1 Scalar type.
 */
template<class T1>
struct gaussian_log_density_update_functor {
  /**
   * Constructor.
   *
   * @param logZ Log of normalising constant.
   */
  gaussian_log_density_update_functor(const T1 logZ) :
      logZ(logZ) {
    //
  }

  /**
   * Apply.
   */
  T1 operator()(const T1 p1, const T1 p2) {
    const T1 a = -0.5;

    return p1 + a * p2 - logZ;
  }

  /**
   * Log of normalising constant.
   */
  const T1 logZ;
};

/**
 * @ingroup math_pdf
 *
 * Poisson log-density functor.
 */
template<class T>
struct poisson_log_density_functor: public std::unary_function<T,T> {
  const T lambda;

  CUDA_FUNC_HOST
  poisson_log_density_functor(const T lambda) :
    lambda(lambda) {
    //
  }

  CUDA_FUNC_BOTH
  T operator()(const T& x) const {
    if (lambda == 0) return (x == 0 ? 0 : -BI_INF);
    else return x * bi::log(lambda) - lambda - bi::lgamma(x + 1);
  }
};

/**
 * @ingroup math_pdf
 *
 * Poisson density functor.
 */
template<class T>
struct poisson_density_functor: private poisson_log_density_functor<T> {
  CUDA_FUNC_HOST
  poisson_density_functor(const T lambda) :
    poisson_log_density_functor<T>(lambda) {
    //
  }

  CUDA_FUNC_BOTH
  T operator()(const T& x) const {
    return bi::exp(poisson_log_density_functor<T>::operator()(x));
  }
};

/**
 * @ingroup math_pdf
 *
 * Negative binomial log-density functor.
 */
template<class T>
struct negbin_log_density_functor: public std::unary_function<T,T> {
  const T mu, k;

  CUDA_FUNC_HOST
  negbin_log_density_functor(const T mu, const T k) :
    mu(mu), k(k) {
    //
    }

  CUDA_FUNC_BOTH
  T operator()(const T& x) const {
    if (x == 0 && k == 0)
      return 0;
    if (x == 0) {
      if (k == 0) return(0);
      return (k * (k < mu ? bi::log(k / (k + mu)) : bi::log1p(-mu / (k + mu))));
    }
    if (x < 1e-10 * k) {
      T p = (k < mu ? bi::log(k / (1 + k/mu)) : bi::log(mu / (1 + mu/k)));
      return (x * p - mu - bi::lgamma(x + 1) + bi::log1p(x * (x - 1) / (2 * k)));
    } else {
      T ans = bi::lgamma(k + x + 1) - bi::lgamma(k + 1) - bi::lgamma(x + 1);
      if (k > 0) ans += k * bi::log(k / (k + mu));
      if (x > 0) ans += x * bi::log(mu / (k + mu));
      return ans + log(k / (k + x));
    }
  }
};

/**
 * @ingroup math_pdf
 *
 * Negative binomial density functor.
 */
template<class T>
struct negbin_density_functor: private negbin_log_density_functor<T> {
  CUDA_FUNC_HOST
  negbin_density_functor(const T mu, const T k) :
    negbin_log_density_functor<T>(mu, k) {
    //
  }

  CUDA_FUNC_BOTH
  T operator()(const T& x) const {
    return bi::exp(negbin_log_density_functor<T>::operator()(x));
  }
};

/**
 * @ingroup math_pdf
 *
 * Binomial log-density functor.
 */
template<class T1, class T2>
struct binomial_log_density_functor: public std::unary_function<T1,T2> {
  const T1 n;
  const T2 lN, logP, log1P;

  CUDA_FUNC_HOST
  binomial_log_density_functor(const T1 n, const T2 p) :
    n(n), lN(bi::lgamma(n + 1)), logP(bi::log(p)), log1P(bi::log1p(-p)) {
    //
    }

  CUDA_FUNC_BOTH
  T2 operator()(const T1& x) const {
    T2 log_density = lN - bi::lgamma(x + 1) - bi::lgamma(n - x + 1);
    if (x > 0) log_density += x * logP;
    if (x < n) log_density += (n - x) * log1P;
    return log_density;
  }
};

/**
 * @ingroup math_pdf
 *
 * Binomial density functor.
 */
template<class T1, class T2>
struct binomial_density_functor: private binomial_log_density_functor<T1, T2> {
  CUDA_FUNC_HOST
  binomial_density_functor(const T1 n, const T2 p) :
    binomial_log_density_functor<T1, T2>(n, p) {
    //
  }

  CUDA_FUNC_BOTH
  T2 operator()(const T1& x) const {
    return bi::exp(binomial_log_density_functor<T1, T2>::operator()(x));
  }
};

}

#endif
