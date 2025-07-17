*****************************************************************************************
*                                                                                       *
*            PCA: Principal Component Analysis applied on IRRAD4-Data                   *
*                                                                                       *
*****************************************************************************************
*                                                                                       *
*  This directory contains all the information about the Principal Component Analysis   *
*  applied on Data acquired during the experiment IRRAD$ @IFUSP - Univ. of Sao Paulo.   *
*  The entire process of this application points to the following objectives:           *
*                                                                                       *
*     1. complexity reduction of the tracks, by preserving the the most relevant info;  *
*     2. highlight correlations between the experimentally measured variables like      *
*        position, charge and multiplicity;                                             *
*     3. identify groups of similar tracks (thus distinguishing physical events, noise  *
*        and anomalies;                                                                 *
*     4. data-preparation for clustering, classification and/or filtering.              *
*                                                                                       *
*  Starting from the "tracks_runID.root" coming by "dataProcessing.sh" (having already  *
*  applied trackGenerator.C), we need to evaluate:                                      *
*                                                                                       *
*     1. average position of the cluster within the segmented row;                      *
*     2. standard deviation of this position to take into account the dispersion;       *
*     3. total collected charge by summing the charge collected from all the pads and   *
*        the unsegmented strips;                                                        *
*     4. maximum charge of the pad;                                                     *
*     5. pad multiplicity;                                                              *
*     6. average position of the pads;                                                  *
*     7. standard deviation of the pad position;                                        *
*     8. strip type (semgented=1, non-segmented=0);                                     *
*
