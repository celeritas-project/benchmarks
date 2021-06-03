//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SiTrackerSD.hh
//! \brief Silicon Tracker sensitive detector
//---------------------------------------------------------------------------//
#pragma once

#include <G4VSensitiveDetector.hh>
#include "TrackerHit.hh"

//---------------------------------------------------------------------------//
/*!
 * Define interface for the Silicon Tracker Sensitive detector
 */
class SiTrackerSD : public G4VSensitiveDetector
{
  public:
    SiTrackerSD(G4String name, G4String collection_name);
    ~SiTrackerSD();

    G4bool ProcessHits(G4Step* step, G4TouchableHistory* touch_hist) override;

    // Maybe these won't be used
    void Initialize(G4HCofThisEvent* hit_col_of_evt) override;
    void EndOfEvent(G4HCofThisEvent* hit_col_of_evt) override;

  private:
    TrackerHitsCollection* hit_collection_;
};
