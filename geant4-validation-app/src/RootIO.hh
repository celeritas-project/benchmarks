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
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>
#include <TGraph.h>

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
    static void construct();

    // Get singleton instance
    static RootIO* get_instance();

    // Clear event_
    void clear_event();

    // Clear track_
    void clear_track();

  public:
    // TFile structure
    std::unique_ptr<TFile>      tfile_;
    std::unique_ptr<TTree>      ttree_event_;
    std::unique_ptr<TDirectory> tdir_hists_;

    // Objects in TFile
    utils::Event            event_;
    utils::Track            track_;
    std::unique_ptr<TGraph> gr_dedx_[3];
    std::vector<utils::Bin> gr_bins_;

  private:
    RootIO();
    ~RootIO() = default;
};
