//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.cc
//---------------------------------------------------------------------------//

#include <G4Track.hh>
#include <G4StackManager.hh>
#include <G4TrackingManager.hh>

#include "TrackingAction.hh"

//---------------------------------------------------------------------------//
//! Constructor and destructor
TrackingAction::TrackingAction() : G4UserTrackingAction()
{
}

TrackingAction::~TrackingAction() = default;

//---------------------------------------------------------------------------//
//! Implementation of virtual functions
void TrackingAction::PreUserTrackingAction(const G4Track *track)
{
}
void TrackingAction::PostUserTrackingAction(const G4Track *track)
{
}