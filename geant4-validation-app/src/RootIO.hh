//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RootIO.hh
//! \brief ROOT I/O interface.
//---------------------------------------------------------------------------//
#pragma once

#include <string>
#include <memory>
#include <TFile.h>
#include <TTree.h>
#include <nlohmann/json.hpp>
#include "Event.hh"

//---------------------------------------------------------------------------//
/*!
 * ROOT I/O interface. It creates a singleton to manage data provided by
 * different Geant4 classes.
 *
 * Use \c RootIO::construct("output.root") to create the ROOT file at the
 * beginning of the simulation run. Call \c RootIO::get_instance() to get
 * access to the constructed RootIO object from any class method.
 * Write the TFile to disk and close it at the end of the simulation run, in
 * \c RunAction::EndOfRunAction(...) .
 */
class RootIO
{
  public:
    // Construct by creating singleton and creating the ROOT file
    static void construct(std::string root_filename);

    // Get singleton instance
    static RootIO* get_instance();

    // Clear utils::Event object
    void clear_event();

    // Clear utils::Track object
    void clear_track();

  public:
    // >>> TFile structure
    std::unique_ptr<TFile> tfile_;
    std::unique_ptr<TTree> ttree_event_;

    // >>> Branch objects
    utils::Event event_;
    utils::Track track_;

  private:
    RootIO(std::string root_filename);
    ~RootIO() = default;
};
