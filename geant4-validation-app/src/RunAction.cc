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
RunAction::~RunAction() = default;

//---------------------------------------------------------------------------//
/*!
 * Set ROOT output filename.
 */
void RunAction::set_output_filename(const std::string root_filename)
{
    geant_root_output_filename_ = root_filename;
}

//---------------------------------------------------------------------------//
/*!
 * Create ROOT file and its ntuple structure.
 */
void RunAction::BeginOfRunAction(const G4Run*)
{
    RootIO::construct(geant_root_output_filename_);
}

//---------------------------------------------------------------------------//
/*!
 * Write ROOT file to disk.
 */
void RunAction::EndOfRunAction(const G4Run*)
{
    auto root_io = RootIO::get_instance();
    root_io->tfile_ptr_->Write();
    root_io->tfile_ptr_->Close();
}
