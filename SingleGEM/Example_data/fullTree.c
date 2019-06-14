#include <Riostream.h>
#include <TStyle.h>
void fullTree()
{ 
  gROOT->Reset();
  
  Double_t xf,yf,xi,yi,zi;
  TCanvas* canvas = new TCanvas();
  canvas->Divide(2,2);    // Divide the canvas further or make a new one if you want to plot more graphs
  TFile *f = new TFile("Data1.root","RECREATE"); 
  TTree *tree = new TTree("full_data","data from csv file");
  
  Long64_t nlines = tree->ReadFile("./Final_Log.csv","xf,yf,zf,sc,xi,yi,zi");
  
 
  printf(" found %lld data sets.\n",nlines);
  canvas->cd(1);
  tree->Draw("zi:xi>>zxHist","1","goff");
  TH2F* zxHist = (TH2F*)gDirectory->Get("zxHist");
  zxHist->SetTitle("Ionization points in the avalanche");
  zxHist->GetXaxis()->SetTitle("X coordinate (um)");
  zxHist->GetYaxis()->SetTitle("Y coordinate (um)");
  zxHist->GetZaxis()->SetTitle("No. of electrons");
  zxHist->Draw("scat"); 
  
  canvas->cd(2);
  //Plotting a histogram of the X spread
  tree->Draw("xf>>xHist","sc==-5 && zf==-2000.0","goff");
  TH1F* xHist = (TH1F*)gDirectory->Get("xHist");
  xHist->Fit("gaus");
  xHist->SetTitle("Histogram of the spread in X at the anode");
  xHist->GetXaxis()->SetTitle("X coordinate (um)");
  xHist->GetYaxis()->SetTitle("No. of electrons");
  xHist->Draw();
  
  canvas->cd(3);
  //Plotting a histogram of the Y spread
  tree->Draw("yf>>yHist","sc==-5 && zf==-2000.0","goff");
  TH1F* yHist = (TH1F*)gDirectory->Get("yHist");
  yHist->Fit("gaus");
  yHist->SetTitle("Histogram of the spread in Y at the anode");
  yHist->GetXaxis()->SetTitle("Y coordinate (um)");
  yHist->GetYaxis()->SetTitle("No. of electrons");
  yHist->Draw();
  
  tree->Write();
   

   f->Write();
}

