/**
 * @file
 *
 * @author Lawrence Murray <lawrence.murray@csiro.au>
 * $Rev$
 * $Date$
 */
#ifndef BI_CUDA_UPDATER_STATICUPDATERKERNEL_CUH
#define BI_CUDA_UPDATER_STATICUPDATERKERNEL_CUH

#include "../cuda.hpp"

namespace bi {
/**
 * Kernel function for static update.
 *
 * @tparam B Model type.
 * @tparam S Action type list.
 */
template<class B, class S>
CUDA_FUNC_GLOBAL void kernelStaticUpdater(State<B,ON_DEVICE> s);

}

#include "StaticUpdaterVisitorGPU.cuh"
#include "../constant.cuh"
#include "../global.cuh"
#include "../../state/Pa.hpp"

template<class B, class S>
CUDA_FUNC_GLOBAL void bi::kernelStaticUpdater(State<B,ON_DEVICE> s) {
  typedef Pa<ON_DEVICE,B,constant,constant,global,global> PX;
  typedef Ou<ON_DEVICE,B,global> OX;
  typedef StaticUpdaterVisitorGPU<B,S,PX,OX> Visitor;

  const int p = blockIdx.x*blockDim.x + threadIdx.x;
  const int id = blockIdx.y*blockDim.y + threadIdx.y;
  PX pax;
  OX x;

  /* update */
  if (p < s.size()) {
    Visitor::accept(s, p, id, pax, x);
  }
}

#endif
