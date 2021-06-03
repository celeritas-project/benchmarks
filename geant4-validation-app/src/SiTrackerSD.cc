//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SiTrackerSD.cc
//---------------------------------------------------------------------------//
#include "SiTrackerSD.hh"

#include <G4SDManager.hh>

//---------------------------------------------------------------------------//
/*!
 * Constructor and default destructor.
 */
SiTrackerSD::SiTrackerSD(G4String name, G4String collection_name)
    : G4VSensitiveDetector(name)
{
    G4VSensitiveDetector::collectionName.insert(collection_name);
}

//---------------------------------------------------------------------------//
SiTrackerSD::~SiTrackerSD() = default;

//---------------------------------------------------------------------------//
/*!
 * Mandatory function called at each step.
 */
G4bool SiTrackerSD::ProcessHits(G4Step* step, G4TouchableHistory* touch_hist)
{
    if (const double edep = step->GetTotalEnergyDeposit())
    {
        const auto lv_name = step->GetPreStepPoint()
                                 ->GetPhysicalVolume()
                                 ->GetLogicalVolume()
                                 ->GetName();

        TrackerHit* hit = new TrackerHit();
        hit->add_energy_dep(edep);
        hit_collection_->insert(hit);
        //std::cout << "hit is processed " << edep << std::endl;

        return true;
    }
    return false;
}

//---------------------------------------------------------------------------//
/*!
 * Optional initialize function.
 */
void SiTrackerSD::Initialize(G4HCofThisEvent* hit_col_of_evt)
{
    hit_collection_ = new TrackerHitsCollection(
        G4VSensitiveDetector::SensitiveDetectorName, collectionName[0]);

    int hit_collection_id
        = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hit_col_of_evt->AddHitsCollection(hit_collection_id, hit_collection_);
}

//---------------------------------------------------------------------------//
/*!
 * Optional end of event function.
 */
void SiTrackerSD::EndOfEvent(G4HCofThisEvent* hit_col_of_evt)
{
    // TODO: Fill ROOT hit data here or in EventAction::EndOfEventAction()?
    std::cout << "end of event" << std::endl;
}
