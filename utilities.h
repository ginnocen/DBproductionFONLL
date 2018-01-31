  const int samples=3; 
  const int nElossType=5;
  const int nresults=2;
  TString namefiles[samples]={"output_pp_d0meson_5TeV_y1.root","output_pp_Bplusmeson_5TeV_y1.root","HEPData-ins1496050-v1-root.root"};
  TString nameHistoInput[samples]={"hpt","hpt","Table 7/Hist1D_y1"};

  double eloss_value[nElossType]={0,1,2,0.20,0.40};  
  double eloss_method[nElossType]={0,0,0,1,1};  

  TString nameeloss[nElossType]={"binary","eloss_flat1GeV", "eloss_flat2GeV","20percent", "40percent"};  
  TString nameParticle[samples]={"D-meson","B-meson","Charged-particle"};  
  
  int colours[nElossType]={1,2,3,4,kOrange};
  int markerstyle[nElossType]={21,21,21,22,22};
  int widthline[nElossType]={2,2,2,2,2};
    
  double nbins[samples]={2000,2000,2000};
  double lowerrangex[samples]={1.,1.,1.};
  double lowerrangexrun[samples]={1,1,1};
  double upperrangex[samples]={100,100,100};
  double lowerrangey[samples]={4,4,4};
  double upperrangey[samples]={1e11,1e11,1e11};
  double lowerrangeyRAA[samples]={0.,0.,0.};
  double upperrangeyRAA[samples]={2,2,2};
  TString string_xaxis[samples]={"p_{T}(GeV)", "p_{T}(GeV)","p_{T}(GeV)"};
