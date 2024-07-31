#!/bin/bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=el8_amd64_gcc12
#export MADGRAPH_CUDA_ARCHITECTURE=60    # 60 for normal GPUs, 80 for A100/H100
cd /cvmfs/cms.cern.ch/el8_amd64_gcc12/cms/cmssw/CMSSW_14_0_9
eval `scram runtime -sh`
cd -
nvcc -V
