#ifndef MGONGPUCONFIG_H
#define MGONGPUCONFIG_H 1

// ** NB1 Throughputs (e.g. 6.8E8) are events/sec for "./gcheck.exe -p 65536 128 12"
// ** NB2 Baseline on b7g47n0004 fluctuates (probably depends on load on other VMs)

// Choose if curand is supported for generating random numbers
// For C++, by default, do not inline, but allow this macro to be set from outside with e.g. -DMGONGPU_HAS_NO_CURAND
#ifdef __CUDACC__
#undef MGONGPU_HAS_NO_CURAND
#else
//#undef MGONGPU_HAS_NO_CURAND // default
////#define MGONGPU_HAS_NO_CURAND 1
#endif

// Choose floating point precision
// If one of these macros has been set from outside with e.g. -DMGONGPU_FPTYPE_FLOAT, nothing happens (issue #167)
#if not defined MGONGPU_FPTYPE_DOUBLE and not defined MGONGPU_FPTYPE_FLOAT
// Floating point precision (CHOOSE ONLY ONE)
#define MGONGPU_FPTYPE_DOUBLE 1 // default (~6.8E8)
//#define MGONGPU_FPTYPE_FLOAT 1 // 2.4x faster (~1.64E9 against 6.8E8)
#endif

// Choose whether to inline all HelAmps functions
// This optimization can gain almost a factor 4 in C++, similar to -flto (issue #229)
// By default, do not inline, but allow this macro to be set from outside with e.g. -DMGONGPU_INLINE_HELAMPS
//#undef MGONGPU_INLINE_HELAMPS // default
////#define MGONGPU_INLINE_HELAMPS 1

// Choose whether to hardcode the cIPC/cIPD physics parameters rather than reading them from user cards
// This optimization can gain 20% in CUDA in eemumu (issue #39)
// By default, do not hardcode, but allow this macro to be set from outside with e.g. -DMGONGPU_HARDCODE_CIPC
//#undef MGONGPU_HARDCODE_CIPC // default
////#define MGONGPU_HARDCODE_CIPC 1

// Complex type in c++: std::complex or cxsmpl (CHOOSE ONLY ONE)
#ifndef __CUDACC__
//#define MGONGPU_CPPCXTYPE_STDCOMPLEX 1 // ~8 percent slower on float, same on double (5.1E6/double, 9.4E6/float)
#define MGONGPU_CPPCXTYPE_CXSMPL 1 // new default (5.1E6/double, 10.2E6/float)
#endif

// Complex type in cuda: thrust or cucomplex or cxsmpl (CHOOSE ONLY ONE)
#ifdef __CUDACC__
#define MGONGPU_CUCXTYPE_THRUST 1 // default (~1.15E9/double, ~3.2E9/float)
//#define MGONGPU_CUCXTYPE_CUCOMPLEX 1 // ~10 percent slower (1.03E9/double, ~2.8E9/float)
//#define MGONGPU_CUCXTYPE_CXSMPL 1 // ~10 percent slower (1.00E9/double, ~2.9E9/float)
#endif

// Cuda nsight compute (ncu) debug: add dummy lines to ease SASS program flow navigation
#ifdef __CUDACC__
#undef MGONGPU_NSIGHT_DEBUG // default
//#define MGONGPU_NSIGHT_DEBUG 1
#endif

// SANITY CHECKS (floating point precision)
#if defined MGONGPU_FPTYPE_DOUBLE and defined MGONGPU_FPTYPE_FLOAT
#error You must CHOOSE (ONE AND) ONLY ONE of MGONGPU_FPTYPE_DOUBLE or defined MGONGPU_FPTYPE_FLOAT
#endif

// SANITY CHECKS (c++ complex number implementation)
#ifndef __CUDACC__
#if defined MGONGPU_CPPCXTYPE_STDCOMPLEX and defined MGONGPU_CPPCXTYPE_CXSMPL
#error You must CHOOSE (ONE AND) ONLY ONE of MGONGPU_CPPCXTYPE_STDCOMPLEX or MGONGPU_CPPCXTYPE_CXSMPL
#endif
#endif

// SANITY CHECKS (cuda complex number implementation)
#ifdef __CUDACC__
#if defined MGONGPU_CUCXTYPE_THRUST and defined MGONGPU_CUCXTYPE_CUCOMPLEX and defined MGONGPU_CUCXTYPE_CXSMPL
#error You must CHOOSE (ONE AND) ONLY ONE of MGONGPU_CUCXTYPE_THRUST or MGONGPU_CUCXTYPE_CUCOMPLEX or MGONGPU_CUCXTYPE_CXSMPL
#endif
#endif

namespace mgOnGpu
{

  // --- Type definitions

  // Floating point type: fptype
#if defined MGONGPU_FPTYPE_DOUBLE
  typedef double fptype; // double precision (8 bytes, fp64)
#elif defined MGONGPU_FPTYPE_FLOAT
  typedef float fptype; // single precision (4 bytes, fp32)
#endif

  // --- Physics process-specific constants that are best declared at compile time

  const int np4 = 4; // dimensions of 4-momenta (E,px,py,pz)

  const int npari = 2; // #particles in the initial state (incoming): e.g. 2 (e+ e-) for e+ e- -> mu+ mu-
  const int nparf = 5; // #particles in the final state (outgoing): e.g. 2 (mu+ mu-) for e+ e- -> mu+ mu-
  const int npar = npari + nparf; // #particles in total (external = initial + final): e.g. 4 for e+ e- -> mu+ mu-

  const int ncomb = 128; // #helicity combinations: e.g. 16 for e+ e- -> mu+ mu- (2**4 = fermion spin up/down ** npar)

  const int nw6 = 6; // dimensions of each wavefunction (HELAS KEK 91-11): e.g. 6 for e+ e- -> mu+ mu- (fermions and vectors)
  const int nwf = 121; // #wavefunctions = #external (npar) + #internal: e.g. 5 for e+ e- -> mu+ mu- (1 internal is gamma or Z)

  // --- Platform-specific software implementation details

  // Maximum number of blocks per grid
  // ** NB Some arrays of pointers will be allocated statically to fit all these blocks
  // ** (the actual memory for each block will then be allocated dynamically only for existing blocks)
  //const int nbpgMAX = 2048;

  // Maximum number of threads per block
  //const int ntpbMAX = 256; // AV Apr2021: why had I set this to 256?
  const int ntpbMAX = 1024; // NB: 512 is ok, but 1024 does fail with "too many resources requested for launch"

  // Alignment requirement for using reinterpret_cast with SIMD vectorized code
  // (using reinterpret_cast with non aligned memory may lead to segmentation faults!)
  // [NB eventually define this also for code built with nvcc (#318), however this requires c++17 and nvcc >= 11.2]
#ifndef __CUDACC__
  constexpr int cppAlign = 64; // alignment requirement for SIMD vectorization (64-byte i.e. 512-bit)
#endif

  // C++ SIMD vectorization width (this will be used to set neppV)
#ifdef __CUDACC__
#undef MGONGPU_CPPSIMD
#else
#if defined __AVX512VL__
#ifdef MGONGPU_PVW512
  // "512z" AVX512 with 512 width (512-bit ie 64-byte): 8 (DOUBLE) or 16 (FLOAT)
#ifdef MGONGPU_FPTYPE_DOUBLE
#define MGONGPU_CPPSIMD 8
#else
#define MGONGPU_CPPSIMD 16
#endif
#else
  // "512y" AVX512 with 256 width (256-bit ie 32-byte): 4 (DOUBLE) or 8 (FLOAT) [gcc DEFAULT]
#ifdef MGONGPU_FPTYPE_DOUBLE
#define MGONGPU_CPPSIMD 4
#else
#define MGONGPU_CPPSIMD 8
#endif
#endif
#elif defined __AVX2__
  // "avx2" AVX2 (256-bit ie 32-byte): 4 (DOUBLE) or 8 (FLOAT) [clang DEFAULT]
#ifdef MGONGPU_FPTYPE_DOUBLE
#define MGONGPU_CPPSIMD 4
#else
#define MGONGPU_CPPSIMD 8
#endif
#elif defined __SSE4_2__
  // "sse4" SSE4.2 (128-bit ie 16-byte): 2 (DOUBLE) or 4 (FLOAT)
#ifdef MGONGPU_FPTYPE_DOUBLE
#define MGONGPU_CPPSIMD 2
#else
#define MGONGPU_CPPSIMD 4
#endif
#else
  // "none" i.e. no SIMD (*** NB: this is equivalent to AOS ***)
#undef MGONGPU_CPPSIMD
#endif
#endif
}

// Expose typedefs and operators outside the namespace
using mgOnGpu::fptype;

// Cuda nsight compute (ncu) debug: add dummy lines to ease SASS program flow navigation
// Arguments (not used so far): text is __FUNCTION__, code is 0 (start) or 1 (end)
#if defined __CUDACC__ && defined MGONGPU_NSIGHT_DEBUG
#define mgDebugDeclare()                              \
  __shared__ float mgDebugCounter[mgOnGpu::ntpbMAX];
#define mgDebugInitialise()                     \
  { mgDebugCounter[threadIdx.x]=0; }
#define mgDebug( code, text )                   \
  { mgDebugCounter[threadIdx.x] += 1; }
#define mgDebugFinalise()                                               \
  { if ( blockIdx.x == 0 && threadIdx.x == 0 ) printf( "MGDEBUG: counter=%f\n", mgDebugCounter[threadIdx.x] ); }
#else
#define mgDebugDeclare()                        \
  /*noop*/
#define mgDebugInitialise()                     \
  { /*noop*/ }
#define mgDebug( code, text )                   \
  { /*noop*/ }
#define mgDebugFinalise()                       \
  { /*noop*/ }
#endif

// Define empty CUDA declaration specifiers for C++
#ifndef __CUDACC__
#define __global__
#define __host__
#define __device__
#endif

// For SANITY CHECKS: check that neppR, neppM, neppV... are powers of two (https://stackoverflow.com/a/108360)
inline constexpr bool ispoweroftwo( int n ){ return ( n > 0 ) && !( n & ( n - 1 ) ); }

#endif // MGONGPUCONFIG_H