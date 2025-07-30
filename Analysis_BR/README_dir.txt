***********************************************************************************
*                 README file for all the directory content                       *
***********************************************************************************
*                                                                                 *
*  This file allows to understand the main work of all the macros inside the      *
*  directory "Analysis_BR".                                                       *
*                                                                                 *
*.................................................................................*
*                                                                                 *
*  1. C_clCharge.C                                                                *
*  This macro allows to plot the cluster charge distributions for each row of the *
*  gas tracker prototype (0,1,2,3,4) starting from the reconstructed track of the *
*  reference run. For the right handling of the macro, be sure the runID has been *
*  correctly modified in all the rows where it appears. Please, be sure of the    *
*  following needs:                                                               *
*                                                                                 *
*     a. the run you want to analyse should exists in the dir. "Raw_Data_Br". If  *
*        not, copy it from "irrad/Analysis_tracker_v1.0/Raw_Data" to the same dir *
*        (Raw_Data_Br), execute ./dataprocessing.sh "idRun";                      *
*     b. before launch C_multDistrib.C, verify all the points of the TCutG for    *
*        the PID performed (i.e., go in the dir. "Tracks_Br", open the root file  *
*        of the tracks for the reference run, execute:                            *
*                       "Data_R->Draw(cl_x_mm[1]:cl_x_mm[0]"                      *
*        and, if not, adjust the points of the TCutG);                            *
*                                                                                 *
*  At the end, C_clCharge.C will return two picture about the cluster charge      *
*  distributions for each row (.png and .eps, the latter to preserve the          *
*  resolution during zoomIN/OUT).                                                 *
*                                                                                 *
*.................................................................................*
*                                                                                 *
*  2. C_multDistrib.C                                                             *
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
*                       "Data_R->Draw(cl_x_mm[1]:cl_x_mm[0]"                      *
*        and, if not, adjust the points of the TCutG);                            *
*                                                                                 *
*  At the end, C_multDistrib.C will return two picture about the multiplicity     *
*  distributions for each row (.png and .eps, the latter to preserve the          *
*  resolution during zoomIN/OUT).                                                 *
*                                                                                 *
*.................................................................................*
*                                                                                 *
*  3. C_multDistrib_v1.C                                                          *
*  This macro allows to plot the multiplicity distribution for each row of the    *
*  gas tracker prototype (0,1,2,3,4) as well as the theta distribution (all evts  *
*  and evts with E_SiC>2000 a.u.), starting from the reconstructed track of the   *
*  reference run. Please, be sure of the following needs:                         *
*                                                                                 *
*     a. the run you want to analyse should exists in the dir. "Raw_Data_Br". If  *
*        not, copy it from "irrad/Analysis_tracker_v1.0/Raw_Data" to the same dir *
*        (Raw_Data_Br), execute ./dataprocessing.sh "idRun";                      *
*     b. before launch C_multDistrib.C, verify all the points of the TCutG for    *
*        the PID performed (i.e., go in the dir. "Tracks_Br", open the root file  *
*        of the tracks for the reference run, execute:                            *
*                       "Data_R->Draw(cl_x_mm[1]:cl_x_mm[0]"                      *
*        and, if not, adjust the points of the TCutG);                            *
*                                                                                 *
*  At the end, C_multDistrib.C will return two picture about the multiplicity     *
*  distributions for each row (.png and .eps, the latter to preserve the          *
*  resolution during zoomIN/OUT).                                                 *
*                                                                                 *
*.................................................................................*
*                                                                                 *
*  4. C_multDistrib_v2.C                                                          *
*  This macro allows to plot the multiplicity distribution for each row of the    *
*  gas tracker prototype (0,1,2,3,4) as well as the theta distribution, using the *
*  condition on the SiC E_SiC>2000 a.u.. A 6-pads TCanvas will be opened, eachone *
*  containing two TH1D (all evts and constrained evts) for the multiplicity distr.*
*  starting from the reconstructed track of the run under study. Please, be sure  *
*  of the following needs:                                                        *
*                                                                                 *
*     a. the run you want to analyse should exists in the dir. "Raw_Data_Br". If  *
*        not, copy it from "irrad/Analysis_tracker_v1.0/Raw_Data" to the same dir *
*        (Raw_Data_Br), execute ./dataprocessing.sh "idRun";                      *
*     b. before launch C_multDistrib.C, verify all the points of the TCutG for    *
*        the PID performed (i.e., go in the dir. "Tracks_Br", open the root file  *
*        of the tracks for the reference run, execute:                            *
*                       "Data_R->Draw(cl_x_mm[1]:cl_x_mm[0]"                      *
*        and, if not, adjust the points of the TCutG);                            *
*                                                                                 *
*  At the end, C_multDistrib.C will return two picture about the multiplicity     *
*  distributions for each row (.png and .eps, the latter to preserve the          *
*  resolution during zoomIN/OUT).                                                 *
*                                                                                 *
*.................................................................................*
*                                                                                 *
*  5. C_plot_charge_mult.C                                                        *
*  This macro allows th plot the charge as a funcion of the pad multiplicity. In  *
*  particular, starting from the reconstructed tracks during the offline analysis,*
*  the events are collected in groups of pad-multiplicity from 0 to 30 for each   *
*  the pad-segemnted row 0,1,2,3,4: the outputs are the charge-distribution and   *
*  the average behabiour of the charge as a function of the pad-multiplicity and  *
*  for each row. It takes as input the runID.                                     *
*                                                                                 *
*.................................................................................*
*                                                                                 *
*  6. C_plot_mult_theta.C                                                         *
*  This macro allows th plot the horizontal angle theta as a funcion of the pad   *
*  multiplicity. In particular, starting from the reconstructed tracks during the *
*  offline analysis, the events are collected in groups of pad-multiplicity from  *
*  0 to 30 for each the pad-segemnted row 0,1,2,3,4: theta-distribution and the   *
*  average behabiour of the charge as a function of the pad-multiplicity (and for *
*  each row) are the main outputs. It takes as input the runID.                   *
*                                                                                 *
*.................................................................................*
*                                                                                 *
*  7. C_plotTheta.C                                                               *
*  This macro allows to plot theta distributions for the reference run, starting  *
*  from the reconstructed tracks during the offline analysis. It takes as input   *
*  the runID.                                                                     *
*                                                                                 *
*.................................................................................* 
*                                                                                 *
*  8. C_plotTheta_padExclusion.C                                                  *
*  This macro allows to plot theta distributions for the reference run excluding  *
*  a couple of pads, i.e. the initial ones on row 0 and the last ones on row 4.   *
*  The starting data are the reconstructed tracks during the offline analysis.    *
*  It takes as input the runID.                                                   *
*                                                                                 *
***********************************************************************************
*                                                                                 *
*  9. C_plotTheta_rangeTracks.C                                                   *
*  This macro allows to plot theta distributions for the reference run, starting  *
*  from the reconstructed tracks during the offline analysis. It takes as input   *
*  the runID, returning the theta-distrib. according to the range of tracks that  *
*  will be processed.                                                             *
*                                                                                 *
***********************************************************************************
