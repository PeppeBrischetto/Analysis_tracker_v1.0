***********************************************************************************
*                 README file for all the directory content                       *
***********************************************************************************
*                                                                                 *
*  This file allows to understand the main work of all the macros inside the      *
*  directory "Analysis_BR".                                                       *
*                                                                                 *
*.................................................................................*
*                                                                                 *
*  1. C_multDistrib.C                                                             *
*  This macro allows to plot the multiplicity distribution for each row of the    *
*  gas tracker prototype (0,1,2,3,4) starting from the reconstructed track of the *
*  reference run. Please, be sure of the following needs:                         *
*                                                                                 *
*     a. the run you want to analyse should exists in the dir. "Raw_Data_Br". If  *
*        not, copy it from "irrad/Analysis_tracker_v1.0/Raw_Data" to the same dir *
*        (Raw_Data_Br), execute ./dataprocessing.sh "idRun";                      *
*     b. before launch C_multDistrib.C, verify all the points of the TCutG for    *
*        the PID performed (i.e., go in the dir. "Tracks_Br", open the root file  *
*        of the tracks for the reference run, execute:                            *
*                       "Data_R->Draw(cl_x_mm[1]:cl_x_mm[0])"                     *
*        and, if not, adjust the points of the TCutG;                             *
*                                                                                 *
*  At the end, C_multDistrib.C will returns two picture about the multiplicity    *
*  distributions for each row (.png and .eps to preserve the resolution in zoom). *
*                                                                                 *
***********************************************************************************
