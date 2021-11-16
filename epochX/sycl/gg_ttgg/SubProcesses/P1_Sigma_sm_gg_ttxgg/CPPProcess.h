//==========================================================================
// This file has been automatically generated for CUDA/C++ standalone by
// MadGraph5_aMC@NLO v. 2.9.5, 2021-08-22
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#ifdef SYCL_LANGUAGE_VERSION
#include <CL/sycl.hpp>
#endif
#include "../../src/HelAmps_sm.h"

#ifndef MG5_Sigma_sm_gg_ttxgg_H
#define MG5_Sigma_sm_gg_ttxgg_H

#include <cassert>
#include <complex>
#include <iostream>
#include <vector>

#include "mgOnGpuConfig.h"
#include "mgOnGpuTypes.h"

#include "Parameters_sm.h"

//--------------------------------------------------------------------------

#ifdef __CUDACC__

#define checkCuda( code ) { assertCuda( code, __FILE__, __LINE__ ); }

inline void assertCuda( cudaError_t code, const char* file, int line, bool abort = true )
{
  if ( code != cudaSuccess )
  {
    printf( "GPUassert: %s %s:%d\n", cudaGetErrorString(code), file, line );
    if ( abort ) assert( code == cudaSuccess );
  }
}

#endif
//--------------------------------------------------------------------------

#ifdef __CUDACC__
namespace gProc
#else
namespace Proc
#endif
{

  //==========================================================================
  // A class for calculating the matrix elements for
  // Process: g g > t t~ g g WEIGHTED<=4 @1
  //--------------------------------------------------------------------------

  class CPPProcess
  {
  public:

    CPPProcess( int numiterations,
                int gpublocks,
                int gputhreads,
                bool verbose = false,
                bool debug = false );

    ~CPPProcess();
    int * get_cHel_ptr();
    fptype * get_cIPC_ptr();
    fptype * get_cIPD_ptr();

    // Initialize process.
    virtual void initProc( std::string param_card_name );

    virtual int code() const { return 1; }

    const std::vector<fptype>& getMasses() const;

    void setInitial( int inid1, int inid2 )
    {
      id1 = inid1;
      id2 = inid2;
    }

    int getDim() const { return dim; }

    int getNIOParticles() const { return nexternal; }

    // Constants for array limits
    static const int ninitial = mgOnGpu::npari;
    static const int nexternal = mgOnGpu::npar;
    //static const int nprocesses = 1;

  private:

    int m_numiterations;
    // gpu variables
    int gpu_nblocks;
    int gpu_nthreads;
    int dim; // gpu_nblocks * gpu_nthreads;

    // print verbose info
    bool m_verbose;

    // print debug info
    bool m_debug;

    static const int nwavefuncs = 6;
    static const int namplitudes = 159;
    static const int ncomb = 64;
    static const int wrows = 63;
    //static const int nioparticles = 6;

    cxtype** amp;

    // Pointer to the model parameters
    Parameters_sm* pars;

    // vector with external particle masses
    std::vector<fptype> mME;

    // Initial particle ids
    int id1, id2;

  };

  //--------------------------------------------------------------------------
#ifdef SYCL_LANGUAGE_VERSION
SYCL_EXTERNAL
void sigmaKin_getGoodHel(const fptype * allmomenta,  // input: momenta as AOSOA[npagM][npar][4][neppM] with nevt=npagM*neppM
bool * isGoodHel,  // output: isGoodHel[ncomb] - device array
sycl::nd_item<3> item_ct1,
int *cHel,
const fptype *cIPC,
const fptype *cIPD
);
#endif

  //--------------------------------------------------------------------------

#ifdef SYCL_LANGUAGE_VERSION
SYCL_EXTERNAL
void sigmaKin_setGoodHel(const bool * isGoodHel, int * cNGoodHel_ptr, int* cGoodHel_ptr);  // input: isGoodHel[ncomb] - host array
#endif

  //--------------------------------------------------------------------------

SYCL_EXTERNAL
void sigmaKin(const fptype * allmomenta,  // input: momenta as AOSOA[npagM][npar][4][neppM] with nevt=npagM*neppM
fptype * allMEs  // output: allMEs[nevt], final |M|^2 averaged over all helicities
, sycl::nd_item<3> item_ct1,
int *cHel,
const fptype *cIPC,
const fptype *cIPD,
int *cNGoodHel,
int *cGoodHel
#ifndef SYCL_LANGUAGE_VERSION
, const int nevt          // input: #events (for cuda: nevt == ndim == gpublocks*gputhreads)
#endif
                 );

  //--------------------------------------------------------------------------
}

#endif // MG5_Sigma_sm_gg_ttxgg_H