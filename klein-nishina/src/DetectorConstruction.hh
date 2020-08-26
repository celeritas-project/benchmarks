//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.hh
//! \brief Creates the detector geometry
//---------------------------------------------------------------------------//
#pragma once

#include <G4VUserDetectorConstruction.hh>
#include <G4GDMLParser.hh>

//---------------------------------------------------------------------------//
//! Implementation of a mandatory Geant4 class
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();

    //! Geometry is coded here
    G4VPhysicalVolume* Construct() override;
};
