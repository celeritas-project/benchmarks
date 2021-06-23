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
    if (const double edep = step->GetTotalEnergyDeposit()
                            && G4VSensitiveDetector::SensitiveDetectorName
                                   == "si_tracker_sd")
    {
        const auto lv_name = step->GetPreStepPoint()
                                 ->GetPhysicalVolume()
                                 ->GetLogicalVolume()
                                 ->GetName();

        TrackerHit* hit = new TrackerHit();
        hit->add_energy_dep(edep);
        hit->track_id_     = step->GetTrack()->GetTrackID();
        hit->hit_position_ = step->GetPostStepPoint()->GetPosition();
        hit_collection_0->insert(hit);
        std::cout << "hit is processed " << edep << std::endl;

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
    std::cout << "INITIALIZE SITRACKERSD " << std::endl;
    std::cout << "SensitiveDetectorName " << SensitiveDetectorName << std::endl;

    if (G4VSensitiveDetector::SensitiveDetectorName == "si_tracker_sd")
    {
        hit_collection_0 = new TrackerHitsCollection(
            G4VSensitiveDetector::SensitiveDetectorName, collectionName[0]);

        int hit_collection_id
            = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

        hit_col_of_evt->AddHitsCollection(hit_collection_id, hit_collection_0);
    }
    else
    {
        std::cout << "I can record another SD" << std::endl;
        hit_collection_1 = new TrackerHitsCollection(
            G4VSensitiveDetector::SensitiveDetectorName, collectionName[1]);

        int hit_collection_id
            = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[1]);

        hit_col_of_evt->AddHitsCollection(hit_collection_id, hit_collection_1);
    }
}

//---------------------------------------------------------------------------//
/*!
 * Optional end of event function.
 */
void SiTrackerSD::EndOfEvent(G4HCofThisEvent* hit_col_of_evt)
{
    // TODO: Fill ROOT hit data here or in EventAction::EndOfEventAction()?
    std::cout << "end of event" << std::endl;
#if 0
    const auto n_hits = hit_collection_0->entries();

    std::cout << "hc_name   : " << hit_collection_0->GetName() << std::endl;
    std::cout << "hc_sd_name: " << hit_collection_0->GetSDname() << std::endl;
    std::cout << "hc_id     : " << hit_collection_0->GetColID() << std::endl;

    for (int i = 0; i < hit_collection_0->entries(); i++)
    {
        (*hit_collection_0)[i]->print();
    }
#endif
}
