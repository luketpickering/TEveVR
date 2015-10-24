#include <iostream>
#include <algorithm>
#include <string>

#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TFile.h"
#include "TKey.h"


//Adapted from code found in NEUTgeom
void RecursiveExclude(std::vector< std::pair<bool, std::string> > const  &VolIdents,
  TGeoVolume *vol, bool exclude, std::string indent=""){

  if(!vol || !std::string(vol->GetName()).length()){
    std::cerr << "[ERROR]: vol(name:"
      << (vol?std::string(vol->GetName()).length():-1)
      << ") wasn't valid(loc:" << vol << ")." << std::endl;
    return;
  }
  std::string volname = vol->GetName();

  std::cout << indent << "Looking in " << vol->GetName()
    << "(NElements: " << vol->GetMaterial()->GetNelements() << ")" << std::endl;
  for( auto const & vi : VolIdents ){
    if(volname == vi.second){
      exclude = !vi.first;
      break;
    }
  }

  if(exclude){
    static TGeoMaterial *MatVacuum = ((TGeoMaterial *)0);
    static TGeoMedium *Vacuum = ((TGeoMedium *)0);
    static bool first = true;
    if(first){
      MatVacuum = new TGeoMaterial("Vacuum", 0.0, 0.0, 0.0);
      Vacuum = new TGeoMedium("Vacuum", 1, MatVacuum);
    }
    vol->SetMedium(Vacuum);
    std::cout << indent << "-Excluding: " << vol->GetTitle() << std::endl;
  }

  for(size_t i = 0; i < vol->GetNdaughters(); ++i){
    if(vol->GetNode(i)){
      RecursiveExclude(VolIdents,vol->GetNode(i)->GetVolume(), exclude, indent+" ");
    }
  }
}

TGeoManager* ReadGeomFromFile(TFile* GeomFile,
  std::string const &KeyName){

  std::vector< std::pair<bool, std::string> > genVol =
    {std::make_pair(true,"Basket"),
     std::make_pair(false,"OA"),
     std::make_pair(true,"P0D"),
     std::make_pair(true,"BrlECal"),
     std::make_pair(false,"ingrid"),
   };

  // Get TGeoManager key name
  TIter iter(GeomFile->GetListOfKeys());
  bool foundGeometry;
  std::string GeomKeyName;
  std::for_each(iter.Begin(),TIter::End(),[&] (TObject *keyptr) mutable {
    if(foundGeometry){return;}

    TKey const *Key = dynamic_cast<TKey const *>(keyptr);
    if(!Key){
      std::cout << "[ERROR]: Failed to cast TKey." << std::endl;
      return;
    }
    TString keyName = Key->GetName();
    std::string className = Key->GetClassName();
    std::cout << "Found key: " << keyName << "(" << className << ")" << std::endl;
    if(className == "TGeoManager" && keyName.BeginsWith("ND280Geometry")){
      std::cout << "[INFO]: Found the geometry manager." << std::endl;
      GeomKeyName = keyName.Data();
      foundGeometry = true;
    }
  });

  if (!foundGeometry) {
    std::cerr << "[ERROR]: Could not find TGeoManager object in file: "
      << GeomFile->GetName() << std::endl;
    return nullptr;
  }

  TGeoManager *GeoM = static_cast<TGeoManager*>(GeomFile->Get(GeomKeyName.c_str()));
  TGeoVolume *TopVol = GeoM->GetTopVolume();
  if(!TopVol || !std::string(TopVol->GetName()).length()){
    std::cerr << "[ERROR]: TopVol wasn't valid." << std::endl;
    return nullptr;
  }

  std::string TVName = TopVol->GetName();
  RecursiveExclude(genVol,TopVol,false);


  // Output information to screen
  std::cout << std::endl << "Geometry file " << GeomFile->GetName()  << std::endl;
  std::cout << "Geometry hash " << GeomKeyName  << std::endl;
  // std::cout << "Volumes       " << genVol    << std::endl << std::endl;
  return GeoM;
}
