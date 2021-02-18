//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SteppingAction.hh
//! \brief Collect step information.
//---------------------------------------------------------------------------//
#pragma once

#include <G4UserSteppingAction.hh>
#include <G4Step.hh>

//---------------------------------------------------------------------------//
/*!
 * Retrieve particle step data and save it to the root file.
 */
class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(bool save_step_data);
    ~SteppingAction();

    void UserSteppingAction(const G4Step* step) override;

  private:
    void fill_rootio_step_data(const G4Step* step);
    const bool save_step_data_;
};
