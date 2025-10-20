//###################################################################################################
//#   This macro allows you to obtain the lateral dispersion (angular straggling) from a SRIM-output.
//#   REMEMBER: to correctly read the SRIM-output file, first transform "," in "."
//#
//###################################################################################################
//#   creatd jan 2025 by A. Pitronaci
//#
//#######################################
//#
//#   updated: January 2025 by A. Pitronaci
//# 
//###################################################################################################

using namespace TMath;

const TString DataTable = "SRIM_output/16O/ROW4/TRANSMIT.txt";
const Int_t N_rows = 11000;


void straggling()
  {
      
      /* Required variables */
      Double_t st_y, st_z = 0.;
      Double_t chi_y,chi_z = 0.;
      Double_t n_y,n_z = 0.;
      Double_t max_y,max_z = 0.;
      Double_t a_y,b_y,a_z,b_z = 0.;
      ifstream input;
      ofstream output;
      string bad_lines;
      char bad_inf[3];
      Double_t provv = 0.;
      Double_t provv1 = 0.;
      Double_t provv2 = 0.;
      Double_t provv3 = 0.;
      Double_t provv4 = 0.;
      Double_t cosz = 0.;
      Double_t y = 0.;
      Double_t z = 0.;
      Double_t theta = 0.;
      TH1D *h_y = new TH1D("Lateral dispersion y","Lateral dispersion y",25,-1,1);
      h_y->GetXaxis()->SetTitle("y (mm)");
      h_y->GetYaxis()->SetTitle("Counts");
      h_y->SetFillColor(kBlue);
      h_y->SetLineColor(kBlue+1);
      h_y->SetFillStyle(3001);
      
      TH1D *h_z = new TH1D("Lateral dispersion z","Lateral dispersion z",25,-1,1);
      h_z->GetXaxis()->SetTitle("z (mm)");
      h_z->GetYaxis()->SetTitle("Counts");
      h_z->SetFillColor(kGreen+1);
      h_z->SetLineColor(kGreen+2);
      h_z->SetFillStyle(3001);
      
      TH2D *print = new TH2D("YZ-dispersion","YZ-dispersion",25,-15,15,25,-15,15);
      print->GetXaxis()->SetTitle("y (mm)");
      print->GetYaxis()->SetTitle("z (mm)");
      print->GetXaxis()->SetRangeUser(-5,5);
      print->GetYaxis()->SetRangeUser(-5,5);
      
      TF1 *norm = new TF1("norm","gaus",-1,1);
      norm->SetParameters(1,0,1);
      
      /*opening file*/
       input.open(DataTable);
       if(input.fail())
         {
             cout << "Access denied to the input_file!" << endl;
         }
         
      /*reading from file - putting values in TTree*/
       for(Int_t i=0; i<12; i++)
          {
              getline(input,bad_lines);
              cout << bad_lines << endl;
          }
       for(Int_t i=12; i<N_rows;i++)
          {
              input >> bad_inf >> provv1 >> provv2 >> provv3 >> provv4 >> y >> z >> provv >> provv >> cosz;
              //cout << bad_inf << "   " << provv1 << "   " << provv2 <<  "   " << provv3 << "   " << provv4 << "   "  << y << "   " << z << "   " << provv << "   " << provv << "   " << "   "  << cosz << endl;
              y = y/10000000;
              h_y->Fill(y);
              z = z/10000000;
              h_z->Fill(z);
              print->Fill(y/10000000,z/10000000);
          }
       
       /* Visualization block */
       
       TCanvas *c = new TCanvas("c","c",1700,600);
       c->Divide(2,1);
       c->cd(1);
       max_y = 0.5*(h_y->GetMaximum());
       a_y = (25 - h_y->FindFirstBinAbove(max_y,1,1,-1));                              // Determined as it is to constraint the fit around the maximum
       b_y = (h_y->FindLastBinAbove(max_y,1,1,-1) - 25);
       cout << "a_y: " << a_y << "   b_y: " << b_y << endl;
       h_y->Fit("norm","","",-1/6,1/6);
       st_y = 2.35*norm->GetParameter(2);
       char tity[100];
       sprintf(tity,"ROW4 - FWHM: %.2f mm",st_y);
       TText *ty = new TText(-0.9,0.5*h_y->GetMaximum(),tity);
       ty->SetTextSize(0.035);
       TLatex *t0 = new TLatex(-0.95,0.6*h_y->GetMaximum(),"^{16}O @ 64.0 MeV P = 10.0 mbar");
       t0->SetTextSize(0.035);
       chi_y = norm->GetChisquare()/norm->GetNDF();
       n_y = norm->GetNDF();
       h_y->Draw();
       t0->Draw("SAME");
       ty->Draw("SAME");
       norm->Draw("SAME");
       c->cd(2);
       max_z = 0.5*(h_z->GetMaximum());
       a_z = (25 - h_z->FindFirstBinAbove(max_z,1,1,-1));
       b_z = (h_z->FindLastBinAbove(max_z,1,1,-1) - 25);
       cout << "a_z: " << a_z << "   b_z: " << b_z << endl;
       norm->SetParameters(0,0,0);
       h_z->Fit("norm","","",-1/6,1/6);
       st_z = 2.35* norm->GetParameter(2);
       char titz[100];
       sprintf(titz,"ROW4 - FWHM: %.2f mm",st_z);
       TText *tz = new TText(-0.9,0.5*h_z->GetMaximum(),titz);
       tz->SetTextSize(0.035);
       chi_z = norm->GetChisquare()/norm->GetNDF();
       n_z = norm->GetNDF();
       h_z->Draw();
       t0->Draw("SAME");
       tz->Draw("SAME");
       norm->Draw("SAME");
       c->SaveAs("SRIM_output/16O/ROW4/LateralDispersions.png");
       
       output.open("SRIM_output/16O/ROW4/outputFit.txt");
       output << "                  Straggling ROW4" << endl << endl;
       output << "        FWHM(y): 2.35*" << st_y << " = " << 2.35*st_y << "     FWHM(z): 2.35*" << st_z << " = " << 2.35*st_z << endl;
       output << "        chi(y): " << chi_y << "     chi(z): " << chi_z << endl;
       output << "        NDoF(y): " << n_y << "         NDoF(z): " << n_z << endl;
       
       TCanvas *c1 = new TCanvas("c1","c1");
       c1->cd();
       print->Draw("CONT4");
       c1->SaveAs("SRIM_output/16O/ROW4/2D_dispersion.png");
  }

