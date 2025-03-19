******************************************************************************************************************
*                                                                                                                *
*                                  README ANTONINO PITRONACI                                                     *
*                                                                                                                *
******************************************************************************************************************
*                                                                                                                *
*   This file contains a bunch of information about analysis tracker after 05/06/2024, using macros you can      *
*   find in previous directories.                                                                                *
*                                                                                                                *
*   ------                                                                                                       *
*   C_plot_theta.C                                                                                               *
*   ------                                                                                                       *
*   To launch this macro, type:       										 *
*  														 *
* 					 ".x C_plot_theta.C(#run_number)" 					 *
*														 *
*   being "#run_number" just the reference number to the run (e.g. 115, 116, ...) coming from last updates on    *
*   macro B_trackGenerator.C. A degenerate-plot about theta distribution will be produced. In particular:	 *
*														 *
* 	- histoTheta: theta distribution without constraints;							 * 
*   	- h_theta_geq2000: theta distribution assuming sic_charge>2000;						 *
*	- h_theta_rms: theta distribution with additional constraint on rms<2.5 to avoid double tracks;		 *
*														 *
*   ------                                                                                                       *
*   C_plot_phi.C                                                                                                 *
*   ------                                                                                                       *
*   To launch this macro, type:       										 *
*  														 *
* 					 ".x C_plot_phi.C(#run_number)" 					 *
*														 *
*   being "#run_number" just the reference number to the run (e.g. 115, 116, ...) coming from last updates on    *
*   macro B_trackGenerator.C. A degenerate-plot about phi distribution will be produced. In particular:	         *
*														 *
* 	- histoPhi: theta distribution without constraints;							 * 
*   	- h_phi_geq2000: theta distribution assuming sic_charge>2000;						 *
*	- h_phi_rms: theta distribution with additional constraint on rms<2.5 to avoid double tracks;		 *
