//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.cc
//---------------------------------------------------------------------------//
#include "TrackingAction.hh"

#include <G4Track.hh>
#include <G4SystemOfUnits.hh>
#include "RootIO.hh"

//---------------------------------------------------------------------------//
/*!
 * Constructor and destructor.
 */
TrackingAction::TrackingAction() : G4UserTrackingAction() {}

TrackingAction::~TrackingAction() = default;

//---------------------------------------------------------------------------//
/*!
 *  Pre-track simulation actions.
 */
void TrackingAction::PreUserTrackingAction(const G4Track*)
{
    RootIO::get_instance()->clear_track();
}

//---------------------------------------------------------------------------//
/*!
 *  Post-track simulation actions.
 */
void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
    auto root_io = RootIO::get_instance();

    root_io->track_.pdg = track->GetParticleDefinition()->GetPDGEncoding();
    root_io->track_.id  = track->GetTrackID();
    root_io->track_.parent_id          = track->GetParentID();
    root_io->track_.length             = track->GetTrackLength() / cm;
    root_io->track_.vertex_energy      = track->GetVertexKineticEnergy() / MeV;
    root_io->track_.vertex_global_time = track->GetGlobalTime() / s;
    G4ThreeVector pos                  = track->GetVertexPosition() / cm;
    G4ThreeVector dir                  = track->GetVertexMomentumDirection();
    root_io->track_.vertex_position    = {pos.x(), pos.y(), pos.z()};
    root_io->track_.vertex_direction   = {dir.x(), dir.y(), dir.z()};

    if (track->GetParentID() == 0)
    {
        // Fill primary information
        root_io->event_.primary = root_io->track_;
    }
    else
    {
        // Fill secondary information
        root_io->event_.secondaries.push_back(std::move(root_io->track_));
    }
}
