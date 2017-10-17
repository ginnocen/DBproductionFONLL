#include "TString.h"
#include "TH1F.h"
#include "TFile.h"

class HFResult {
  private:

  TString _namefileMC;
  TString _namefileData;
  TH1F*_hEff;
  TH1F*_hYield;
  TH1F*_hCorrYield;
  TFile *_fileMC;
  TFile *_fileData;
  
  public:
    HFResult(TString namefileMC, TString namefileData);
    ~HFResult() { }
    void LoadHisto();
    TH1F*GetEff(){return _hEff;};
    TH1F*GetYield(){return _hYield;};
    TH1F*GetYieldCorr(){return _hCorrYield;};  
};
