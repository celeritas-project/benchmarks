//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorHit.cc
//---------------------------------------------------------------------------//
#include "DetectorHit.hh"

#include <iostream>

//---------------------------------------------------------------------------//
/*!
 * Construct empty.
 */
DetectorHit::DetectorHit() : G4VHit() {}

//---------------------------------------------------------------------------//
/*!
 * Construct with HitData.
 */
DetectorHit::DetectorHit(HitData& hit) : G4VHit(), hit_(hit) {}

//---------------------------------------------------------------------------//
/*!
 * Default destructor.
 */
DetectorHit::~DetectorHit() = default;

//---------------------------------------------------------------------------//
/*!
 * Construct with HitData.
 */
void DetectorHit::print()
{
    std::cout << "Track id  : " << hit_.track_id << std::endl;
    std::cout << "Parent id : " << hit_.parent_id << std::endl;
    std::cout << "Energy dep: " << hit_.energy_deposition << std::endl;
    std::cout << "Position  : " << hit_.position.x << ", " << hit_.position.y
              << ", " << hit_.position.z << std::endl;
    std::cout << "Direction : " << hit_.direction.x << ", " << hit_.direction.y
              << ", " << hit_.direction.z << std::endl;
}
