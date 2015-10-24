#ifndef SEEN_READGEOM_HXX
#define SEEN_READGEOM_HXX

#include <string>
#include "TGeoManager.h"
#include "TFile.h"

TGeoManager* ReadGeomFromFile(TFile* GeomFile=nullptr,
  std::string const &KeyName="ND280Geometry");

#endif
