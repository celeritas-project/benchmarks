//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file read_events.C
//! \brief Example macro for looping over event tree.
//---------------------------------------------------------------------------//
#include "../src/Event.hh"

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>

//---------------------------------------------------------------------------//
/*!
 * Example macro that loops over event tree and prints up to 5 events.
 * Only a couple secondaries are printed to avoid cluttering.
 * Usage:
 * \code
 * $ root
 * $ root[0] .x read_events.C("path/to/g4_output_post.root")
 * \endcode
 */
void read_events(const std::string file_name)
{
    TFile* input      = new TFile(file_name.c_str(), "read");
    TTree* event_tree = (TTree*)input->Get("event");

    utils::Event* event = nullptr;
    event_tree->SetBranchAddress("event", &event);

    int number_of_events = event_tree->GetEntries();
    if (number_of_events > 5)
    {
        number_of_events = 5;
    }

    for (int i = 0; i < number_of_events; i++)
    {
        event_tree->GetEntry(i);

        std::cout << ">>> Event " << event->id << std::endl;
        std::cout << "-----------" << std::endl;

        // Primary
        std::cout << " Primary" << std::endl;

        utils::Vector3 prim_pos = event->primary.vertex_position;
        utils::Vector3 prim_dir = event->primary.vertex_direction;

        std::cout << "  | pdg encoding           : " << event->primary.pdg
                  << std::endl;
        std::cout << "  | vertex position [cm]   : " << prim_pos.x << ", "
                  << prim_pos.y << ", " << prim_pos.z << std::endl;
        std::cout << "  | vertex direction       : " << prim_dir.x << ", "
                  << prim_dir.y << ", " << prim_dir.z << std::endl;
        std::cout
            << "  | vertex energy [MeV]    : " << event->primary.vertex_energy
            << std::endl;
        std::cout << "  | track length [cm]      : " << event->primary.length
                  << std::endl;
        std::cout
            << "  | energy deposition [MeV]: " << event->primary.vertex_energy
            << std::endl;
        std::cout << "  | steps (" << event->primary.steps.size() << ")"
                  << std::endl;

        if (event->primary.steps.size() > 0)
        {
            utils::Step prim_step = event->primary.steps.at(0);
            std::cout << "  |  | process          : " << prim_step.process
                      << std::endl;
            std::cout
                << "  |  | energy [MeV]     : " << prim_step.kinetic_energy
                << std::endl;
            std::cout << "  |  | energy loss [MeV]: " << prim_step.energy_loss
                      << std::endl;
            std::cout << "  |  | position [cm]    : " << prim_step.position.x
                      << ", " << prim_step.position.y << ", "
                      << prim_step.position.z << std::endl;
            std::cout << "  |  | direction        : " << prim_step.direction.x
                      << ", " << prim_step.direction.y << ", "
                      << prim_step.direction.z << std::endl;
            if (event->primary.steps.size() > 1)
            {
                std::cout << "  |  | --" << std::endl;
                std::cout << "  |  :" << std::endl;
                std::cout << "  |  :" << std::endl;
            }
        }

        // Secondaries
        std::cout << "  | Secondaries (" << event->secondaries.size() << ")"
                  << std::endl;

        int  number_of_secondaries = event->secondaries.size();
        bool ellipsis              = false;

        if (number_of_secondaries > 2)
        {
            number_of_secondaries = 2;
            ellipsis              = true;
        }

        for (int j = 0; j < number_of_secondaries; j++)
        {
            utils::Track   secondary = event->secondaries.at(j);
            utils::Vector3 pos       = secondary.vertex_position;
            utils::Vector3 dir       = secondary.vertex_direction;

            std::cout << "     | pdg encoding           : " << secondary.pdg
                      << std::endl;
            std::cout << "     | vertex position [cm]   : " << pos.x << ", "
                      << pos.y << ", " << pos.z << std::endl;
            std::cout << "     | vertex direction       : " << dir.x << ", "
                      << dir.y << ", " << dir.z << std::endl;
            std::cout << "     | vertex energy [MeV]    : "
                      << secondary.vertex_energy << std::endl;
            std::cout << "     | track length [cm]      : " << secondary.length
                      << std::endl;
            std::cout << "     | energy deposition [MeV]: "
                      << secondary.vertex_energy << std::endl;
            std::cout << "     | steps (" << secondary.steps.size() << ")"
                      << std::endl;

            if (secondary.steps.size() > 0)
            {
                utils::Step step = secondary.steps.at(0);

                std::cout << "     |  | process          : " << step.process
                          << std::endl;
                std::cout
                    << "     |  | energy [MeV]     : " << step.kinetic_energy
                    << std::endl;
                std::cout << "     |  | energy loss [MeV]: "
                          << secondary.steps.at(0).energy_loss << std::endl;
                std::cout << "     |  | position [cm]    : " << step.position.x
                          << ", " << step.position.y << ", " << step.position.z
                          << std::endl;
                std::cout
                    << "     |  | direction        : " << step.direction.x
                    << ", " << step.direction.y << ", " << step.direction.z
                    << std::endl;

                if (secondary.steps.size() > 1)
                {
                    std::cout << "     |  | --" << std::endl;
                    std::cout << "     |  :" << std::endl;
                    std::cout << "     |  :" << std::endl;
                }
            }
            if (number_of_secondaries > 1)
            {
                std::cout << "     | --" << std::endl;
            }
            if (j == number_of_secondaries - 1)
            {
                if (ellipsis)
                {
                    std::cout << "     :" << std::endl;
                    std::cout << "     :" << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }
    input->Close();
}