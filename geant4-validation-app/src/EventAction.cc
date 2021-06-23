//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.cc
//---------------------------------------------------------------------------//
#include "EventAction.hh"

#include <G4Event.hh>
#include <G4EventManager.hh>
#include <G4SDManager.hh>
#include "RootIO.hh"
#include "JsonReader.hh"

//---------------------------------------------------------------------------//
/*
 * Constructor and default destructor.
 */
EventAction::EventAction() : G4UserEventAction()
{
    const auto json = JsonReader::get_instance()->json();
    G4EventManager::GetEventManager()->SetVerboseLevel(
        json.at("verbosity").at("EventAction").get<int>());
}

//---------------------------------------------------------------------------//
EventAction::~EventAction() = default;

//---------------------------------------------------------------------------//
/*
 * Begin of event actions.
 */
void EventAction::BeginOfEventAction(const G4Event* event)
{
    auto root_io = RootIO::get_instance();
    root_io->clear_event();
    root_io->event_.id = event->GetEventID();
}

//---------------------------------------------------------------------------//
/*
 * End of event actions.
 */
void EventAction::EndOfEventAction(const G4Event* event)
{
    RootIO::get_instance()->ttree_event_->Fill();

    // const int si_col_id
    //    = G4SDManager::GetSDMpointer()->GetHCtable()->GetCollectionID(
    //        "si_collection");
    // const auto hits = event->GetHCofThisEvent()->GetHC(si_col_id);
    //
    //// Get deposited energy from hit collection
    // hits->PrintAllHits();
}
