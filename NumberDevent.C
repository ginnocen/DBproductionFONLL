#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"

using namespace std;

void NumberDevent(){
    
    double sigmaIN5TeV=70*1e-3;
    double unitconversion=1e-12;
    double common_normPP=unitconversion/sigmaIN5TeV*2;
    Double_t DzeroBRchain=0.0388;
    Double_t BplusBRchain=6.12*1e-5;
    Double_t Ncoll_0100=392.;
    
    double normPP[2]={0.557*common_normPP*DzeroBRchain,0.401*common_normPP*BplusBRchain};
    
    TFile *inDPPYield = TFile::Open("inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/hPtSpectrumDzeroPPMB.root");
    TFile *inDPbPbYield = TFile::Open("inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/hPtSpectrumDzeroPbPbMB.root");
    
    TFile *inDPPMBMC = TFile::Open("inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/MCstudiesPPMB.root");
    TFile *inDPbPbMBMC = TFile::Open("inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/MCstudiesPbPbMB.root");
    TFile *inDPPMC = TFile::Open("inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/MCstudiesPP.root");
    TFile *inDPbPbMC = TFile::Open("inputMeasurement/DntupleRunIIfinal/CrossSection/ROOTfiles/MCstudiesPbPb.root");
    TFile *inBPPMC = TFile::Open("inputMeasurement/BplusRAA2015RunII/CrossSection/ROOTfiles/MCstudiesPP.root");
    TFile *inBPbPbMC = TFile::Open("inputMeasurement/BplusRAA2015RunII/CrossSection/ROOTfiles/MCstudiesPbPb.root");

	TH1F *hEffDPPMB = (TH1F*)inDPPMBMC->Get("hEff");
	TH1F *hEffDPbPbMB = (TH1F*)inDPbPbMBMC->Get("hEff");
	TH1F *hEffDPP = (TH1F*)inDPPMC->Get("hEff");
	TH1F *hEffDPbPb = (TH1F*)inDPbPbMC->Get("hEff");
	TH1F *hEffBPP = (TH1F*)inBPPMC->Get("hEff");
	TH1F *hEffBPbPb = (TH1F*)inBPbPbMC->Get("hEff");
	
	TH1F *hDPPYield = (TH1F*)inDPPYield->Get("hPt");
	TH1F *hDPbPbYield = (TH1F*)inDPbPbYield->Get("hPt");
	hDPPYield->Divide(hEffDPPMB);
	hDPbPbYield->Divide(hEffDPbPbMB);
	hDPPYield->Scale(1/(2*1e9));
	hDPbPbYield->Scale(1/(300*1e6));
	
	TFile *inFileD = TFile::Open("output_pp_d0meson_5TeV_y1.root");
	TH1F *hYieldDPP = (TH1F*)inFileD->Get("hpt");
	hYieldDPP->Scale(normPP[0]);
	TH1F *hYieldDPbPb = (TH1F*)hYieldDPP->Clone("hYieldDPbPb");
	hYieldDPbPb->Scale(Ncoll_0100);

	TFile *inFileB = TFile::Open("output_pp_Bplusmeson_5TeV_y1.root");
	TH1F *hYieldBPP = (TH1F*)inFileB->Get("hpt");
	hYieldBPP->Scale(normPP[1]);
	TH1F *hYieldBPbPb = (TH1F*)hYieldBPP->Clone("hYieldBPbPb");
	hYieldBPbPb->Scale(Ncoll_0100);

	TFile *inFileB7TeV = TFile::Open("output_pp_Bplusmeson_7TeV_y1.root");
	TH1F *hYieldBPP7TeV = (TH1F*)inFileB7TeV->Get("hpt");
	hYieldBPP7TeV->Scale(normPP[1]);
	TH1F *hYieldBPbPb7TeV = (TH1F*)hYieldBPP7TeV->Clone("hYieldBPbPb");
	hYieldBPbPb7TeV->Scale(Ncoll_0100);

	TH1F *hYieldBPP7TeVOver5TeV = (TH1F*)hYieldBPP7TeV->Clone("hYieldBPP7TeVOver5TeV");
	TH1F *hDenhYieldBPP5TeV = (TH1F*)hYieldBPP->Clone("hDenhYieldBPP5TeV");
    hYieldBPP7TeVOver5TeV->Divide(hDenhYieldBPP5TeV);

   TCanvas*c=new TCanvas("c","c",1000,1000);
    c->Divide(3,2);
    c->cd(1);
    gPad->SetLogy();
    TH2F* hemptyPP=new TH2F("hemptyPP","",50,0,100.,10,1e-16,1);  
    hemptyPP->GetXaxis()->CenterTitle();
    hemptyPP->GetYaxis()->CenterTitle();
    hemptyPP->GetYaxis()->SetTitle("N. of candidates / MB event / GeV");
    hemptyPP->GetXaxis()->SetTitle("p_{T}");
    hemptyPP->GetXaxis()->SetTitleOffset(0.9);
    hemptyPP->GetYaxis()->SetTitleOffset(1.4);
    hemptyPP->GetXaxis()->SetTitleSize(0.05);
    hemptyPP->GetYaxis()->SetTitleSize(0.05);
    hemptyPP->GetXaxis()->SetTitleFont(42);
    hemptyPP->GetYaxis()->SetTitleFont(42);
    hemptyPP->GetXaxis()->SetLabelFont(42);
    hemptyPP->GetYaxis()->SetLabelFont(42);
    hemptyPP->GetXaxis()->SetLabelSize(0.035);
    hemptyPP->GetYaxis()->SetLabelSize(0.035);  
    hemptyPP->Draw();
    hYieldDPP->Draw("same");
    hYieldBPP->Draw("same");
    hDPPYield->Draw("same");
    hYieldBPP7TeV->Draw("same");
    c->cd(2);
    gPad->SetLogy();
    TH2F* hemptyPbPb=new TH2F("hemptyPbPb","",50,0,100.,10,1e-16,1);  
    hemptyPbPb->GetXaxis()->CenterTitle();
    hemptyPbPb->GetYaxis()->CenterTitle();
    hemptyPbPb->GetYaxis()->SetTitle("N. of candidates / MB event / GeV");
    hemptyPbPb->GetXaxis()->SetTitle("p_{T}");
    hemptyPbPb->GetXaxis()->SetTitleOffset(0.9);
    hemptyPbPb->GetYaxis()->SetTitleOffset(1.4);
    hemptyPbPb->GetXaxis()->SetTitleSize(0.05);
    hemptyPbPb->GetYaxis()->SetTitleSize(0.05);
    hemptyPbPb->GetXaxis()->SetTitleFont(42);
    hemptyPbPb->GetYaxis()->SetTitleFont(42);
    hemptyPbPb->GetXaxis()->SetLabelFont(42);
    hemptyPbPb->GetYaxis()->SetLabelFont(42);
    hemptyPbPb->GetXaxis()->SetLabelSize(0.035);
    hemptyPbPb->GetYaxis()->SetLabelSize(0.035);  
    hemptyPbPb->Draw();
    hYieldDPbPb->Draw("same");
    hYieldBPbPb->Draw("same");
    hDPbPbYield->Draw("same");
    hYieldBPbPb7TeV->Draw("same");
    c->cd(3);
    //gPad->SetLogy();
    TH2F* hRatio=new TH2F("hRatio","",50,0,100.,10,0.,5.);  
    hRatio->GetXaxis()->CenterTitle();
    hRatio->GetYaxis()->CenterTitle();
    hRatio->GetYaxis()->SetTitle("7 TeV/5 TeV B+");
    hRatio->GetXaxis()->SetTitle("p_{T}");
    hRatio->GetXaxis()->SetTitleOffset(0.9);
    hRatio->GetYaxis()->SetTitleOffset(1.4);
    hRatio->GetXaxis()->SetTitleSize(0.05);
    hRatio->GetYaxis()->SetTitleSize(0.05);
    hRatio->GetXaxis()->SetTitleFont(42);
    hRatio->GetYaxis()->SetTitleFont(42);
    hRatio->GetXaxis()->SetLabelFont(42);
    hRatio->GetYaxis()->SetLabelFont(42);
    hRatio->GetXaxis()->SetLabelSize(0.035);
    hRatio->GetYaxis()->SetLabelSize(0.035);  
    hRatio->Draw();
    hYieldBPP7TeVOver5TeV->Draw("same");
    c->cd(4);
    gPad->SetLogy();
    TH2F* hemptyPPEff=new TH2F("hemptyPPEff","",50,0,100.,10,0.001,1.);  
    hemptyPPEff->GetXaxis()->CenterTitle();
    hemptyPPEff->GetYaxis()->CenterTitle();
    hemptyPPEff->GetYaxis()->SetTitle("Efficiency");
    hemptyPPEff->GetXaxis()->SetTitle("p_{T}");
    hemptyPPEff->GetXaxis()->SetTitleOffset(0.9);
    hemptyPPEff->GetYaxis()->SetTitleOffset(1.4);
    hemptyPPEff->GetXaxis()->SetTitleSize(0.05);
    hemptyPPEff->GetYaxis()->SetTitleSize(0.05);
    hemptyPPEff->GetXaxis()->SetTitleFont(42);
    hemptyPPEff->GetYaxis()->SetTitleFont(42);
    hemptyPPEff->GetXaxis()->SetLabelFont(42);
    hemptyPPEff->GetYaxis()->SetLabelFont(42);
    hemptyPPEff->GetXaxis()->SetLabelSize(0.035);
    hemptyPPEff->GetYaxis()->SetLabelSize(0.035);  
    hemptyPPEff->Draw();
    hEffDPPMB->Draw("same");
    hEffDPP->Draw("same");
    hEffBPP->Draw("same");
    c->cd(5);
    gPad->SetLogy();
    TH2F* hemptyPbPbEff=new TH2F("hemptyPbPbEff","",50,0,100.,10,0.001,1.);  
    hemptyPbPbEff->GetXaxis()->CenterTitle();
    hemptyPbPbEff->GetYaxis()->CenterTitle();
    hemptyPbPbEff->GetYaxis()->SetTitle("Efficiency");
    hemptyPbPbEff->GetXaxis()->SetTitle("p_{T}");
    hemptyPbPbEff->GetXaxis()->SetTitleOffset(0.9);
    hemptyPbPbEff->GetYaxis()->SetTitleOffset(1.4);
    hemptyPbPbEff->GetXaxis()->SetTitleSize(0.05);
    hemptyPbPbEff->GetYaxis()->SetTitleSize(0.05);
    hemptyPbPbEff->GetXaxis()->SetTitleFont(42);
    hemptyPbPbEff->GetYaxis()->SetTitleFont(42);
    hemptyPbPbEff->GetXaxis()->SetLabelFont(42);
    hemptyPbPbEff->GetYaxis()->SetLabelFont(42);
    hemptyPbPbEff->GetXaxis()->SetLabelSize(0.035);
    hemptyPbPbEff->GetYaxis()->SetLabelSize(0.035);  
    hemptyPbPbEff->Draw();
    hEffDPbPbMB->Draw("same");
    hEffDPbPb->Draw("same");
    hEffBPbPb->Draw("same");

	cout<<"total number of D->Kpi pt integrated PP ="<<hYieldDPP->Integral(1,hYieldDPP->GetNbinsX())<<endl;
	cout<<"total number of B->mumuK pt integrated PP="<<hYieldBPP->Integral(1,hYieldBPP->GetNbinsX())<<endl;
	
	cout<<"total number of D->Kpi pt integrated PbPb ="<<hYieldDPbPb->Integral(1,hYieldDPbPb->GetNbinsX())<<endl;
	cout<<"total number of B->mumuK pt integrated PbPb="<<hYieldBPbPb->Integral(1,hYieldBPbPb->GetNbinsX())<<endl;

}