//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file StackingAction.hh
//! \brief Classify tracks.
//---------------------------------------------------------------------------//
#pragma once

#include <G4UserStackingAction.hh>
#include <G4ClassificationOfNewTrack.hh>

//---------------------------------------------------------------------------//
/*!
 * Reclassify secondaries as G4ClassificationOfNewTrack::fKill, preventing
 * them from being transported.
 */
class StackingAction : public G4UserStackingAction
{
  public:
    StackingAction();
    virtual ~StackingAction();

  private:
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track) override;
};
