//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.cc
//---------------------------------------------------------------------------//
#include "DetectorConstruction.hh"

#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4GDMLParser.hh>

//---------------------------------------------------------------------------//
/*!
 * Construct with programmatic geometry.
 */
DetectorConstruction::DetectorConstruction() : use_gdml_(false) {}

//---------------------------------------------------------------------------//
/*!
 * Construct with gdml geometry.
 */
DetectorConstruction::DetectorConstruction(std::string gdml_input_file)
    : use_gdml_(true)
{
    G4GDMLParser   gdml_parser;
    constexpr bool validate_gdml_schema = false;
    gdml_parser.Read(gdml_input_file, validate_gdml_schema);
    phys_vol_world_.reset(gdml_parser.GetWorldVolume());
}

//---------------------------------------------------------------------------//
/*!
 * Default destructor.
 */
DetectorConstruction::~DetectorConstruction() = default;

//---------------------------------------------------------------------------//
/*!
 * Construct geometry.
 */
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    if (!use_gdml_)
    {
        phys_vol_world_.reset(CreateGeometry());
    }

    return phys_vol_world_.release();
}

//---------------------------------------------------------------------------//
/*!
 * Programmatic geometry definition.
 */
G4VPhysicalVolume* DetectorConstruction::CreateGeometry()
{
    // Create all used materials
    G4NistManager* nist     = G4NistManager::Instance();
    G4Material*    material = nist->FindOrBuildMaterial("G4_Al");

    // Define size of all objects
    double world_size = 100 * km;

    // List of geometry objects
    G4Box* world_box
        = new G4Box("world_box", world_size, world_size, world_size);

    // Create volume
    G4LogicalVolume* world_logical_volume;
    world_logical_volume
        = new G4LogicalVolume(world_box, material, "world_logical_volume");

    G4VPhysicalVolume* world_physical_volume;
    world_physical_volume = new G4PVPlacement(
        0, G4ThreeVector(), world_logical_volume, "world", 0, false, 0, false);

    return world_physical_volume;
}
