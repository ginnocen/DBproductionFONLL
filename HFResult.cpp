#include "HFResult.h"
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"



HFResult::HFResult(TString namefileMC, TString namefileData) {
  _namefileMC=namefileMC;
  _namefileData=namefileData;
  LoadHisto();
}

void HFResult::LoadHisto(){
  
    _fileMC = new TFile(_namefileMC.Data());
    _fileData = new TFile(_namefileData.Data());

	_hEff = (TH1F*)_fileMC->Get("hEff");
	_hYield = (TH1F*)_fileData->Get("hPt");
    _hCorrYield=(TH1F*)_fileData->Get("hPt");
	  TH1F*hEffDen = (TH1F*)_fileMC->Get("hEff");
	_hCorrYield->Divide(hEffDen);
	
}




