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

#ifndef __CINT__

#ifndef ROOT_TApplication
#include "TApplication.h"
#endif
#include "Ising.h"
#endif

#ifdef __CINT__
#include "Ising.cxx"
#endif

int Application(){
  Ising *App = new Ising(gClient->GetRoot(),750,648);
  return 0;
}

#ifndef __CINT__
int main(int argc, char** argv)
{
  TApplication Ising2DApp("Ising2DApp", &argc, argv);
  int retVal = Application();
  Ising2DApp.Run();
  return retVal;
}
#endif
