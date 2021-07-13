//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SiTrackerSD.cc
//---------------------------------------------------------------------------//
#include "SiTrackerSD.hh"

#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>

//---------------------------------------------------------------------------//
/*!
 * Constructor with sensitive detector and hit collection names.
 */
SiTrackerSD::SiTrackerSD(G4String sd_name, G4String hc_name)
    : G4VSensitiveDetector(sd_name), hit_collection_(nullptr)
{
    // collectionName is a vector<string> and insert() is just a push_back()
    G4VSensitiveDetector::collectionName.insert(hc_name);
}

//---------------------------------------------------------------------------//
SiTrackerSD::~SiTrackerSD() = default;

//---------------------------------------------------------------------------//
/*!
 * Called at the beginning of each event.
 */
void SiTrackerSD::Initialize(G4HCofThisEvent* hit_col_of_evt)
{
    hit_collection_
        = new HitsCollection(G4VSensitiveDetector::SensitiveDetectorName,
                             G4VSensitiveDetector::collectionName.front());

    int hit_collection_id = G4SDManager::GetSDMpointer()->GetCollectionID(
        collectionName.front());

    hit_col_of_evt->AddHitsCollection(hit_collection_id, hit_collection_);
}

//---------------------------------------------------------------------------//
/*!
 * Called at each step.
 */
G4bool SiTrackerSD::ProcessHits(G4Step* step, G4TouchableHistory* touch_hist)
{
    if (const double edep = step->GetTotalEnergyDeposit())
    {
        G4ThreeVector  pos = step->GetPostStepPoint()->GetPosition() * cm;
        G4ThreeVector  dir = step->GetPostStepPoint()->GetMomentumDirection();
        utils::HitData hit_data;
        hit_data.energy_deposition = edep * MeV;
        hit_data.track_id          = step->GetTrack()->GetTrackID();
        hit_data.parent_id         = step->GetTrack()->GetParentID();
        hit_data.position          = {pos.x(), pos.y(), pos.z()};
        hit_data.direction         = {dir.x(), dir.y(), dir.z()};

        DetectorHit* hit = new DetectorHit(hit_data);
        hit_collection_->insert(hit);

        return true;
    }

    return false;
}

//---------------------------------------------------------------------------//
/*!
 * Optional function called at the end of event.
 */
void SiTrackerSD::EndOfEvent(G4HCofThisEvent* hit_col_of_evt)
{
    // TODO: Fill ROOT hit data here or in EventAction::EndOfEventAction()?
    const auto hit_vector = hit_collection_->GetVector();

    int i = 0;
    for (const auto hit : *hit_vector)
    {
        i++;
    }
}
