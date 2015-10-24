#include <iostream>

#include "TFile.h"
#include "TGeoManager.h"

#include "WindowLoop.hxx"
#include "ReadGeom.hxx"

#include "CLITools.hxx"

namespace CLIOpts {
  std::string GeomFileName;

  void SetOpts(){
      CLIArgs::AddOpt("-g", "--Geom-File", true,
      [&] (std::string const &opt) -> bool {
        std::cout << "\t--Attempting to read ROOT geometry from file: "
          << opt << std::endl;
          GeomFileName = opt;
        return true;
      }, true,
      [](){},
      "ROOT file to read geometry from.");

  }
}

int main(int argc, char const * argv[]){

  try {
    CLIOpts::SetOpts();
  } catch (std::exception const & e){
    std::cerr << "[ERROR]: " << e.what() << std::endl;
    return 1;
  }

  CLIArgs::AddArguments(argc,argv);
  if(!CLIArgs::HandleArgs()){
    CLIArgs::SayRunLike();
    return 1;
  }

  // open the geometry file
  TFile* GeomFile = new TFile(CLIOpts::GeomFileName.c_str());

  TGeoManager* geo = ReadGeomFromFile(GeomFile);
  (void)geo;
  WindowLoop();

  GeomFile->Close();
  return 0;
}
