//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file StackingAction.cc
//---------------------------------------------------------------------------//
#include "StackingAction.hh"

#include <G4Track.hh>

//---------------------------------------------------------------------------//
/*!
 * Default constructor and destructor.
 */
StackingAction::StackingAction() : G4UserStackingAction() {}

StackingAction::~StackingAction() = default;

//---------------------------------------------------------------------------//
/*!
 * Kill all secondaries before they are transported.
 */
G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* track)
{
    if (track->GetParentID() != 0)
        return G4ClassificationOfNewTrack::fKill;

    return G4ClassificationOfNewTrack::fUrgent;
}
