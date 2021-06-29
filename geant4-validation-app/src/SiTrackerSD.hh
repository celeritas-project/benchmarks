//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SiTrackerSD.hh
//! \brief Silicon Tracker sensitive detector
//---------------------------------------------------------------------------//
#pragma once

#include <map>
#include <G4VSensitiveDetector.hh>
#include "DetectorHit.hh"

//---------------------------------------------------------------------------//
/*!
 * Define interface for the Silicon Tracker Sensitive detector.
 */
class SiTrackerSD : public G4VSensitiveDetector
{
  public:
    SiTrackerSD(G4String name, G4String collection_name);
    ~SiTrackerSD();

    // Optional. Called at the beginning of each event
    void Initialize(G4HCofThisEvent* hit_col_of_evt) override;

    // Mandatory. Called at each step
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* touch_hist) override;

    // Optional. Called at the end of every event
    void EndOfEvent(G4HCofThisEvent* hit_col_of_evt) override;

  private:
    HitsCollection* hit_collection_;
};
