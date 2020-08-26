//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file StackingAction.cc
//---------------------------------------------------------------------------//

#include "StackingAction.hh"
#include <G4Track.hh>

//---------------------------------------------------------------------------//
//! Constructor and destructor
StackingAction::StackingAction() : G4UserStackingAction()
{
}

StackingAction::~StackingAction() = default;

//---------------------------------------------------------------------------//
//! Implementation of virtual functions
/*!
 * If called by ActionInitialization, this class kills all secondaries 
 * before they are transported
 */
G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track *track)
{
    if (track->GetParentID() != 0)
    {

        return G4ClassificationOfNewTrack::fKill;
    }

    return G4ClassificationOfNewTrack::fUrgent;
}