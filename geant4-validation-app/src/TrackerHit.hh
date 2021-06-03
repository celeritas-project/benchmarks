//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackerHit.hh
//! \brief Silicon Tracker sensitive detector hit
//---------------------------------------------------------------------------//
#pragma once

#include <G4VHit.hh>
#include <G4THitsCollection.hh>

//---------------------------------------------------------------------------//
/*!
 * Define interface for the Silicon Tracker Sensitive detector hits
 */
class TrackerHit : public G4VHit
{
  public:
    TrackerHit();
    ~TrackerHit();

    void add_energy_dep(const double energy);

  private:
    double energy_dep_;
};

typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;
