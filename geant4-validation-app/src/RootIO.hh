//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RootIO.hh
//! \brief ROOT output interface.
//---------------------------------------------------------------------------//
#pragma once

#include <string>
#include <TFile.h>
#include <TTree.h>
#include <nlohmann/json.hpp>
#include "Event.hh"

//---------------------------------------------------------------------------//
/*!
 * ROOT I/O interface. It creates a static instance available to all Geant4
 * classes.
 * Use \c RootIO::construct("output.root") to create the ROOT file at the
 * beginning of the simulation run. Call \c RootIO::get_instance() to get
 * access to the constructed RootIO object from any class method.
 * At the end of the run, write the TFile to disk and close it. 
 */
class RootIO
{
  public:
    ~RootIO();

    static void    construct(std::string root_filename);
    static RootIO* get_instance();
    void           clear_event();
    void           clear_track();

  public:
    utils::Event event_;
    utils::Track track_;
    TFile*       tfile_ptr_;
    TTree*       ttree_ptr_;

  private:
    RootIO(std::string root_filename);
};
