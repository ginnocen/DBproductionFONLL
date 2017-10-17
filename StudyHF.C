#include "HFResult.cpp"
#include "Plotting.cpp"


int StudyHF(){
  
  const int nSampleParts=2; 

  TString filenameDDataPP[nSampleParts]={"inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/hPtSpectrumDzeroPPMB.root","inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/hPtSpectrumDzeroPP.root"};
  TString filenameDDataPbPb[nSampleParts]={"inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/hPtSpectrumDzeroPbPbMB.root","inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/hPtSpectrumDzeroPbPbGJ.root"};
  TString filenameDMCPP[nSampleParts]={"inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/MCstudiesPPMB.root","inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/MCstudiesPP.root"};
  TString filenameDMCPbPb[nSampleParts]={"inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/MCstudiesPbPbMB.root","inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/MCstudiesPbPb.root"};

  HFResult*hfDPP[nSampleParts];
  HFResult*hfDPbPb[nSampleParts];
  
  Plotting*myplot=new Plotting();  

  TH1F*hDEffPP[nSampleParts];
  TH1F*hDEffPbPb[nSampleParts];
  TH1F*hDYieldPP[nSampleParts];
  TH1F*hDYieldPbPb[nSampleParts];
    
  TH2F*hDemptyYieldPP=(TH2F*)myplot->GetEmpty("hDemptyYieldPP","p_{T}","N. of candidates / GeV",0,100,0.01,1e10);
  TH2F*hDemptyYieldPbPb=(TH2F*)myplot->GetEmpty("hDemptyYieldPbPb","p_{T}","N. of candidates / GeV",0,100,0.01,1e10);
  TH2F*hDemptyEffPP=(TH2F*)myplot->GetEmpty("hDemptyEffPP","p_{T}","N. of efficiency",0,100,0.001,3.);
  TH2F*hDemptyEffPbPb=(TH2F*)myplot->GetEmpty("hDemptyEffPbPb","p_{T}","N. of efficiency",0,100,0.001,3.);
  
   for(int i=0;i<nSampleParts;i++){
     hfDPP[i]=new HFResult(filenameDMCPP[i],filenameDDataPP[i]);
     hfDPbPb[i]=new HFResult(filenameDMCPbPb[i],filenameDDataPbPb[i]);
     hDEffPP[i]=hfDPP[i]->GetEff();
     hDEffPbPb[i]=hfDPbPb[i]->GetEff();
     hDYieldPP[i]=hfDPP[i]->GetYield();
     hDYieldPbPb[i]=hfDPbPb[i]->GetYield();
   }
    
   TCanvas*c=new TCanvas("c","c",1000,1000);
    c->Divide(2,2);
    c->cd(1);
    gPad->SetLogy();
    hDemptyYieldPP->Draw();
    for (int index=0;index<nSampleParts;index++){
      hDYieldPP[index]->Draw("same");
    }
    
    c->cd(2);
    gPad->SetLogy();
    hDemptyYieldPbPb->Draw();
    for (int index=0;index<nSampleParts;index++){
      hDYieldPbPb[index]->Draw("same");
    }
    
    c->cd(3);
    gPad->SetLogy();
    hDemptyEffPP->Draw();
    for (int index=0;index<nSampleParts;index++){
      hDEffPP[index]->Draw("same");
    }
    
    c->cd(4);
    gPad->SetLogy();
    hDemptyEffPbPb->Draw();
    for (int index=0;index<nSampleParts;index++){
      hDEffPbPb[index]->Draw("same");
    }
  c->SaveAs("canvas_DataYield.pdf");
  return 0;

}


int DPredictions(bool isnormalised=false){

   //0=D
   //1=B
  Plotting*myplot=new Plotting();    
  double sigmaIN5TeV=70*1e-3;
  double unitconversion=1e-12;
  double common_normPP=unitconversion/sigmaIN5TeV*2;
  double DzeroBRchain=0.0388;
  double BplusBRchain=6.12*1e-5;
  double Ncoll_0100=392.;
  double norm[2]={0.557*common_normPP*DzeroBRchain,0.401*common_normPP*BplusBRchain};

  TString filenames[2]={"output_pp_d0meson_5TeV_y1.root","output_pp_Bplusmeson_5TeV_y1.root"};
  TFile *files[2];
   TH1F *hYieldPP[2];
   TH1F *hYieldPbPb[2];
   
   for (int index=0;index<2;index++){
     files[index]=new TFile(filenames[index].Data(),"read");
     hYieldPP[index]= (TH1F*)files[index]->Get("hpt");
     hYieldPbPb[index]= (TH1F*)files[index]->Get("hpt");
     if(isnormalised) hYieldPP[index]->Scale(norm[index]);
     hYieldPbPb[index]=(TH1F*)hYieldPP[index]->Clone("hpt");
     hYieldPbPb[index]->Scale(Ncoll_0100);

   }
   TString string_yaxis;
   double ymin;
   double ymax;
   
   if(isnormalised){
     string_yaxis="N. of candidates / Event / GeV";
     ymin=1e-15;
     ymax=1;
   }
   else{
     string_yaxis="cross section (1/pb/GeV)";
     ymin=1;
     ymax=1e13;
   }
  TH2F*hDemptyYieldPP=(TH2F*)myplot->GetEmpty("hDemptyYieldPP","p_{T}",string_yaxis.Data(),0,100,ymin,ymax);
  TH2F*hDemptyYieldPbPb=(TH2F*)myplot->GetEmpty("hDemptyYieldPbPb","p_{T}",string_yaxis.Data(),0,100,ymin,ymax);
  
  TLegend*legPP=(TLegend*)myplot->GetLegend(0.4205154,0.7232817,0.8200301,0.8721362);
  TLegend*legPbPb=(TLegend*)myplot->GetLegend(0.4205154,0.7232817,0.8200301,0.8721362);
  
  int colours[2]={1,2};
  int markerstyle[2]={21,22};
  int width[2]={2,2};
  TString labePP[2]={"D pp 5 TeV","B pp 5 TeV"};
  TString labePbPb[2]={"D PbPb 5 TeV","B PbPb 5 TeV"};

  for (int index=0;index<2;index++){
    hYieldPP[index]->SetLineColor(colours[index]);
    hYieldPbPb[index]->SetLineColor(colours[index]);
    hYieldPP[index]->SetLineWidth(width[index]);
    hYieldPbPb[index]->SetLineWidth(width[index]);
    legPP->AddEntry(hYieldPP[index],labePP[index].Data(),"pl");
    legPbPb->AddEntry(hYieldPbPb[index],labePbPb[index].Data(),"pl");
  }
  
  TCanvas*c=new TCanvas("c","c",1000,500);
  c->Divide(2,1);
  c->cd(1);
  gPad->SetLogy();
  hDemptyYieldPP->Draw();
  for (int index=0;index<2;index++){
    hYieldPP[index]->Draw("same");
  }
  legPP->Draw();
  c->cd(2);
  gPad->SetLogy();
  hDemptyYieldPbPb->Draw();
  for (int index=0;index<2;index++){
    hYieldPbPb[index]->Draw("same");
  }
  legPbPb->Draw();
  c->SaveAs(Form("canvas_BDprediction_norm%d.pdf",(int)isnormalised));
  
  	cout<<"total number of D->Kpi pt integrated PP ="<<hYieldPP[0]->Integral(1,hYieldPP[0]->GetNbinsX())<<endl;
	cout<<"total number of B->mumuK pt integrated PP="<<hYieldPP[1]->Integral(1,hYieldPP[1]->GetNbinsX())<<endl;
	cout<<"total number of D->Kpi pt integrated PbPb ="<<hYieldPbPb[0]->Integral(1,hYieldPbPb[0]->GetNbinsX())<<endl;
	cout<<"total number of B->mumuK pt integrated PbPb="<<hYieldPbPb[1]->Integral(1,hYieldPbPb[1]->GetNbinsX())<<endl;


}

