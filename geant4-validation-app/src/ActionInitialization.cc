//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
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
/*!
 * Constructor and default destructor.
 */
ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

//---------------------------------------------------------------------------//
ActionInitialization::~ActionInitialization() = default;

//---------------------------------------------------------------------------//
/*!
 * Construct and invoke all SetUserAction() kind of classes.
 */
void ActionInitialization::Build() const
{
    SetUserAction(new RunAction());              // Manage root file
    SetUserAction(new PrimaryGeneratorAction()); // Set particle gun
    SetUserAction(new EventAction());            // Set event rules
    SetUserAction(new TrackingAction());         // Save track info
    SetUserAction(new SteppingAction());         // Save step data
    // SetUserAction(new StackingAction());      // Kill secondaries
}
