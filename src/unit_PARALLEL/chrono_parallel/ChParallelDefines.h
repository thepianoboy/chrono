// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Hammad Mazhar
// =============================================================================
//
// Description: lots of useful definitions for thrust, includes and enums
// =============================================================================

#ifndef CHPARALLELDEFINES_H
#define CHPARALLELDEFINES_H

#undef _GLIBCXX_ATOMIC_BUILTINS
#undef _GLIBCXX_USE_INT128

#ifdef SIM_ENABLE_GPU_MODE
#define THRUST_DEVICE_SYSTEM THRUST_DEVICE_SYSTEM_CUDA
#else
//#define THRUST_DEVICE_SYSTEM THRUST_DEVICE_SYSTEM_OMP
//#define THRUST_HOST_SYSTEM THRUST_HOST_SYSTEM_OMP
#endif

#ifndef _MSC_VER
#include <fenv.h>
#endif
#include "chrono_parallel/ChApiParallel.h"
#include "chrono_parallel/ChConfigParallel.h"

#ifdef _MSC_VER
#define thrust_parallel thrust::cpp::par
#else
#define thrust_parallel thrust::omp::par
#endif

typedef unsigned int uint;
typedef int shape_type;

#ifdef __CDT_PARSER__
#define BLAZE_SERIAL_SECTION
//#define __host__
//#define __device__
//#define __global__
//#define __constant__
//#define __shared__
//#define CUDA_KERNEL_DIM(...) ()
//#define __KERNEL__(...) ()
//#else
//#define CUDA_KERNEL_DIM(...)  <<< __VA_ARGS__ >>>
//#define __KERNEL__(...)  <<< __VA_ARGS__ >>>
#endif

//#define SIM_ENABLE_GPU_MODE
#ifdef SIM_ENABLE_GPU_MODE
#define custom_vector thrust::device_vector
#else
#ifndef __CDT_PARSER__
#define custom_vector thrust::host_vector
#else
using namespace thrust;
#define custom_vector host_vector
#endif
#endif

// Output Verbosity Level
// Level 0: none
// Level 1: basic
// Level 2: more verbose
#define PRINT_LEVEL 0

// defines to cast thrust vectors as raw pointers
#define CASTC1(x) (char*) thrust::raw_pointer_cast(&x[0])
#define CASTU1(x) (uint*) thrust::raw_pointer_cast(&x[0])
#define CASTU2(x) (uint2*) thrust::raw_pointer_cast(&x[0])
#define CASTU3(x) (uint3*) thrust::raw_pointer_cast(&x[0])
#define CASTI1(x) (int*) thrust::raw_pointer_cast(&x[0])
#define CASTLL(x) (long long*) thrust::raw_pointer_cast(&x[0])
#define CASTS2(x) (short2*) thrust::raw_pointer_cast(&x[0])
#define CASTI2(x) (int2*) thrust::raw_pointer_cast(&x[0])
#define CASTI3(x) (int3*) thrust::raw_pointer_cast(&x[0])
#define CASTI4(x) (int4*) thrust::raw_pointer_cast(&x[0])
#define CASTR1(x) (real*) thrust::raw_pointer_cast(&x[0])
#define CASTR2(x) (real2*) thrust::raw_pointer_cast(&x[0])
#define CASTR3(x) (real3*) thrust::raw_pointer_cast(&x[0])
#define CASTR4(x) (real4*) thrust::raw_pointer_cast(&x[0])
#define CASTB1(x) (bool*) thrust::raw_pointer_cast(&x[0])
#define CASTS(x) (shape_type*) thrust::raw_pointer_cast(&x[0])
#define OBJCAST(x) (object*) thrust::raw_pointer_cast(&x[0])
#define AABBCAST(x) (AABB*) thrust::raw_pointer_cast(&x[0])
#define CONTCAST(x) (contactGPU*) thrust::raw_pointer_cast(&x[0])

#define CHVECCAST(v) ChVector<>(v.x, v.y, v.z)
#define CHQUATCAST(q) ChQuaternion<>(q.w, q.x, q.y, q.z)

#define THREADS 128
#define MAXBLOCK 65535
#define BLOCKS(x) max((int)ceil(x / (real)THREADS), 1)
#define BLOCKS_T(x, y) max((int)ceil(x / (real)y), 1)
#define BLOCKS2D(x) dim3(min(MAXBLOCK, BLOCKS(x)), ceil(BLOCKS(x) / (real)MAXBLOCK), 1)
#define COPY_TO_CONST_MEM(x) cudaMemcpyToSymbolAsync(x##_const, &x, sizeof(x), 0, cudaMemcpyHostToDevice)
#define INDEX1D (blockIdx.x * blockDim.x + threadIdx.x)
#define INDEX3D                                                                      \
  (threadIdx.x + blockDim.x * threadIdx.y + (blockIdx.x * blockDim.x * blockDim.y) + \
   (blockIdx.y * blockDim.x * blockDim.y))
#define INIT_CHECK_THREAD_BOUNDED(x, y) \
  uint index = x;                       \
  if (index >= y) {                     \
    return;                             \
  }

#define START_TIMING(x, y, z) \
  cudaEventCreate(&x);        \
  cudaEventCreate(&y);        \
  cudaEventRecord(x, 0);      \
  z = 0;
#define STOP_TIMING(x, y, z)      \
  cudaThreadSynchronize();        \
  cudaEventRecord(y, 0);          \
  cudaEventSynchronize(y);        \
  cudaEventElapsedTime(&z, x, y); \
  cudaEventDestroy(x);            \
  cudaEventDestroy(y);
#define BIND_TEXF4(x) cudaBindTexture(NULL, x##_tex, CASTF4(x), x.size() * sizeof(real4));
#define BIND_TEXU1(x) cudaBindTexture(NULL, x##_tex, CASTU1(x), x.size() * sizeof(uint1));
#define UNBIND_TEX(x) cudaUnbindTexture(x##_tex);

#define Thrust_Inclusive_Scan_Sum(x, y)                  \
  thrust::inclusive_scan(x.begin(), x.end(), x.begin()); \
  y = x.back();
#define Thrust_Sort_By_Key(x, y) thrust::sort_by_key(x.begin(), x.end(), y.begin())
#define Thrust_Reduce_By_KeyA(x, y, z)                                                                               \
  x = (thrust::reduce_by_key(y.begin(), y.end(), thrust::constant_iterator<uint>(1), y.begin(), z.begin()).second) - \
      z.begin()
#define Thrust_Reduce_By_KeyB(x, y, z, w)                                                                            \
  x = (thrust::reduce_by_key(y.begin(), y.end(), thrust::constant_iterator<uint>(1), z.begin(), w.begin()).second) - \
      w.begin()
#define Thrust_Inclusive_Scan(x) thrust::inclusive_scan(x.begin(), x.end(), x.begin())
#define Thrust_Fill(x, y) thrust::fill(x.begin(), x.end(), y)
#define Thrust_Sort(x) thrust::sort(x.begin(), x.end())
#define Thrust_Count(x, y) thrust::count(x.begin(), x.end(), y)
#define Thrust_Sequence(x) thrust::sequence(x.begin(), x.end())
#define Thrust_Equal(x, y) thrust::equal(x.begin(), x.end(), y.begin())
#define Thrust_Max(x) x[thrust::max_element(x.begin(), x.end()) - x.begin()]
#define Thrust_Min(x) x[thrust::min_element(x.begin(), x.end()) - x.begin()]
#define Thrust_Total(x) thrust::reduce(x.begin(), x.end())
#define DBG(x) printf(x);

enum SOLVERTYPE {
  STEEPEST_DESCENT,
  GRADIENT_DESCENT,
  CONJUGATE_GRADIENT,
  CONJUGATE_GRADIENT_SQUARED,
  BICONJUGATE_GRADIENT,
  BICONJUGATE_GRADIENT_STAB,
  MINIMUM_RESIDUAL,
  QUASAI_MINIMUM_RESIDUAL,
  APGD,
  APGDREF,
  JACOBI,
  GAUSS_SEIDEL,
  PDIP
};

enum SOLVERMODE { NORMAL, SLIDING, SPINNING, BILATERAL };

enum COLLISIONSYSTEMTYPE { COLLSYS_PARALLEL, COLLSYS_BULLET_PARALLEL };

enum NARROWPHASETYPE {
  NARROWPHASE_MPR,
  // NARROWPHASE_GJK, //TODO: implement/test this
  NARROWPHASE_R,
  NARROWPHASE_HYBRID_MPR,
  // NARROWPHASE_HYBRID_GJK //TODO: implement/test this
};

// This is set so that parts of the code that have been "flattened" can know what
// type of system is used.
enum SYSTEMTYPE { SYSTEM_DVI, SYSTEM_DEM };

enum BILATERALTYPE { BODY_BODY, SHAFT_SHAFT, SHAFT_SHAFT_SHAFT, SHAFT_BODY, SHAFT_SHAFT_BODY, UNKNOWN };

// DEM contact force model
enum CONTACTFORCEMODEL { HOOKE, HOOKE_HISTORY, HERTZ, HERTZ_HISTORY };

#endif
