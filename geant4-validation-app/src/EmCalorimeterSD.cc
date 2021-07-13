//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EmCalorimeterSD.cc
//---------------------------------------------------------------------------//
#include "EmCalorimeterSD.hh"

#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>

//---------------------------------------------------------------------------//
/*!
 * Constructor and default destructor.
 */
EmCalorimeterSD::EmCalorimeterSD(G4String name, G4String collection_name)
    : G4VSensitiveDetector(name)
{
    G4VSensitiveDetector::collectionName.insert(collection_name);
}

//---------------------------------------------------------------------------//
EmCalorimeterSD::~EmCalorimeterSD() = default;

//---------------------------------------------------------------------------//
/*!
 * Optional initialize function.
 */
void EmCalorimeterSD::Initialize(G4HCofThisEvent* hit_col_of_evt)
{
    hit_collection_ = new HitsCollection(
        G4VSensitiveDetector::SensitiveDetectorName, collectionName[0]);

    int hit_collection_id
        = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hit_col_of_evt->AddHitsCollection(hit_collection_id, hit_collection_);
}

//---------------------------------------------------------------------------//
/*!
 * Mandatory function called at each step.
 */
G4bool
EmCalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory* touch_hist)
{
    if (const double edep = step->GetTotalEnergyDeposit())
    {
        G4ThreeVector  pos = step->GetPostStepPoint()->GetPosition();
        G4ThreeVector  dir = step->GetPostStepPoint()->GetMomentumDirection();
        utils::HitData hit_data;
        hit_data.energy_deposition = edep;
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
 * Optional end of event function.
 */
void EmCalorimeterSD::EndOfEvent(G4HCofThisEvent* hit_col_of_evt)
{
    // TODO: Fill ROOT hit data here or in EventAction::EndOfEventAction()?
    const auto hit_vector = hit_collection_->GetVector();

    int i = 0;
    for (const auto hit : *hit_vector)
    {
        i++;
    }
}
