//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file StackingAction.hh
//! \brief Controls tracking information
//---------------------------------------------------------------------------//
#pragma once

#include <G4UserStackingAction.hh>
#include <G4ClassificationOfNewTrack.hh>

//---------------------------------------------------------------------------//
/*!
 * Some help
 */
class StackingAction : public G4UserStackingAction
{
public:
  StackingAction();
  virtual ~StackingAction();

protected:
  G4StackManager *stackManager;

public:
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track) override;
  //void NewStage() override;
  //void PrepareNewEvent() override;
};