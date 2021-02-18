//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RootIO.cc
//---------------------------------------------------------------------------//
#include "RootIO.hh"

#include <memory>

//---------------------------------------------------------------------------//
/*!
 * Create a static instance.
 */
static RootIO* rootio_static_instance = nullptr;

//---------------------------------------------------------------------------//
/*!
 * Assign constructor to the static RootIO*.
 */
void RootIO::construct(std::string root_filename)
{
    rootio_static_instance = new RootIO(root_filename);
}

//---------------------------------------------------------------------------//
/*!
 * Construct and create new TFile.
 */
RootIO::RootIO(std::string root_filename)
{
    tfile_ptr_ = new TFile(root_filename.c_str(), "recreate");
    ttree_ptr_ = new TTree("event", "event");
    ttree_ptr_->Branch("event", &event_);
}

//---------------------------------------------------------------------------//
/*!
 * Close ROOT file at destruction, if not previously done so.
 */
RootIO::~RootIO()
{
    if (tfile_ptr_->IsOpen())
    {
        tfile_ptr_->Close();
    }
}

//---------------------------------------------------------------------------//
/*!
 * Get static RootIO instance. \c construct() *MUST* be called before this.
 */
RootIO* RootIO::get_instance()
{
    return rootio_static_instance;
}

//---------------------------------------------------------------------------//
/*!
 * Clear event_ struct for storing new data.
 */
void RootIO::clear_event()
{
    event_ = utils::Event();
}

//---------------------------------------------------------------------------//
/*!
 * Clear track_ struct for storing new data.
 */
void RootIO::clear_track()
{
    track_ = utils::Track();
}