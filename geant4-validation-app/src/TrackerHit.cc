//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackerHit.cc
//---------------------------------------------------------------------------//
#include "TrackerHit.hh"

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
