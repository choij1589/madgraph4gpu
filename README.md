# Madgraph 4 GPU

This repository contains code developed in the context of porting the [MadGraph5_aMC@NLO](https://cp3.irmp.ucl.ac.be/projects/madgraph/) event generator software onto GPU platforms and vector instructions on CPUs. MadGraph5_aMC@NLO is able to generate code for various physics processes in different programming languages (Fortran, C, C++). The code generated in this repository in "epochX" of the MadGraph5_aMC@NLO generator allows to also produce source code for those physics processes to run on GPU and CPU platforms. 

## To reproduce FE_INVALID / FE_DIVBYZERO errors
See this [commit](https://github.com/madgraph5/madgraph4gpu/compare/master...choij1589:madgraph4gpu:dev_cms_integration) for the changes.
- Envrionment setup done by setup.sh scripts
- proc_card.dat and run_card.dat are attached in CMSCards directory

## Notes
1. After setting ptl = 10 / ptj = 10 in run_card.dat, it still prints FE_INVALID but the frequency reduces
2. export RNDGEN=hassNoCurand gives the same error for CPP 

