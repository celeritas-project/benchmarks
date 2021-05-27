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
#include <G4Tubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4GDMLParser.hh>

//---------------------------------------------------------------------------//
/*!
 * Construct with programmatic geometry.
 */
DetectorConstruction::DetectorConstruction()
{
    phys_vol_world_.reset(this->create_simple_cms());
}

//---------------------------------------------------------------------------//
/*!
 * Construct with gdml geometry.
 */
DetectorConstruction::DetectorConstruction(std::string gdml_input_file)
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
    return phys_vol_world_.release();
}

//---------------------------------------------------------------------------//
/*!
 * Export GDML file representing the programmatic geometry.
 */
void DetectorConstruction::export_gdml(std::string gdml_filename)
{
    G4GDMLParser parser;
    parser.Write(gdml_filename, this->create_simple_cms());
}

//---------------------------------------------------------------------------//
// PRIVATE
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Programmatic geometry definition: Al cube.
 */
G4VPhysicalVolume* DetectorConstruction::create_Al_cube()
{
    // Create all used materials
    G4NistManager* nist     = G4NistManager::Instance();
    G4Material*    material = nist->FindOrBuildMaterial("G4_Al");

    // Define size of all objects
    const double world_size = 100 * km;

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

//---------------------------------------------------------------------------//
/*!
 * Programmatic geometry definition: Single material CMS mock up.
 *
 * This is set of single-element concentric cylinders that act as a spherical
 * cow in a vacuum version of CMS.
 */
G4VPhysicalVolume* DetectorConstruction::create_simple_cms()
{
    // Create all used materials
    G4NistManager* nist   = G4NistManager::Instance();
    G4Material*    vacuum = nist->FindOrBuildMaterial("G4_Galactic"); // H only
    G4Material*    Si     = nist->FindOrBuildMaterial("G4_Si");
    G4Material*    Pb     = nist->FindOrBuildMaterial("G4_Pb");
    G4Material*    C      = nist->FindOrBuildMaterial("G4_C");
    G4Material*    Ti     = nist->FindOrBuildMaterial("G4_Ti");
    G4Material*    Fe     = nist->FindOrBuildMaterial("G4_Fe");

    // Size of World volume
    const double world_size = 20 * m;
    // Half length of all concentric cylinders (z-axis)
    const double half_length = 7 * m;
    // Small distance between cylinder edges to avoid volume overlap
    const double delta = 1e-10;

    // List of solids
    G4Box* world_box
        = new G4Box("world_box", world_size / 2, world_size / 2, world_size);

    G4Tubs* si_tracker = new G4Tubs("silicon_tracker",
                                    30 * cm,          // Inner radius
                                    125 * cm - delta, // Outer radius
                                    half_length,      // Half-length z
                                    0 * deg,          // Start angle
                                    360 * deg);       // Spanning angle

    G4Tubs* em_calorimeter = new G4Tubs("crystal_em_calorimeter",
                                        125 * cm,
                                        175 * cm - delta,
                                        half_length,
                                        0 * deg,
                                        360 * deg);

    G4Tubs* had_calorimeter = new G4Tubs("hadron_calorimeter",
                                         175 * cm,
                                         275 * cm - delta,
                                         half_length,
                                         0 * deg,
                                         360 * deg);

    G4Tubs* sc_solenoid = new G4Tubs("superconducting_solenoid",
                                     275 * cm,
                                     375 * cm - delta,
                                     half_length,
                                     0 * deg,
                                     360 * deg);

    G4Tubs* iron_muon_chambers = new G4Tubs("iron_muon_chambers",
                                            375 * cm,
                                            700 * cm,
                                            half_length,
                                            0 * deg,
                                            360 * deg);

    // List of logical volumes
    const auto world_lv = new G4LogicalVolume(world_box, vacuum, "world_lv");
    const auto si_tracker_lv
        = new G4LogicalVolume(si_tracker, Si, "si_tracker_lv");
    const auto em_calorimeter_lv
        = new G4LogicalVolume(em_calorimeter, Pb, "em_calorimeter_lv");
    const auto had_calorimeter_lv
        = new G4LogicalVolume(had_calorimeter, C, "had_calorimeter_lv");
    const auto sc_solenoid_lv
        = new G4LogicalVolume(sc_solenoid, Ti, "sc_solenoid_lv");
    const auto iron_muon_chambers_lv
        = new G4LogicalVolume(iron_muon_chambers, Fe, "iron_muon_chambers_lv");

    // List of physical volumes
    const auto world_pv = new G4PVPlacement(0,               // Rotation matrix
                                            G4ThreeVector(), // Position
                                            world_lv,        // Current LV
                                            "world_pv",      // Name
                                            nullptr,         // Mother LV
                                            false,           // Bool operation
                                            0,               // Copy number
                                            false);          // Overlap check

    new G4PVPlacement(0,
                      G4ThreeVector(),
                      si_tracker_lv,
                      "si_tracker_pv",
                      world_lv,
                      false,
                      0,
                      false);

    new G4PVPlacement(0,
                      G4ThreeVector(),
                      em_calorimeter_lv,
                      "em_calorimeter_pv",
                      world_lv,
                      false,
                      0,
                      false);

    new G4PVPlacement(0,
                      G4ThreeVector(),
                      had_calorimeter_lv,
                      "had_calorimeter_pv",
                      world_lv,
                      false,
                      0,
                      false);

    new G4PVPlacement(0,
                      G4ThreeVector(),
                      sc_solenoid_lv,
                      "sc_solenoid_pv",
                      world_lv,
                      false,
                      0,
                      false);

    new G4PVPlacement(0,
                      G4ThreeVector(),
                      iron_muon_chambers_lv,
                      "iron_muon_chambers_"
                      "pv",
                      world_lv,
                      false,
                      0,
                      false);

    return world_pv;
}
