**********************************************************************************
*                                                                                *
*                   README ANALYSIS STRATEGY FOR STUDYING                        *
*                        THE GAS TRACKER RESOLUTION                              *
*                                                                                *
**********************************************************************************
*                                                                                *
*  In order to study the gas tracker resolution, we have to proceed in different *
*  ways, according to the type of probe used for the characterization:           *
*                                                                                *
*     1. radioactive alpha source: all the events are ok;                        *
*     2. collimated beam: the acquired run has to be first cleaned using a TCut  *
*        i.e. a graphycal cut around the blob appearing in the (x0,x1) 2D-plot   *
*        in order to concentrate on the event characterized by a definite angle  *
*        theta.                                                                  *
*                                                                                *
*  Once the distinction is made, we have to define two types of variables that   *
*  are object of analysis:                                                       *
*                                                                                *
*     1. track-variables: the angular coefficient, the intercept, the single     *
*        offset,... i.e. all the variables referred to the nominal track;        *
*     2. global variables: these are the variables obtained by histograms where  *
*        are reported track-variables, for instance the standard deviation that  *
*        characterizes the histograms of the run intercepts, as weel as the one  *
*        of the angular coefficients, and so on.                                 *
*                                                                                *
* ------------------------------------------------------------------------------ *
*                                                                                *
*  Charge Distribution (chargeDistr.C)                                           *
*  To study the charge distribution for erach row, be sure that the TCutG is     *
*  properly arranged very closed to the blob in the (x0,x1) 2D-plot. Thus, the   *
*  main global information (i.e. theta mean, stDEV of the charge distribution    *
*  and relative FWHM, charge distribution width on the pad-plane and the average *
*  maximum amplitude of the signals). These information are important for the    *
*  next step, i.e. the study on the charge distribution width. The main outputs  *
*  are going to be saved in a devoted directory (i.e. ChargeDistrib).            *
*                                                                                *
* ------------------------------------------------------------------------------ *
*                                                                                *
*  Offset evaluaiton (systematicOffset.C)                                        *
*  This represents the first step to study the intrisic resolution of the gas    *
*  tracker. In particular, all the centroids obtained adopting the centre of     *
*  gravity algorithm (5 centroids for the signle track) "could be" affected by   *
*  an offset. Thus, we have to evaluate the existence of a systematic offset in  *
*  order to correct it and re-align all the tracks. The outputs are saved in two *
*  different directories, i.e. "Tracks" (for the best tracks coming by the fit)  *
*  and "TrackQualityControl" (for the outputs coming by offset evaluation, the   *
*  amplitudes, chi_square, theta mean and intercept, Pearson coefficient, errX,  *
*  x_foc and errMQ).                                                             *
*                                                                                *
* ------------------------------------------------------------------------------ *
*                                                                                *
*  Offset correction (systematicOffset.C)                                        *
*  Having ascertained the existence of a systematic offset, this latter has to   *
*  corrected aligning the tracks. The outputs now will be affected by a fewer    *
*  error with respect to the previous one (i.e. the standard deviaitions in each *
*  plots for each global variable will decrease).                                *
*                                                                                *
* ------------------------------------------------------------------------------ *
*                                                                                *
*  Slit(s) application ()                                                        *
*  This is the step when we are going to evaliate the intrinsic resolution of    *
*  the detector. In particular, for all the times when the beam (or in general   *
*  your probe) is not perfectically collimated, we need to introduce virtual     *
*  collimation object. In our case, we are going to introduce first a slit on    *
*  ROW4, thus evaluating the x_corr in all the other rows (0,1,2,3) in order to  *
*  obtain the standard deviation and the FWHM. Once this is done, another slit   *
*  of the same size is introduced on ROW0 and again, the x_corr for the ROWS 1,  *
*  2,3. These results can be achieved since the x_corr (i.e. the x coordinates   *
*  with the systematic offset correction applied) have been saved in a TTree.    *
*                                                                                *
**********************************************************************************
