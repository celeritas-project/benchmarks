//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.cc
//---------------------------------------------------------------------------//

#include <G4Event.hh>
#include <g4root.hh>

#include "EventAction.hh"

//---------------------------------------------------------------------------//
//! Constructor and destructor
EventAction::EventAction() : G4UserEventAction() {}

EventAction::~EventAction() = default;

//---------------------------------------------------------------------------//
//! Implementation of virtual functions
void EventAction::BeginOfEventAction(const G4Event* event)
{
    // event_id_ = event->GetEventID();
}