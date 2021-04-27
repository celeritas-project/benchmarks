//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.hh
//! \brief Create or load the detector geometry.
//---------------------------------------------------------------------------//
#pragma once

#include <string>
#include <G4VUserDetectorConstruction.hh>

//---------------------------------------------------------------------------//
/*!
 * Construct detector geometry either programmatically or via a gdml input.
 */
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    // Construct with programmatic geometry
    DetectorConstruction();
    // Construct with GDML input
    DetectorConstruction(std::string gdml_input_file);
    // Default destructor
    ~DetectorConstruction();

    // Construct geometry
    G4VPhysicalVolume* Construct() override;

    // Export GDML file representing the programmatic geometry
    void export_gdml(std::string gdml_filename);

  private:
    // Define programmatic geometry: Al cube
    G4VPhysicalVolume* CreateGeometry();
    // Define programmatic geometry: single-material CMS mock up
    G4VPhysicalVolume* CreateCMSGeometry();

  private:
    bool                               use_gdml_;
    std::unique_ptr<G4VPhysicalVolume> phys_vol_world_;
};
