//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.cc
//---------------------------------------------------------------------------//
#include "EventAction.hh"

#include <G4Event.hh>
#include "RootIO.hh"

//---------------------------------------------------------------------------//
/*
 * Constructor and destructor.
 */
EventAction::EventAction() : G4UserEventAction() {}
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
void EventAction::EndOfEventAction(const G4Event*)
{
    RootIO::get_instance()->ttree_ptr_->Fill();
}
