#!/bin/bash

# GL
#export LIBGL_DEBUG='verbose'
#export LIBGL_ALWAYS_INDIRECT=1
#export LIBGL_ALWAYS_SOFTWARE=1
#export LIBGL_NO_DRAWARRAYS=1

# GEANT

#source /exp/sera/dadoun/SL6/Geant4-10.0.0-Linux/share/Geant4-10.0.0/geant4make/geant4make.sh

source /exp/sera/dadoun/SL6/Geant4-9.6.2-Linux/share/Geant4-9.6.2/geant4make/geant4make.sh

#source /opt/lal/share/Geant4-10.1.1/geant4make/geant4make.sh

export G4WORKDIR=`pwd`
echo G4WORKDIR is `pwd`