//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RootIO.cc
//---------------------------------------------------------------------------//
#include "RootIO.hh"

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
    const auto  json  = JsonReader::get_instance()->json();
    int         nbins = json.at("cumulative").at("dedx")[0].get<int>();
    double      min   = json.at("cumulative").at("dedx")[1].get<double>();
    double      max   = json.at("cumulative").at("dedx")[2].get<double>();
    std::string root_filename = json.at("root_output").get<std::string>();

    tfile_.reset(TFile::Open(root_filename.c_str(), "recreate"));
    ttree_event_ = std::make_unique<TTree>("event", "event");
    ttree_event_->Branch("event", &event_);
    tdir_hists_.reset(tfile_->mkdir("cumulative"));

    // Initialize graphs to allow storing data from other class methods
    for (int j = 0; j < 3; j++)
    {
        gr_dedx_[j].reset(new TGraph());
        std::string title = "dedx_" + std::to_string(j);
        gr_dedx_[j]->SetTitle(title.c_str());
        gr_dedx_[j]->SetName(title.c_str());
    }

    const double bin_width      = (max - min) / nbins;
    const double bin_half_width = bin_width / 2;

    for (int i = 0; i < nbins; i++)
    {
        utils::Bin bin;
        bin.x_min = min + i * bin_width;
        bin.x_max = min + i * bin_width + bin_width;
        gr_bins_.push_back(std::move(bin));

        double bin_center = min + i * bin_width + bin_half_width;
        for (int j = 0; j < 3; j++)
        {
            gr_dedx_[j]->SetPoint(i, bin_center, 0);
        }
    }
}
