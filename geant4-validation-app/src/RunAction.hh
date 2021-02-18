//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.hh
//! \brief Manage the simulation run.
//---------------------------------------------------------------------------//
#pragma once

#include <G4Run.hh>
#include <G4UserRunAction.hh>

//---------------------------------------------------------------------------//
/*!
 * Manage simulation run. Create and write the output ROOT file to disk.
 */
class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run* run) override;
    void EndOfRunAction(const G4Run*) override;
    void set_output_filename(const std::string root_filename);

  private:
    void CreateRootNtuples();

  private:
    std::string  geant_root_output_filename_ = "out.root";
};
