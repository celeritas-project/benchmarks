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

    void   Initialize(G4HCofThisEvent* hit_col_of_evt) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* touch_hist) override;
    void   EndOfEvent(G4HCofThisEvent* hit_col_of_evt) override;

  private:
    TrackerHitsCollection*     hit_collection_0;
    CalorimeterHitsCollection* hit_collection_1;
    static const int           si_tracker_sd_id     = 0;
    static const int           em_calorimeter_sd_id = 0;
};
