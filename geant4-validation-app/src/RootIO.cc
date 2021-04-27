//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RootIO.cc
//---------------------------------------------------------------------------//
#include "RootIO.hh"

#include <memory>
#include <iostream>

#include "JsonReader.hh"

//---------------------------------------------------------------------------//
/*!
 * Singleton declaration.
 */
static RootIO* rootio_singleton = nullptr;

//---------------------------------------------------------------------------//
// PUBLIC
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Constructor singleton.
 */
void RootIO::construct()
{
    if (!rootio_singleton)
    {
        rootio_singleton = new RootIO();
    }
    else
    {
        std::cout << "ROOT I/O already constructed. Nothing to do.\n";
        return;
    }
}

//---------------------------------------------------------------------------//
/*!
 * Get static RootIO instance. \c construct() *MUST* be called before this.
 */
RootIO* RootIO::get_instance()
{
    return rootio_singleton;
}

//---------------------------------------------------------------------------//
/*!
 * Clear event_ struct.
 */
void RootIO::clear_event()
{
    event_ = utils::Event();
}

//---------------------------------------------------------------------------//
/*!
 * Clear track_ struct.
 */
void RootIO::clear_track()
{
    track_ = utils::Track();
}

//---------------------------------------------------------------------------//
// PRIVATE
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Construct new TFile with given root filename.
 */
RootIO::RootIO()
{
    const auto  json          = JsonReader::get_instance()->json();
    std::string root_filename = json.at("root_output").get<std::string>();

    tfile_.reset(TFile::Open(root_filename.c_str(), "recreate"));
    ttree_event_ = std::make_unique<TTree>("event", "event");
    ttree_event_->Branch("event", &event_);
}
