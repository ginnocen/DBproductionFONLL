#include "Plotting.cpp"
#include "utilities.h"


void StudyEloss(){

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
    leg[index]=(TLegend*)myplot->GetLegend(0.2114094,0.6474695,0.6610738,0.8638743);
    
    for (int indexeloss=0;indexeloss<nElossType;indexeloss++){ 
      TString ptnames=nameeloss[indexeloss];
      legentry[index][indexeloss]=leg[index]->AddEntry(hpt[index][indexeloss],ptnames.Data(),"pl");
      legentry[index][indexeloss]->SetLineColor(colours[indexeloss]);
    }  
  }



  TCanvas*c[samples];
  TCanvas*cRAA[samples];
  TString canvasname[samples];
  TString canvasnameRAA[samples];
  

    for (int index=0;index<samples;index++){
      canvasname[index]=Form("canvasSpectraplot%s.pdf",nameParticle[index].Data());
      c[index]=new TCanvas(canvasname[index].Data(),canvasname[index].Data(),600,600);
      canvasnameRAA[index]=Form("canvasRAA%s.pdf",nameParticle[index].Data());
      cRAA[index]=new TCanvas(canvasnameRAA[index].Data(),canvasnameRAA[index].Data(),600,600);
    }

  for (int index=0;index<samples;index++){
    c[index]->cd();
    gPad->SetLogx();
    gPad->SetLogy();
    hempty[index]->Draw();    
    for (int indexeloss=0;indexeloss<nElossType;indexeloss++){
      hpt[index][indexeloss]->Draw("same");
    }
    leg[index]->Draw();
    c[index]->SaveAs(canvasname[index].Data());
  } 

   TCanvas*cRAA[samples];
  for (int index=0;index<samples;index++){
    cRAA[index]->cd();
    gPad->SetLogx();
    hemptyRAA[index]->Draw();    
    for (int indexeloss=0;indexeloss<nElossType;indexeloss++){
      TString myraa="hRAA"+nameParticle[index]+" "+nameeloss[indexeloss];
      hRAA[index][indexeloss]=(TH1F*)hpt[index][indexeloss]->Clone(myraa);
      hRAA[index][indexeloss]->Divide(hpt[index][0]);
      hRAA[index][indexeloss]->SetLineColor(colours[indexeloss]);
      hRAA[index][indexeloss]->SetMarkerColor(colours[indexeloss]);
      hRAA[index][indexeloss]->SetLineWidth(widthline[indexeloss]);
      hRAA[index][indexeloss]->Draw("same");
    }
    leg[index]->Draw();
    cRAA[index]->SaveAs(canvasnameRAA[index].Data());

  } 

  
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
      hRAAratio[indexeloss]->SetLineWidth(widthline[indexeloss]);
      hRAAratio[indexeloss]->Draw("same");
    }

  leg[0]->Draw();
  cRAARatios->SaveAs("RAARatioplot.pdf");

}
