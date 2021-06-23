//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackerHit.cc
//---------------------------------------------------------------------------//
#include "TrackerHit.hh"
#include <iostream>

//---------------------------------------------------------------------------//
/*!
 * Default constructor and destructor.
 */
TrackerHit::TrackerHit() : G4VHit() {}

TrackerHit::~TrackerHit() = default;

//---------------------------------------------------------------------------//
/*!
 * Increment the energy deposition.
 */
void TrackerHit::add_energy_dep(const double energy)
{
    energy_dep_ += energy;
}

//---------------------------------------------------------------------------//
/*!
 * Print hit information.
 */
void TrackerHit::print()
{
    std::cout << "trackid/position: " << track_id_ << " / " << hit_position_
              << std::endl;
}