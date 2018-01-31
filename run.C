#include "Plotting.cpp"
#include <vector>
#include "utilities.h"
#include <TROOT.h>
#include <TStyle.h>
#include <iostream>
#include <iostream>
#include <TF1.h>
#include <TH1F.h>

void run(){


  TH1F* fitCharged(TH1F*);
  TH1F* reduceHistoInput(TH1F*,double );
  
  TH1F* reduceHistoInput(TH1F*,double);
  TH1F* reduceHistoInput(TH1F*);
  double calculateEloss(double, double, int);

  gStyle->SetTextSize(0.05);
  gStyle->SetTextFont(42);
  gStyle->SetPadRightMargin(0.043);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.145);
  gStyle->SetTitleX(.0f);
  gStyle->SetTitleY(.0f);
  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(0.8);
  
  TString namehempty;

  TFile *files[samples];
  TH1F *hpt_ppreference[samples];  

  TH1F *hpt[samples][nElossType];  
  double value=0;
  int lowerx[samples];
  int  higherx[samples];


  for (int index=0;index<samples;index++){
    files[index]=new TFile(namefiles[index].Data());
    TH1F*htemp;
    TH1F*htemp2;
    if (index!=2) {
      htemp=(TH1F*)files[index]->Get(nameHistoInput[index].Data());
      hpt_ppreference[index]=(TH1F*)reduceHistoInput(htemp,lowerrangex[index]); 
    }
    else {
      htemp2=(TH1F*)files[index]->Get(nameHistoInput[index].Data());
      htemp=(TH1F*)fitCharged(htemp2);
      hpt_ppreference[index]=(TH1F*)reduceHistoInput(htemp,lowerrangex[index]); 
    }
    
    nbins[index]=hpt_ppreference[index]->GetNbinsX();
    lowerx[index]=0;
    higherx[index]=100;
    
   for (int indexeloss=0;indexeloss<nElossType;indexeloss++){
      TString namehisto="hpt"+nameParticle[index]+nameeloss[indexeloss];
      hpt[index][indexeloss]=new TH1F(namehisto.Data(),namehisto.Data(),nbins[index],lowerx[index],higherx[index]);
      hpt[index][indexeloss]->SetLineColor(colours[indexeloss]);
      hpt[index][indexeloss]->SetMarkerColor(colours[indexeloss]);
      hpt[index][indexeloss]->SetLineWidth(widthline[indexeloss]);

    }
  }

  for (int index=0;index<samples;index++){
    for (int random_index=0;random_index<1e8;random_index++){
    
      value=hpt_ppreference[index]->GetRandom();  
      double quenchedvalue=-1;
      for (int indexeloss=0;indexeloss<nElossType;indexeloss++){        
        quenchedvalue=calculateEloss(value,eloss_value[indexeloss],eloss_method[indexeloss]);
        hpt[index][indexeloss]->Fill(quenchedvalue);
      }
    }
  }

    TFile *foutput=new TFile("fileout.root","recreate");
    foutput->cd();
    for (int index=0;index<samples;index++){
      for (int indexeloss=0;indexeloss<nElossType;indexeloss++){        
        hpt[index][indexeloss]->Write();      
      }
    }
}



TH1F* reduceHistoInput(TH1F* histo,double ptcut){

  const int nbinsX=histo->GetNbinsX();
  std::vector<double> reducedarraybins;
  std::vector<double> reducedarrayvalues;
    
  for (int index=1;index<=nbinsX; index++){
    if (histo->GetBinLowEdge (index)>ptcut){
      reducedarraybins.push_back(histo->GetBinLowEdge(index));
      reducedarrayvalues.push_back(histo->GetBinContent(index));
    }
  }
  reducedarraybins.push_back(histo->GetBinLowEdge(nbinsX)+histo->GetBinWidth(nbinsX));
  cout<<"reducedarrayvalues"<<reducedarraybins[reducedarraybins.size()]<<endl;
  
  
  const int nbinsreduced=reducedarraybins.size()-1;
  double arraybinreduced[nbinsreduced+1];
  
  for (int index=0;index<nbinsreduced+1;index++){
    arraybinreduced[index]=reducedarraybins[index];
    cout<<arraybinreduced[index]<<endl;
  }  
  
  TH1F*hreduced=new TH1F("hreduced","hreduced",nbinsreduced,arraybinreduced);
  for (int index=1;index<=hreduced->GetNbinsX(); index++){
    hreduced->SetBinContent(index,reducedarrayvalues[index-1]);
  }
  return hreduced;
}


TH1F* fitCharged(TH1F* histo){

  TF1*fTsallis = new TF1( "fTsallis", "[0]*(1+([3]-1)*(x-[2])/[1])**([3]/(1-[3]))", 0.0,400);          
  fTsallis->SetParameter(0,5.15760e-07);
  fTsallis->SetParameter(1,1.14520e+00);
  fTsallis->SetParameter(2,5.64780e+00);
  fTsallis->SetParameter(3,1.17624e+00);
  histo->Fit("fTsallis","R");
  TH1F*hFit=new TH1F("hFit","hFit",1600,0.,400);
  
  for (int index=1;index<=hFit->GetNbinsX();index++){
    hFit->SetBinContent(index,fTsallis->Eval(hFit->GetBinCenter(index)));
  }
  return hFit; 
}


double calculateEloss(double ptvalue, double factor, int method){

  double ptafterEloss=0;
  
  if (method==0){
    if ((ptvalue-factor)>0) ptafterEloss= ptvalue-factor;
    else ptafterEloss=0.3;
  }
  
  if (method==1){
    ptafterEloss= ptvalue-factor*ptvalue;
  }
  return ptafterEloss; 
}
