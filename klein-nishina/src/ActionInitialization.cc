//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file ActionInitialization.cc
//---------------------------------------------------------------------------//

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "StackingAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"

//---------------------------------------------------------------------------//
//! Implementation of a mandatory Geant4 virtual class method
//! Construct and invoke all SetUserAction() kind of classes
ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}
ActionInitialization::~ActionInitialization() = default;

void ActionInitialization::Build() const
{
    EventAction*    event_action    = new EventAction();
    SteppingAction* stepping_action = new SteppingAction(event_action);

    SetUserAction(new RunAction());              //! Create root file
    SetUserAction(stepping_action);              //! Fetch step data
    SetUserAction(new PrimaryGeneratorAction()); //! Configure particle gun
    SetUserAction(event_action);                 //! Set event rules
    // SetUserAction(new StackingAction());      //! Kill secondaries
    // SetUserAction(new TrackingAction());      //! Set tracking rules (empty)
}
