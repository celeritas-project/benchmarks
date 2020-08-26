//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.cc
//---------------------------------------------------------------------------//

#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>

#include "DetectorConstruction.hh"

//---------------------------------------------------------------------------//
//! Default constructor and destructor
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() = default;

//---------------------------------------------------------------------------//
//! Implementation of a mandatory Geant4 virtual class method
//! The geometry is implemented here
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Create all used materials
    G4NistManager* nist     = G4NistManager::Instance();
    G4Material*    material = nist->FindOrBuildMaterial("G4_Fe");

    // Define size of all objects
    double world_size = 100 * km;

    // List of geometry objects
    G4Box* world_box
        = new G4Box("world_box", world_size, world_size, world_size);

    /*!
     * World volume
     */
    G4LogicalVolume* world_logical_volume;
    world_logical_volume
        = new G4LogicalVolume(world_box, material, "world_logical_volume");

    G4VPhysicalVolume* world_physical_volume;
    world_physical_volume = new G4PVPlacement(
        0, G4ThreeVector(), world_logical_volume, "world", 0, false, 0, false);

    return world_physical_volume;
}
