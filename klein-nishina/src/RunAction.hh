//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.hh
//! \brief Defines the beginning and end of the simulation run
//---------------------------------------------------------------------------//
#pragma once

#include <G4Run.hh>
#include <G4UserRunAction.hh>

#include "PrimaryGeneratorAction.hh"

class G4Run;

//---------------------------------------------------------------------------//
// Create and write the output root file to disk
class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run* run) override;
    void EndOfRunAction(const G4Run*) override;

  protected:
    void CreateRootNtuples();
};