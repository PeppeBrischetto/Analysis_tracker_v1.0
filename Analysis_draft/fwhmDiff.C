//#################################################################################################################
//#   This macro allows to evaluate the resolution on the x_{i} measured on the padPlane opening virtual slists on
//#   the sensing rows of the gas tracker prorotype.
//#################################################################################################################
//#   Created December 2025 by A. Pitronaci 
//#################################################################################################################
//#   Updated:
//#################################################################################################################

#include <iostream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TLatex.h>
#include <TMath.h>
#include "TPrincipal.h"
#include "../Include/openfilesthresh.h"

using namespace std;
using namespace TMath;

const Int_t NRows = 5;
const Int_t NStrips = 11;
const Int_t NPads = 60;

const TString InputFile = "resolution/FWHM_3_2_diff_0mm_slit.txt";

void fwhmDiff(){


}
