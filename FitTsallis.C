void check(){
     
     //TF1*fBoltz = new TF1( "fBoltz", "[0]*exp(-(x-[2])/[1])", 0.3,3);          
     TF1*fTsallis = new TF1( "fTsallis", "[0]*(1+([3]-1)*(x-[2])/[1])**([3]/(1-[3]))", 0.,400);          


     TFile *file=new TFile("HEPData-ins1496050-v1-root.root");
     TH1F*h=(TH1F*)file->Get("Table 7/Hist1D_y1");

    //fBoltz->SetParameter(0,1.);
    //fBoltz->SetParameter(1,100);
    //fBoltz->SetParameter(2,0.2);

    fTsallis->SetParameter(0,5.15760e-07);
    fTsallis->SetParameter(1,1.14520e+00);
    fTsallis->SetParameter(2,5.64780e+00);
    fTsallis->SetParameter(3,1.17624e+00);

     TCanvas*c=new TCanvas("c","c",500,500);
     c->cd();
     gPad->SetLogy();
     gPad->SetLogx();
     h->Fit("fTsallis","R");
     //f->Draw("same");
     
          
}