//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SteppingAction.hh
//! \brief Collects step information
//---------------------------------------------------------------------------//
#pragma once

#include <G4UserSteppingAction.hh>
#include <G4Step.hh>

#include "DetectorConstruction.hh"
#include "EventAction.hh"

//---------------------------------------------------------------------------//
//! Retrieves particle step data for the root file
class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(EventAction* event_action);
    ~SteppingAction();

    void UserSteppingAction(const G4Step* step) override;

    EventAction* event_action_;
};
