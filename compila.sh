#! /bin/bash

g++ converter_solaris_tracker.cc -o converter_solaris_tracker.exe  -lm  -L$ROOTSYS/lib/root -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -I$ROOTSYS/include/root 
