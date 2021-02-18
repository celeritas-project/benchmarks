//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file basic_plots.C
//! \brief Example macro for looping over events and drawing plots.
//---------------------------------------------------------------------------//
#include "../src/Event.hh"

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TH1F.h>
#include <TCanvas.h>

//---------------------------------------------------------------------------//
/*!
 * Example macro that loops over event tree and produce a few plots.
 * Usage:
 * \code
 * $ root
 * $ root[0] .x basic_plots.C("path/to/g4_output_post.root")
 * \endcode
 */
void basic_plots(const std::string file_name)
{
    // Open file and event ttree
    TFile* input      = new TFile(file_name.c_str(), "read");
    TTree* event_tree = (TTree*)input->Get("event");

    utils::Event* event = nullptr;
    event_tree->SetBranchAddress("event", &event);

    // Create histograms
    TH1F* h_n_secondaries = new TH1F("number of secondaries", "`", 100, 0, 100);
    TH1F* h_n_steps_prim
        = new TH1F("number of steps per primary", "", 100, 0, 100);
    TH1F* h_n_steps_sec
        = new TH1F("number of steps per secondary", "", 50, 0, 100);
    TH1F* h_secondaries_energy = new TH1F("secondaries energy", "", 44, 0, 11);
    TH1F* h_vertex_x = new TH1F("secondaries vertex x", "", 60, -15, 15);
    TH1F* h_vertex_y = new TH1F("secondaries vertex y", "", 60, -15, 15);
    TH1F* h_vertex_z = new TH1F("secondaries vertex z", "", 90, -15, 30);

    // Loop over data and fill histograms
    for (int i = 0; i < event_tree->GetEntries(); i++)
    {
        event_tree->GetEntry(i);
        h_n_secondaries->Fill(event->secondaries.size());

        if (event->primary.steps.size())
            h_n_steps_prim->Fill(event->primary.steps.size());

        for (int j = 0; j < event->secondaries.size(); j++)
        {
            auto secondary = event->secondaries.at(j);

            h_secondaries_energy->Fill(secondary.vertex_energy);
            h_vertex_x->Fill(secondary.vertex_position.x);
            h_vertex_y->Fill(secondary.vertex_position.y);
            h_vertex_z->Fill(secondary.vertex_position.z);

            if (secondary.steps.size())
                h_n_steps_sec->Fill(secondary.steps.size());
        }
    }

    // Create canvas and divide it into a 3x2 grid
    TCanvas* canvas = new TCanvas("canvas", "canvas", 1200, 600);
    canvas->Divide(3, 2);

    // Draw each plot in one slot of the canvas' grid
    canvas->cd(1);
    gPad->SetLogy();
    h_n_steps_prim->Draw();
    h_n_steps_prim->GetXaxis()->SetTitle("Number of steps (primary)");
    h_n_steps_prim->GetXaxis()->CenterTitle();

    canvas->cd(2);
    gPad->SetLogy();
    h_n_steps_sec->Draw();
    h_n_steps_sec->GetXaxis()->SetTitle("Number of steps (secondary)");
    h_n_steps_sec->GetXaxis()->CenterTitle();

    canvas->cd(3);
    gPad->SetLogy();
    h_secondaries_energy->Draw();
    h_secondaries_energy->GetXaxis()->SetTitle("Secondary energy [MeV]");
    h_secondaries_energy->GetXaxis()->CenterTitle();

    canvas->cd(4);
    h_vertex_x->Draw();
    h_vertex_x->GetXaxis()->SetTitle("Secondary vertex x [cm]");
    h_vertex_x->GetXaxis()->CenterTitle();
    canvas->cd(5);
    h_vertex_y->Draw();
    h_vertex_y->GetXaxis()->SetTitle("Secondary vertex y [cm]");
    h_vertex_y->GetXaxis()->CenterTitle();
    canvas->cd(6);
    h_vertex_z->Draw();
    h_vertex_z->GetXaxis()->SetTitle("Secondary vertex z [cm]");
    h_vertex_z->GetXaxis()->CenterTitle();
}