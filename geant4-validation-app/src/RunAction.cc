//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.cc
//---------------------------------------------------------------------------//
#include "RunAction.hh"

#include <G4RunManager.hh>
#include <G4HCtable.hh>
#include "RootIO.hh"
#include "JsonReader.hh"

//---------------------------------------------------------------------------//
/*!
 * Default constructor and destructor.
 */
RunAction::RunAction() : G4UserRunAction()
{
    const auto json = JsonReader::get_instance()->json();

    G4RunManager::GetRunManager()->SetVerboseLevel(
        json.at("verbosity").at("RunAction").get<int>());

    if (const int n_evts = json.at("verbosity").at("PrintProgress").get<int>())
    {
        G4RunManager::GetRunManager()->SetPrintProgress(n_evts);
    }
}

//---------------------------------------------------------------------------//
RunAction::~RunAction() = default;

//---------------------------------------------------------------------------//
/*!
 * Create ROOT file and its ntuple structure.
 */
void RunAction::BeginOfRunAction(const G4Run*)
{
    RootIO::construct_io();
}

//---------------------------------------------------------------------------//
/*!
 * Write data to the ROOT file and write file to disk.
 */
void RunAction::EndOfRunAction(const G4Run*)
{
    auto root_io = RootIO::get_instance();
    root_io->ttree_event_->Write();

    this->fill_cumulative();

    root_io->tdir_hists_->cd();
    for (int i = 0; i < 3; i++)
    {
        root_io->gr_dedx_[i]->Write();
    }

    root_io->tfile_->Close();
}

//---------------------------------------------------------------------------//
/*!
 * Populate cumulative data that depends only on track information before
 * writing ROOT file to disk.
 */
void RunAction::fill_cumulative()
{
    auto root_io = RootIO::get_instance();

    utils::Event* event = nullptr;
    root_io->ttree_event_->SetBranchAddress("event", &event);

    for (int i = 0; i < root_io->ttree_event_->GetEntries(); i++)
    {
        root_io->ttree_event_->GetEntry(i);

        for (unsigned long j = 0; j < event->secondaries.size(); j++)
        {
            const auto& secondary = event->secondaries.at(j);
            const auto& vtx       = secondary.vertex_position;

            for (unsigned long k = 0; k < root_io->gr_bins_.size(); k++)
            {
                const auto& bin = root_io->gr_bins_.at(k);

                for (int l = 0; l < 3; l++)
                {
                    if (vtx[l] >= bin.x_min && vtx[l] < bin.x_max)
                    {
                        // Add secondary kinetic energy as energy deposition
                        const auto& energy = secondary.vertex_energy;
                        double      x, y;
                        root_io->gr_dedx_[l]->GetPoint(k, x, y);
                        root_io->gr_dedx_[l]->SetPoint(k, x, y + energy);
                    }
                }
            }
        }
    }
}
