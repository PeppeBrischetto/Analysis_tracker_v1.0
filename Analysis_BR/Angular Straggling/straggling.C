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
      TH1D *h_y = new TH1D("Lateral dispersion y","Lateral dispersion y",50,-10,10);
      h_y->GetXaxis()->SetTitle("y (mm)");
      h_y->GetYaxis()->SetTitle("Counts");
      
      TH1D *h_z = new TH1D("Lateral dispersion z","Lateral dispersion z",50,-10,10);
      h_z->GetXaxis()->SetTitle("z (mm)");
      h_z->GetYaxis()->SetTitle("Counts");
      
      TH2D *print = new TH2D("YZ-dispersion","YZ-dispersion",50,-15,15,50,-15,15);
      print->GetXaxis()->SetTitle("y (mm)");
      print->GetYaxis()->SetTitle("z (mm)");
      print->GetXaxis()->SetRangeUser(-5,5);
      print->GetYaxis()->SetRangeUser(-5,5);
      
      TF1 *norm = new TF1("norm","gaus",-30,30);
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
       
       TText *ty = new TText(5.,0.5*h_y->GetMaximum(),"ROW 4");
       ty->SetTextSize(0.07);
       TText *tz = new TText(5.,0.5*h_z->GetMaximum(),"ROW 4");
       tz->SetTextSize(0.07);
       
       /* Visualization block */
       TLatex *t0 = new TLatex(82,145,"^{7}Li @ 24.0 MeV - P = 10.0 mbar");
       TCanvas *c = new TCanvas("c","c",1700,600);
       c->Divide(2,1);
       c->cd(1);
       h_y->Fit("norm","N","N",-30,30);
       st_y = 2.35*norm->GetParameter(2);
       chi_y = norm->GetChisquare()/norm->GetNDF();
       n_y = norm->GetNDF();
       h_y->Draw();
       ty->Draw("SAME");
       norm->Draw("C && SAME");
       c->cd(2);
       h_z->Fit("norm","N","N",-30,30);
       st_z = 2.35* norm->GetParameter(2);
       chi_z = norm->GetChisquare()/norm->GetNDF();
       n_z = norm->GetNDF();
       h_z->Draw();
       tz->Draw("SAME");
       norm->Draw("C && SAME");
       c->SaveAs("SRIM_output/16O/ROW4/LateralDispersions.png");
       
       output.open("SRIM_output/16O/ROW4/outputFit.txt");
       output << "                  Straggling ROW 4" << endl << endl;
       output << "        FWHM(y): " << st_y << "     FWHM(z): " << st_z << endl;
       output << "        chi(y): " << chi_y << "     chi(z): " << chi_z << endl;
       output << "        NDoF(y): " << n_y << "         NDoF(z): " << n_z << endl;
       
       TCanvas *c1 = new TCanvas("c1","c1");
       c1->cd();
       print->Draw("CONT4");
       c1->SaveAs("SRIM_output/16O/ROW4/2D_dispersion.png");
  }

