//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file StackingAction.hh
//! \brief Defines which tracks will be killed
//---------------------------------------------------------------------------//
#pragma once

#include <G4UserStackingAction.hh>
#include <G4ClassificationOfNewTrack.hh>

//---------------------------------------------------------------------------//
/*!
 * Reclassifies secondary tracks as G4ClassificationOfNewTrack::fKill. This
 * prevents secondary from being transported, but also breaks conservation
 * laws since the information of the secondary is lost.
 */
class StackingAction : public G4UserStackingAction
{
  public:
    StackingAction();
    virtual ~StackingAction();

  public:
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track) override;
    // void NewStage() override;
    // void PrepareNewEvent() override;
};