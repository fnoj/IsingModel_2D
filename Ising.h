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

#include <RQ_OBJECT.h>
#include <TGFrame.h>
#include <TFile.h>
#include <TTimer.h>
#include <TGTab.h>
#include <TPad.h>
#include <TGStatusBar.h>
#include <TBox.h>
#include <TAxis.h>
#include <TRandom3.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TGNumberEntry.h>
#include <TGraph.h>
#include <TMath.h>
#include <TLegend.h>
#include <TGClient.h>
#include <TDialogCanvas.h>
#include <TGFileDialog.h>

class Ising{

  RQ_OBJECT("Ising");

 public:
  
  Ising(const TGWindow *p,UInt_t w,UInt_t h);
  void GraphsHidden(Bool_t Opt);
  void CBGraphsClicked();
  void CBAnimationClicked();
  void CBFMagClicked();
  void CBLimTiClicked();
  void Control();
  void Run();
  void Reset();
  void ResetAll();
  void PrintImage();
  void PrintAll();
  ~Ising();
  
 private:

  TGTransientFrame *fWCGraphs;
  TGLayoutHints *l1, *l2;
  TGMainFrame *TMF1;

  TRootEmbeddedCanvas *EC1, *ECMagn, *ECEner, *ECMagnC, *ECEnerC;
  TCanvas *c1, *CMagn, *CEnerg, *CMagnC, *CEnergC;  
  TGVerticalFrame  *VFO1, *VFO2;
  TGHorizontalFrame  *HF1;
  TGNumberEntry *NEPoints, *NEFMagnetic, *NERate, *NELimitTime, *NETemp;                   //NumberEntry Values
  TGTextButton *TGBRun,*TGBReset, *TGBPrintAll;
  TGTextButton *TGBEDat, *TGBEImage, *TGBExit;
  TGraph *GMagn;
  TGraph *GEnerg;
  TGraph *GMagnC;
  TGraph *GEnergC;
  TLegend *LEnerg, *LMagn, *LTc;
  TGGroupFrame  *GF1, *GF2, *GF3;                          //Group Parameters, Options and Results.
  TGGroupFrame *GFNode, *GFFMagn, *GFRate, *GFLimitTime, *GFTemp;
  TGCheckButton *CBFMag, *CBLimitTime, *CBGraphs, *CBAnimation;
  TGStatusBar *SB;
  
  TTimer *Count; 
  TFile *f1;
  TRandom3 *Rand;
  TGFileInfo fi;
  TGFileDialog *TGfd;

  Int_t CountState;    // 0=Run     1=Pause     2=Stop

  Bool_t ShowAnimation;

  Double_t NRandom;
  Int_t time;
  Int_t timeStable;
  Int_t NBox;         //Number Box
  Int_t Points;
  Int_t s[100][100];

  Double_t B;
  Double_t K;
  Double_t Temp;
  Double_t TempC[200];
  Double_t Tc;
  Double_t J;
  Double_t H;
  Double_t M;

  Int_t StateRej;
  Int_t StateAcc;
  Int_t iSta; // i Stable
  Int_t iTempC; // i for TempC
  Double_t Energy;
  Double_t Energy2;
  Double_t EnergyStable;
  Double_t MagnetizationStable;
  Double_t Magne;
  Double_t MEnergy;
  Bool_t WithMag;
  Bool_t WithLimitTime;

  Int_t aleator1, aleator2;
  Double_t OldEn,NewEn;
  TBox *tb[100][100]; 

  Char_t Mag[20];
  Char_t Ene[20];
  Char_t Time[20];
  Char_t TCr[20];

  void Init();
  void CreateGraphs();
  void CreateAllGraphs();
  void Frontier();
  void Calculate_Energy();
  void Calculate_EnergyB();
  void Magnetization();
  void PrintState();
  void ResetValues();
  void DrawTc();
  
  ClassDef(Ising,0);
};
