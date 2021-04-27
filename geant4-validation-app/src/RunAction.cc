//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.cc
//---------------------------------------------------------------------------//
#include "RunAction.hh"

#include <G4RunManager.hh>
#include "RootIO.hh"

//---------------------------------------------------------------------------//
/*!
 * Default constructor and destructor.
 */
RunAction::RunAction() : G4UserRunAction() {}

//---------------------------------------------------------------------------//
RunAction::~RunAction() = default;

//---------------------------------------------------------------------------//
/*!
 * Create ROOT file and its ntuple structure.
 */
void RunAction::BeginOfRunAction(const G4Run*)
{
    RootIO::construct();
}

//---------------------------------------------------------------------------//
/*!
 * Write ROOT file to disk.
 */
void RunAction::EndOfRunAction(const G4Run*)
{
    auto root_io = RootIO::get_instance();
    root_io->tfile_->Write();
    root_io->tfile_->Close();
}
