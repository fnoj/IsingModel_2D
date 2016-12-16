///////////////////////////////////////////////////////////////////////////
//                                                                       //
//  NOTICE OF COPYRIGHT                                                  //
//                                                                       //
//                Copyright (C) 2016   John Suárez                       //
//                https://github.com/fnoj/IsingModel_2D                  //
//                                                                       //
// This program is free software; you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by  //
// the Free Software Foundation; either version 2 of the License, or     //
// (at your option) any later version.                                   //
//                                                                       //
// This program is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of        //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
// GNU General Public License for more details:                          //
//                                                                       //
//          http://www.gnu.org/copyleft/gpl.html                         //
//                                                                       //
///////////////////////////////////////////////////////////////////////////


#include "Ising.h"
#include "TApplication.h"
#include "TBenchmark.h"
#include <iostream>
using namespace std;

Ising::Ising(const TGWindow *p,UInt_t w,UInt_t h){

  l1 = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY ,2,2,2,2);
  l2 = new TGLayoutHints(kLHintsCenterX | kLHintsCenterY ,4,4,4,4);
    
  TMF1 = new TGMainFrame(p,h,w);
  TMF1->SetWindowName("Ising 2D");
  TMF1->SetWMSizeHints(w,h,w,h,1,1);

  HF1 = new TGHorizontalFrame(TMF1,2,2);

  // Group Frames Left______________________________________________________
  VFO1 = new TGVerticalFrame(HF1,2,2);

  GF1 = new TGGroupFrame(VFO1,"Parameters",kVerticalFrame);

  GFNode= new TGGroupFrame(GF1,"Nodes [2-99]",kHorizontalFrame);
  NEPoints = new TGNumberEntry(GFNode,20);
  NEPoints->SetNumStyle(TGNumberFormat::kNESReal);
  NEPoints->SetNumAttr(TGNumberFormat::kNEAPositive);
  NEPoints->SetLimits(TGNumberFormat::kNELLimitMinMax,2,99);
  GFNode->AddFrame(NEPoints);

  GFTemp= new TGGroupFrame(GF1,"Temperature [Max]",kHorizontalFrame);
  NETemp = new TGNumberEntry(GFTemp,4);
  NETemp->SetNumStyle(TGNumberFormat::kNESReal);
  NETemp->SetNumAttr(TGNumberFormat::kNEAPositive);
  NETemp->SetState(true);
  GFTemp->AddFrame(NETemp);

  GFFMagn= new TGGroupFrame(GF1,"Field Magnetic",kVerticalFrame);
  NEFMagnetic = new TGNumberEntry(GFFMagn,-2);
  NEFMagnetic->SetNumStyle(TGNumberFormat::kNESReal);
  NEFMagnetic->SetLimits(TGNumberFormat::kNELLimitMinMax,-1000,1000);
  CBFMag = new TGCheckButton(GFFMagn,"Magnetic Field",1);
  CBFMag->Connect("Clicked()","Ising",this,"CBFMagClicked()");
  CBFMag->SetState(kButtonUp);
  GFFMagn->AddFrame(NEFMagnetic,new TGLayoutHints(kLHintsNormal,3,3,3,3));
  GFFMagn->AddFrame(CBFMag,new TGLayoutHints(kLHintsNormal,3,3,3,3));
  WithMag=false;

  GFRate= new TGGroupFrame(GF1,"Rate (Green) [\%]",kHorizontalFrame);
  NERate = new TGNumberEntry(GFRate,0);
  NERate->SetNumStyle(TGNumberFormat::kNESReal);
  NERate->SetLimits(TGNumberFormat::kNELLimitMinMax,0,100);
  GFRate->AddFrame(NERate);
  
  GFLimitTime= new TGGroupFrame(GF1,"LimitTime",kVerticalFrame);
  NELimitTime = new TGNumberEntry(GFLimitTime,100);
  NELimitTime->SetNumStyle(TGNumberFormat::kNESReal);
  NELimitTime->SetLimits(TGNumberFormat::kNELLimitMinMax,0,10000);
  NELimitTime->SetState(false);
  CBLimitTime = new TGCheckButton(GFLimitTime,"Limit Time ",2);
  CBLimitTime->Connect("Clicked()","Ising",this,"CBLimTiClicked()");
  CBLimitTime->SetState(kButtonUp);
  GFLimitTime->AddFrame(NELimitTime);
  GFLimitTime->AddFrame(CBLimitTime);

  //Options__________________________________________________
  GF2 = new TGGroupFrame(VFO1,"Options",kVerticalFrame);
  TGBRun = new TGTextButton(GF2,"&Start",1);
  TGBRun->Connect("Clicked()","Ising",this,"Control()");

  TGBReset = new TGTextButton(GF2,"&Reset",1);
  TGBReset->Connect("Clicked()","Ising",this,"ResetAll()");
  TGBReset->SetEnabled(false);

  TGBEImage = new TGTextButton(GF2,"P&rint",2);
  TGBEImage->Connect("Clicked()","Ising",this,"PrintImage()");
  TGBEImage->SetEnabled(false);

  TGBPrintAll = new TGTextButton(GF2,"Print A&ll",2);
  TGBPrintAll->Connect("Clicked()","Ising",this,"PrintAll()");
  TGBPrintAll->SetEnabled(false);

  TGBExit = new TGTextButton(GF2,"&Exit","gApplication->Terminate(0)");

  CBGraphs = new TGCheckButton(VFO1,"Graphs",1);
  CBGraphs->Connect("Clicked()","Ising",this,"CBGraphsClicked()");
  CBGraphs->SetState(kButtonUp);

  CBAnimation = new TGCheckButton(VFO1,"Animation",1);
  CBAnimation->Connect("Clicked()","Ising",this,"CBAnimationClicked()");
  CBAnimation->SetState(kButtonDown);
  ShowAnimation=true;

  GF1->AddFrame(GFNode,new TGLayoutHints(kLHintsNormal,3,3,3,3));
  GF1->AddFrame(GFTemp,new TGLayoutHints(kLHintsNormal,3,3,3,3));
  GF1->AddFrame(GFFMagn,new TGLayoutHints(kLHintsNormal,3,3,3,3));
  GF1->AddFrame(GFRate,new TGLayoutHints(kLHintsNormal,3,3,3,3));
  GF1->AddFrame(GFLimitTime,new TGLayoutHints(kLHintsNormal,3,3,3,3));

  GF2->AddFrame(TGBRun,l1);
  GF2->AddFrame(TGBReset,l1);
  GF2->AddFrame(TGBEImage,l1);
  GF2->AddFrame(TGBPrintAll,l1);
  GF2->AddFrame(TGBExit,l1);

  VFO1->AddFrame(GF1,l2);
  VFO1->AddFrame(GF2,l2);
  VFO1->AddFrame(CBGraphs,l2);
  VFO1->AddFrame(CBAnimation,l2);

  // Canvas Right___________________________________________________________
  VFO2 = new TGVerticalFrame(HF1,2,2,2,2);

  SB = new TGStatusBar(TMF1);
  SB->SetParts(4);

  EC1 = new TRootEmbeddedCanvas("EC1",VFO2,510,510);   
  c1 = EC1->GetCanvas();

  VFO2->AddFrame(EC1,new TGLayoutHints(kLHintsNormal,1,1,1,1));

  HF1->AddFrame(VFO1,new TGLayoutHints(kLHintsNormal,10,10,10,10));
  HF1->AddFrame(VFO2,new TGLayoutHints(kLHintsNormal,10,10,10,10));

  //WCGraphs___________________________________________________
  fWCGraphs = new TGTransientFrame(p,gClient->GetRoot());
  fWCGraphs->SetWMSizeHints(675,495,675,495,20,20);
  TGTab *Tabs = new TGTab(fWCGraphs);
  TGCompositeFrame *TabMag = Tabs->AddTab("M vs. t");
  TGCompositeFrame *TabEner = Tabs->AddTab("E vs. t");
  TGCompositeFrame *TabMagC = Tabs->AddTab("<M> vs. T");
  TGCompositeFrame *TabEnerC = Tabs->AddTab("<E> vs. T");

  ECMagn = new TRootEmbeddedCanvas("ECMagn",TabMag,650,450);
  ECEner = new TRootEmbeddedCanvas("ECEner",TabEner,650,450); 
  ECMagnC = new TRootEmbeddedCanvas("ECMagnC",TabMagC,650,450); 
  ECEnerC = new TRootEmbeddedCanvas("ECEnerC",TabEnerC,650,450); 

  CMagn = ECMagn->GetCanvas();
  CEnerg = ECEner->GetCanvas();
  CMagnC = ECMagnC->GetCanvas();
  CEnergC = ECEnerC->GetCanvas();

  TabMag->AddFrame(ECMagn, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,10,10,10,10));
  TabEner->AddFrame(ECEner, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,10,10,10,10));
  TabMagC->AddFrame(ECMagnC, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,10,10,10,10));
  TabEnerC->AddFrame(ECEnerC, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,10,10,10,10));

  LEnerg = new TLegend(0.8,0.85,1,1);
  LMagn = new TLegend(0.8,0.85,1,1);
  LTc = new TLegend(0.8,0.85,1,1);

  CEnergC->cd();  
  GEnergC = new TGraph(NBox);
  GEnergC->GetYaxis()->CenterTitle(true);
  GEnergC->GetYaxis()->SetTitleFont(12);
  GEnergC->GetYaxis()->SetLabelFont(12);
  GEnergC->GetXaxis()->CenterTitle(true);
  GEnergC->GetXaxis()->SetTitleFont(12);
  GEnergC->GetXaxis()->SetLabelFont(12);
  GEnergC->SetTitle("Energy vs. T");
  CEnergC->Update();

  CMagnC->cd();
  GMagnC = new TGraph(NBox);
  GMagnC->GetYaxis()->CenterTitle(true);
  GMagnC->GetYaxis()->SetTitleFont(12);
  GMagnC->GetYaxis()->SetLabelFont(12);
  GMagnC->GetXaxis()->CenterTitle(true);
  GMagnC->GetXaxis()->SetTitleFont(12);
  GMagnC->GetXaxis()->SetLabelFont(12);
  GMagnC->GetXaxis()->SetTitleOffset(1);
  GMagnC->SetTitle("Magnetization vs. T");
  CMagnC->Update();

  WithLimitTime=false;
  
  fWCGraphs->AddFrame(Tabs);
  fWCGraphs->MapSubwindows();
  fWCGraphs->MapWindow();

  TMF1->AddFrame(HF1);
  TMF1->AddFrame(SB,new TGLayoutHints(kLHintsExpandX,1,1,1,1));
  
  TMF1->MapSubwindows();
  TMF1->Resize(TMF1->GetDefaultSize());
  TMF1->MapWindow();

  CountState=2;
  iTempC=0;
  TempC[0]=0.1;
  Count = new TTimer(0.01);
  Count->Connect("Timeout()","Ising",this,"Run()");
}

void Ising::CreateGraphs(){
  CMagn->cd();
  GMagn = new TGraph(NBox);
  GMagn->GetYaxis()->CenterTitle(true);
  GMagn->GetYaxis()->SetTitleFont(12);
  GMagn->GetYaxis()->SetLabelFont(12);
  GMagn->GetXaxis()->CenterTitle(true);
  GMagn->GetXaxis()->SetTitleFont(12);
  GMagn->GetXaxis()->SetLabelFont(12);
  GMagn->GetXaxis()->SetTitleOffset(1);
  GMagn->SetTitle("Magnetization vs. Time");
  CMagn->Update();

  CEnerg->cd();  
  GEnerg = new TGraph(NBox);
  GEnerg->GetYaxis()->CenterTitle(true);
  GEnerg->GetYaxis()->SetTitleFont(12);
  GEnerg->GetYaxis()->SetLabelFont(12);
  GEnerg->GetXaxis()->CenterTitle(true);
  GEnerg->GetXaxis()->SetTitleFont(12);
  GEnerg->GetXaxis()->SetLabelFont(12);
  GEnerg->SetTitle("Energy vs. Time");
  CEnerg->Update();
}

void Ising::CreateAllGraphs(){
  CMagn->cd();
  GMagn = new TGraph(NBox);
  GMagn->GetYaxis()->CenterTitle(true);
  GMagn->GetYaxis()->SetTitleFont(12);
  GMagn->GetYaxis()->SetLabelFont(12);
  GMagn->GetXaxis()->CenterTitle(true);
  GMagn->GetXaxis()->SetTitleFont(12);
  GMagn->GetXaxis()->SetLabelFont(12);
  GMagn->GetXaxis()->SetTitleOffset(1);
  GMagn->SetTitle("Magnetization vs. Time");
  CMagn->Update();

  CEnerg->cd();  
  GEnerg = new TGraph(NBox);
  GEnerg->GetYaxis()->CenterTitle(true);
  GEnerg->GetYaxis()->SetTitleFont(12);
  GEnerg->GetYaxis()->SetLabelFont(12);
  GEnerg->GetXaxis()->CenterTitle(true);
  GEnerg->GetXaxis()->SetTitleFont(12);
  GEnerg->GetXaxis()->SetLabelFont(12);
  GEnerg->SetTitle("Energy vs. Time");
  CEnerg->Update();

  CEnergC->cd();  
  GEnergC = new TGraph(NBox);
  GEnergC->GetYaxis()->CenterTitle(true);
  GEnergC->GetYaxis()->SetTitleFont(12);
  GEnergC->GetYaxis()->SetLabelFont(12);
  GEnergC->GetXaxis()->CenterTitle(true);
  GEnergC->GetXaxis()->SetTitleFont(12);
  GEnergC->GetXaxis()->SetLabelFont(12);
  GEnergC->SetTitle("Energy vs. T");
  CEnergC->Update();

  CMagnC->cd();
  GMagnC = new TGraph(NBox);
  GMagnC->GetYaxis()->CenterTitle(true);
  GMagnC->GetYaxis()->SetTitleFont(12);
  GMagnC->GetYaxis()->SetLabelFont(12);
  GMagnC->GetXaxis()->CenterTitle(true);
  GMagnC->GetXaxis()->SetTitleFont(12);
  GMagnC->GetXaxis()->SetLabelFont(12);
  GMagnC->GetXaxis()->SetTitleOffset(1);
  GMagnC->SetTitle("Magnetization vs. T");
  CMagnC->Update();
}

void Ising::Control(){
  if(CountState==0){
    Count->TurnOff();
    CountState=1;                                     //State->Pause
    TGBRun->SetText("&Cont.");
    TGBReset->SetEnabled(true);
    TGBEImage->SetEnabled(true);
    NELimitTime->SetState(true);
    CBLimitTime->SetEnabled(true);
    TGBPrintAll->SetEnabled(true);
  }
  else if(CountState==2){
    CountState=0;                                     //State->Run
    TGBRun->SetText("&Pause");
    NEPoints->SetState(false);
    NERate->SetState(false);
    NETemp->SetState(false);
    CreateGraphs();
    Ising::Run();
    TGBEImage->SetEnabled(false);
    NELimitTime->SetState(false);
    CBLimitTime->SetEnabled(false);
    TGBPrintAll->SetEnabled(false);
  }
  else if(CountState==1){
    Count->TurnOn();
    CountState=0;                                     //State->Run
    NEPoints->SetState(false);
    NERate->SetState(false);
    NETemp->SetState(false);
    TGBRun->SetText("&Pause");
    TGBReset->SetEnabled(false);
    TGBEImage->SetEnabled(false);
    NELimitTime->SetState(false);
    CBLimitTime->SetEnabled(false);
    TGBPrintAll->SetEnabled(false);
  }
}

void Ising::Reset(){
  time=0;
  c1->Clear();
  c1->Update();
  NEPoints->SetState(true);
  NERate->SetState(true);
  NETemp->SetState(true);
  CEnerg->Clear();
  CEnerg->Update();
  CMagn->Clear();
  CMagn->Update();
  LEnerg->Clear();
  LMagn->Clear();
  CreateGraphs();
  Init();
  Count->Reset();
}

void Ising::ResetAll(){
  time=0;
  c1->Clear();
  c1->Update();
  NEPoints->SetState(true);
  NERate->SetState(true);
  NETemp->SetState(true);
  TGBRun->SetText("&Start");
  CEnerg->Clear();
  CEnerg->Update();
  CMagn->Clear();
  CMagn->Update();
  CEnergC->Clear();
  CEnergC->Update();
  CMagnC->Clear();
  CMagnC->Update();
  LEnerg->Clear();
  LMagn->Clear();
  CreateAllGraphs();
  Init();
  Count->Reset();
  iSta=0;
  iTempC=0;
  TempC[0]=0.1;
}

void Ising::CBFMagClicked(){
  if(WithMag==false) WithMag=true;
  else WithMag=false;
}

void Ising::CBGraphsClicked(){
  if(CBGraphs->IsOn()) GraphsHidden(true);
  else GraphsHidden(false);
}

void Ising::CBAnimationClicked(){
  if(CBAnimation->IsOn()){ShowAnimation=true;}
  else ShowAnimation=false;
  //cout<<ShowAnimation<<endl;
}

void Ising::GraphsHidden(Bool_t Opt){
  if(Opt){fWCGraphs->UnmapWindow();}
  else{fWCGraphs->MapWindow();}
}

void Ising::CBLimTiClicked(){
  if(WithLimitTime==true){
    cout<<"\n Limit Time Desactivated!"<<endl;
    WithLimitTime=false;
    NELimitTime->SetState(false);
  }
  else{
    WithLimitTime=true;
    NELimitTime->SetState(true);
  }   
}

void Ising::PrintImage(){
  const char *filetypes[] = { "PDF file",    "*.pdf",
			      "PNG file",    "*.png",
			      "EPS file",    "*.eps",
			      "All files",     "*",
			      0,               0 };
  TGFileInfo fi;
  fi.fFileTypes = filetypes;
  TGfd = new TGFileDialog(gClient->GetRoot(),TMF1, kFDSave, &fi);
  TString FileName = fi.fFilename;
  if(FileName==""){
    cout << "Print Failed!"<<endl;
  }
  else{
    switch (fi.fFileTypeIdx){
    case 0:
      FileName+=".pdf";
      break;
    case 2:
      FileName+=".png";      
      break;
    case 4:
      FileName+=".eps";
      break;
    default:
      FileName+=".eps";
      break;
    }
    c1->SaveAs(FileName,"");
  }
}

void Ising::PrintAll(){
  Char_t CanMain[20],CanMagC[20],CanE[20],CanMag[20],CanEC[20],RootFile[20];
  Int_t Point = NEPoints->GetNumber();
  Int_t Rate = NERate->GetNumber();

  sprintf(CanMain,"Ising_2D_State_%d_%d_%4.2f.eps",Point,Rate,TempC[iTempC]);
  c1->Write(CanMain);
  c1->Print(CanMain,"eps");

  sprintf(CanEC,"Ising_2D_EvsT_%d_%d_%4.2f.eps",Point,Rate,TempC[iTempC]);
  CEnergC->Write(CanEC);
  CEnergC->Print(CanEC,"eps");
  
  sprintf(CanE,"Ising_2D_Evst_%d_%d_%4.2f.eps",Point,Rate,TempC[iTempC]);
  CEnerg->Write(CanE);
  CEnerg->Print(CanE,"eps");
  
  sprintf(CanMagC,"Ising_2D_MagvsT_%d_%d_%4.2f.eps",Point,Rate,TempC[iTempC]);
  CMagnC->Write(CanMagC);
  CMagnC->Print(CanMagC,"eps");
  
  sprintf(CanMag,"Ising_2D_Magvst_%d_%d_%4.2f.eps",Point,Rate,TempC[iTempC]);
  CMagn->Write(CanMag);
  CMagn->Print(CanMag,"eps");
  
  sprintf(RootFile,"Ising_2D_ROOTFILE_%d_%d_%4.2f.root",Point,Rate,TempC[iTempC]);
  f1 = new TFile(RootFile,"recreate");
}

// MAIN /////////////////////////////////////////////////////////////////////////////

void Ising::Init(){
  Rand = new TRandom3();
  Rand->SetSeed();
  Double_t Rate = NERate->GetNumber()/100;
  if(Rate!=0){
    cout<<"Rate: "<<Rate<<"\%"<<endl;
    for (Int_t j=0;j<NBox;j++){ 
      for (Int_t n=0;n<NBox*Rate;n++){ 
  	tb[j][n] = new TBox(n,j,n+1,j+1); 
	tb[j][n]->SetFillColor(8);//Verde
	tb[j][n]->Draw();  
	s[j][n]=1;
      }  
      for (Int_t n=NBox*Rate;n<NBox;n++){ 
   	tb[j][n] = new TBox(n,j,n+1,j+1); 
   	tb[j][n]->SetFillColor(2);//Rojo
 	tb[j][n]->Draw();  
 	s[j][n]=-1;
      }  
    }
  }
  if(Rate==0){
    for (Int_t i=0;i<NBox;i++){ 
      for (Int_t j=0;j<NBox;j++){ 
	tb[i][j] = new TBox(j,NBox-i,j+1,NBox-i-1); 
	s[i][j]=Rand->Uniform(-2,2);
	while(s[i][j]==0){
	  s[i][j]=Rand->Uniform(-2,2);
	}
	if(s[i][j]==1){
	  tb[i][j]->SetFillColor(8);//Verde
	}  
	else if(s[i][j]==-1){
	  tb[i][j]->SetFillColor(2);//Rojo
	}
	tb[i][j]->Draw();
      }
    }
  }
  time=0;
  StateRej=0;
  StateAcc=0;
  B=0;
  OldEn=0;
  NewEn=0;
  H=0;
  M=0;
  K=1;//1.38064852*TMath::Power(10,-23);
  J=1;
  iSta=0;
  Tc=2*J/(K*TMath::ASinH(1));
  EnergyStable=0;
  MagnetizationStable=0;
}

void Ising::PrintState(){
  for (Int_t i=0;i<NBox;i++){ 
    for (Int_t j=0;j<NBox;j++){ 
      cout<<"\t"<<s[i][j];
    }
    cout<<"\n";
  }
}

void Ising::Frontier(){
  NBox = NEPoints->GetNumber();
  for(Int_t i=0;i<NBox;i++){
    s[NBox][i]=s[0][i];
    s[i][NBox]=s[i][0];
  }
}

void Ising::Calculate_Energy(){
  H=0;
  for(Int_t i=0;i<NBox;i++){
    for(Int_t j=0;j<NBox;j++){
      H=H+s[i][j]*(s[i+1][j]+s[i][j+1]);
    }
  }
  H=-J*H;
}

void Ising::Calculate_EnergyB(){
  H=0;
  for(Int_t i=0;i<=NBox;i++){
    for(Int_t j=0;j<=NBox;j++){
      H+=s[i][j]*(s[i+1][j]+s[i][j+1]-B);
    }
  }
  H=-J*H;
}

void Ising::Magnetization(){
  M=0;
  for(Int_t i=0;i<NBox;i++){
    for(Int_t j=0;j<NBox;j++){
      M+=s[i][j];
    }
  }
  M=M/(NBox*NBox);
}

void Ising::Run(){
  c1->cd();
  Rand = new TRandom3();  
  Rand->SetSeed();
  //cout<<"Tc: "<<2*J/(K*TMath::ASinH(1))<<endl;
  NBox = NEPoints->GetNumber();
  timeStable=TMath::Power(NBox,3);   //Take the time of stability how N^3
  B = NEFMagnetic->GetNumber();
  Temp = NETemp->GetNumber();
  Points = NEPoints->GetNumber();
  c1->Range(0,0,NBox,NBox);
  
  Count->TurnOn();
  
  if(time==0){
    //cout<<"Started!"<<endl;
    Ising::Init();
    //Ising::PrintState();
    c1->Update();
  }
  if(time==NELimitTime->GetNumber() && WithLimitTime==true){
    //cout<<"Stoped!"<<endl;
    Count->Stop();
    TGBRun->SetText("&Cont.");    
  }
  
  Ising::Frontier();
  
  if(WithMag==true){
    Ising::Calculate_EnergyB();
  }
  if(WithMag==false){
    Ising::Calculate_Energy();
  }
  OldEn=H; 
  
  aleator1 = Rand->Uniform(0,NBox); 
  aleator2 = Rand->Uniform(0,NBox); 

  //cout<<"["<<aleator1<<","<<aleator2<<"]"<<endl;

  s[aleator1][aleator2]*=(-1); 
  //Ising::PrintState();
  Ising::Frontier();
   
  if(WithMag==true){
    Ising::Calculate_EnergyB();
  }
  if(WithMag==false){
    Ising::Calculate_Energy();
  }
  NewEn=H;
  
  //cout<<"OldEn: "<<OldEn<<"\t NewEn: "<<NewEn<<"\t D_En: "<<NewEn-OldEn<<endl;
  Int_t DEn=NewEn-OldEn;
  Double_t p=Rand->Uniform(0,1);
  //Metropolis Algoritm
  if( ((DEn>0) && (TMath::Exp((-DEn)/(K*TempC[iTempC]))>p))){
    //cout<<"Acepted"<<endl;
    if(s[aleator1][aleator2]==-1){
      if(ShowAnimation==true){
	tb[aleator1][aleator2]->SetFillColor(2);
	tb[aleator1][aleator2]->Draw();
      }
    }
    else if(s[aleator1][aleator2]==1){
      if(ShowAnimation==true){
	tb[aleator1][aleator2]->SetFillColor(8);
	tb[aleator1][aleator2]->Draw();    
      }
    }
    else{
      if(ShowAnimation==true){
	tb[aleator1][aleator2]->SetFillColor(0);
	tb[aleator1][aleator2]->Draw();
      }
    }
    StateAcc++;
  }
  else if(DEn<=0){ //Follow Draw() to view evolution in mesh !!!!!!!!
    //cout<<"Acepted!"<<endl;
    if(s[aleator1][aleator2]==-1){
      if(ShowAnimation==true){
	tb[aleator1][aleator2]->SetFillColor(2);
	tb[aleator1][aleator2]->Draw();
      }
    }
    else if(s[aleator1][aleator2]==1){
      if(ShowAnimation==true){
	tb[aleator1][aleator2]->SetFillColor(8);
	tb[aleator1][aleator2]->Draw();    
      }
    }
    else{
      if(ShowAnimation==true){
	tb[aleator1][aleator2]->SetFillColor(0);
	tb[aleator1][aleator2]->Draw();
      }
    }
    StateAcc++;
  }
  else{
    StateRej++;
    NewEn=OldEn;
    s[aleator1][aleator2]*=(-1);
  }
  Magnetization();
  //cout<<"M: "<<M<<endl;
  Energy=NewEn;
  Energy2=NewEn*NewEn;
  Magne=M;
  c1->Update();
  //cout<<"A: "<< StateAcc<<"\t R: "<<StateRej<<endl;
  
  sprintf(Ene,"E= %4.2f",Energy);
  SB->SetText(Ene,0);
  //cout<<"Magnetization: "<<Magne<<endl;
  sprintf(Mag,"M= %4.2f",Magne);
  SB->SetText(Mag,1);

  sprintf(TCr,"T= %4.2f",TempC[iTempC]);
  SB->SetText(TCr,3);

  // if(time==2*timeStable){
  //   cout<<"Stable "<<2*timeStable<<endl;
  // }
  
  if(time>=3*timeStable){
    EnergyStable+=Energy;
    MagnetizationStable+=Magne;
    //cout<<"iSta: "<<iSta<<endl;
    if(iSta==(10000)){
      //cout<<"iSta: "<<iSta<<endl;
      //cout<<"iStaF: "<<iSta<<endl;
      EnergyStable=EnergyStable/(iSta);
      MagnetizationStable=MagnetizationStable/(iSta);
      //cout<<"iTemp"<<iTempC<<endl;
      //cout<<"Tc: "<<TempC[iTempC]<<endl;
      
      CMagnC->cd(); 
      CMagnC->SetGrid();   
      GMagnC->SetPoint(iTempC,TempC[iTempC],MagnetizationStable); 
      GMagnC->Draw("A*");
      GMagnC->GetYaxis()->SetTitle("Magnetization Mean");
      GMagnC->GetXaxis()->SetTitle("T");
      CMagnC->Update();

      CEnergC->cd(); 
      CEnergC->SetGrid();   
      GEnergC->SetPoint(iTempC,TempC[iTempC],EnergyStable); 
      GEnergC->Draw("A*");
      GEnergC->GetYaxis()->SetTitle("Energy Mean");
      GEnergC->GetXaxis()->SetTitle("T");
      CEnergC->Update();
      
      CEnerg->cd();
      GEnerg->Draw("AL*");
      LEnerg->Draw();
      
      CMagn->cd(); 
      GMagn->Draw("AL*");
      LMagn->Draw();
      iSta=0;
      iTempC++;
      TempC[iTempC]=TempC[iTempC-1]+0.05;
      Reset();
      Run();
    }
    iSta++;
    if(TempC[iTempC]>=Temp){
      cout<<"Stop!"<<endl;
      Count->Stop();
    }
  }

  CEnerg->cd(); 
  CEnerg->SetGrid();   
  GEnerg->SetPoint(time,time,Energy); 
  GEnerg->GetYaxis()->SetTitle("Energy");
  GEnerg->GetXaxis()->SetTitle("Time");
  LEnerg->Clear();
  LEnerg->AddEntry(GEnerg,Ene,"");
  LEnerg->AddEntry(GEnerg,TCr,"");
  if(ShowAnimation==true){
    GEnerg->Draw("AL*");
    LEnerg->Draw();
    CEnerg->Update();
  }
  
  CMagn->cd(); 
  CMagn->SetGrid();   
  GMagn->SetPoint(time,time,Magne); 
  GMagn->GetYaxis()->SetTitle("Magnetization");
  GMagn->GetXaxis()->SetTitle("Time");
  LMagn->Clear();
  LMagn->AddEntry(GMagn,Mag,"");
  LMagn->AddEntry(GMagn,TCr,"");
  if(ShowAnimation==true){
    GMagn->Draw("AL*");
    LMagn->Draw();
    CMagn->Update();
  }
  sprintf(Time,"Time= %4d",time);
  SB->SetText(Time,2);
  
  c1->Update(); 
  time++;
  //cout<<time<<endl;
  H=0;
  NewEn=0;
  OldEn=0;
  Energy=0;
  Magne=0;
}

Ising::~Ising(){
  TMF1->Cleanup();
  delete TMF1;
}
