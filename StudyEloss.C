#include "Plotting.cpp"
#include <vector>


  const int samples=3; 
  const int nElossType=5;
  const int nresults=2;
  TString namefiles[samples]={"output_pp_d0meson_5TeV_y1.root","output_pp_Bplusmeson_5TeV_y1.root","HEPData-ins1496050-v1-root.root"};
  TString nameHistoInput[samples]={"hpt","hpt","Table 7/Hist1D_y1"};

  double eloss_value[nElossType]={0,1,2,0.20,0.40};  
  double eloss_method[nElossType]={0,0,0,1,1};  

  TString nameeloss[nElossType]={"binary","eloss_flat1GeV", "eloss_flat2GeV","20percent", "40percent"};  
  TString nameParticle[samples]={"D-meson","B-meson","Charged-particle"};  
  
  int colours[nElossType]={1,2,3,4,8};
  int markerstyle[nElossType]={21,21,21,22,22};
  int width[nElossType]={2,2,2,2,2};
    
  double nbins[samples]={2000,2000,2000};
  double lowerrangex[samples]={1.,1.,1.};
  double upperrangex[samples]={100,100,100};
  double lowerrangey[samples]={4,4,4};
  double upperrangey[samples]={1e10.,1e10,1e10};
  double lowerrangeyRAA[samples]={0.,0.,0.};
  double upperrangeyRAA[samples]={4.,4.,4.};
  TString string_xaxis[samples]={"p_{T}(GeV)", "p_{T}(GeV)","p_{T}(GeV)"};


void run(){

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
      htemp2=files[index]->Get(nameHistoInput[index].Data());
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
      hpt[index][indexeloss]->SetLineWidth(width[indexeloss]);

    }
  }

  for (int index=0;index<samples;index++){
    for (int random_index=0;random_index<1e7;random_index++){
    
      value=hpt_ppreference[index]->GetRandom();  
      double quenchedvalue=-1;
      for (int indexeloss=0;indexeloss<nElossType;indexeloss++){        
        quenchedvalue=calculateEloss(value,eloss_value[indexeloss],eloss_method[indexeloss]);
        hpt[index][indexeloss]->Fill(quenchedvalue);
      }
    }
  }

    TFile *foutput=new TFile("fileout.root","recreate");
    for (int index=0;index<samples;index++){
      for (int indexeloss=0;indexeloss<nElossType;indexeloss++){        
        hpt[index][indexeloss]->Write();      
      }
    }
}

void StudyEloss(){

    TH1F *hpt[samples][nElossType];  
    TFile *finput=new TFile("fileout.root");
    for (int index=0;index<samples;index++){
      for (int indexeloss=0;indexeloss<nElossType;indexeloss++){     
        TString namehisto="hpt"+nameParticle[index]+nameeloss[indexeloss];
        cout<<namehisto<<endl;
        hpt[index][indexeloss]=(TH1F*)finput->Get(namehisto.Data());   
      }
    }

  TH1F *hRAA[samples][nElossType];
  TH1F *hRAAratio[nElossType];
  
  TLegend*leg[samples];
  TLegendEntry *legentry[samples][nElossType];
  
  
  Plotting*myplot=new Plotting();
  TH2F* hempty[samples];
  TH2F* hemptyRAA[samples];
  TH2F* hemptyRAAratio;
  TLegend*leg[samples];

  

  for (int index=0;index<samples;index++){
    TString namehistoempty="hemptyPt"+nameParticle[index];
    TString namehistoemptyRAA="hemptyRAA"+nameParticle[index];
    hempty[index]=(TH2F*)myplot->GetEmpty(namehistoempty.Data(),string_xaxis[index].Data(),"Entries",lowerrangex[index],upperrangex[index],lowerrangey[index],upperrangey[index]);
    hemptyRAA[index]=(TH2F*)myplot->GetEmpty(namehistoemptyRAA.Data(),string_xaxis[index].Data(),"R_{AA}"+ nameParticle[index],lowerrangex[index],upperrangex[index],lowerrangeyRAA[index],upperrangeyRAA[index]);
    leg[index]=(TLegend*)myplot->GetLegend(0.2749591,0.4740111,0.8593539,0.848799);
    
    for (int indexeloss=0;indexeloss<nElossType;indexeloss++){ 
      TString ptnames=nameeloss[indexeloss];
      legentry[index][indexeloss]=leg[index]->AddEntry(hpt[index][indexeloss],ptnames.Data(),"pl");
      legentry[index][indexeloss]->SetLineColor(colours[indexeloss]);
    }  
  }


  TCanvas*c=new TCanvas("c","c",samples*400,400);
  c->Divide(samples,1);

  for (int index=0;index<samples;index++){
    c->cd(index+1);
    gPad->SetLogx();
    gPad->SetLogy();
    hempty[index]->Draw();    
    for (int indexeloss=0;indexeloss<nElossType;indexeloss++){
      hpt[index][indexeloss]->Draw("same");
    }
    leg[index]->Draw();
  } 
  c->SaveAs("Spectraplot.pdf");

  TCanvas*cRAA=new TCanvas("cRAA","cRAA",samples*400,400);
  cRAA->Divide(samples,1);

  for (int index=0;index<samples;index++){
    cRAA->cd(index+1);
    gPad->SetLogx();
    hemptyRAA[index]->Draw();    
    for (int indexeloss=0;indexeloss<nElossType;indexeloss++){
      TString myraa="hRAA"+nameParticle[index]+" "+nameeloss[indexeloss];
      hRAA[index][indexeloss]=(TH1F*)hpt[index][indexeloss]->Clone(myraa);
      hRAA[index][indexeloss]->Divide(hpt[index][0]);
      hRAA[index][indexeloss]->SetLineColor(colours[indexeloss]);
      hRAA[index][indexeloss]->SetMarkerColor(colours[indexeloss]);
      hRAA[index][indexeloss]->SetLineWidth(width[indexeloss]);
      hRAA[index][indexeloss]->Draw("same");
    }
    leg[index]->Draw();
  } 
  cRAA->SaveAs("RAAplot.pdf");

  TCanvas*cRAARatios=new TCanvas("cRAARatios","cRAARatios",400,400);
  cRAARatios->cd(1);
  gPad->SetLogx();
  TString namehistoemptyRAAratio="hemptyRAAratio";
  hemptyRAAratio=(TH2F*)myplot->GetEmpty(namehistoemptyRAAratio.Data(),string_xaxis[0].Data(),"B/D ratio R_{AA} ",lowerrangex[0],upperrangex[0],0.,10);


  hemptyRAAratio->Draw();
      
  for (int indexeloss=0;indexeloss<nElossType;indexeloss++){
      TString myraa="hRAAratio"+nameeloss[indexeloss];
      hRAAratio[indexeloss]=(TH1F*)hRAA[1][indexeloss]->Clone(myraa);
      hRAAratio[indexeloss]->Divide(hRAA[0][indexeloss]);
      hRAAratio[indexeloss]->SetLineColor(colours[indexeloss]);
      hRAAratio[indexeloss]->SetMarkerColor(colours[indexeloss]);
      hRAAratio[indexeloss]->SetLineWidth(width[indexeloss]);
      hRAAratio[indexeloss]->Draw("same");
    }

  leg[0]->Draw();
  cRAARatios->SaveAs("RAARatioplot.pdf");
 
}

TH1F* reduceHistoInput(TH1F* histo,double ptcut){

  const int nbins=histo->GetNbinsX();
  std::vector<double> reducedarraybins;
  std::vector<double> reducedarrayvalues;
    
  for (int index=1;index<=nbins; index++){
    if (histo->GetBinLowEdge (index)>ptcut){
      reducedarraybins.push_back(histo->GetBinLowEdge(index));
      reducedarrayvalues.push_back(histo->GetBinContent(index));
    }
  }
  reducedarraybins.push_back(histo->GetBinLowEdge(nbins)+histo->GetBinWidth(nbins));
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
