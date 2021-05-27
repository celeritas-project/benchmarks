//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SteppingAction.cc
//---------------------------------------------------------------------------//
#include "SteppingAction.hh"

#include <G4VProcess.hh>
#include <G4SystemOfUnits.hh>
#include "RootIO.hh"
#include "JsonReader.hh"

//---------------------------------------------------------------------------//
/*!
 * Constructor and default destructor.
 */
SteppingAction::SteppingAction() : G4UserSteppingAction()
{
    // Step info
    const auto json = JsonReader::get_instance()->json();
    save_step_data_ = json.at("step_info").get<bool>();
}

//---------------------------------------------------------------------------//
SteppingAction::~SteppingAction() = default;

//---------------------------------------------------------------------------//
/*!
 * Fetch data at every new step and populate Event object.
 */
void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto root_io = RootIO::get_instance();
    root_io->track_.energy_dep += step->GetTotalEnergyDeposit() / MeV;

    this->fill_cumulative(step);

    if (save_step_data_)
    {
        this->fill_rootio_step_data(step);
    }
}

//---------------------------------------------------------------------------//
/*!
 * Populate step information in RootIO::track_.
 */
void SteppingAction::fill_rootio_step_data(const G4Step* step)
{
    auto        root_io = RootIO::get_instance();
    utils::Step this_step;

    this_step.process
        = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    this_step.kinetic_energy = step->GetTrack()->GetKineticEnergy() / MeV;
    this_step.energy_loss    = step->GetTotalEnergyDeposit() / MeV;
    this_step.global_time    = step->GetPreStepPoint()->GetGlobalTime() / s;
    G4ThreeVector pos        = step->GetTrack()->GetPosition() / cm;
    G4ThreeVector dir        = step->GetPreStepPoint()->GetMomentumDirection();
    this_step.position       = {pos.x(), pos.y(), pos.z()};
    this_step.direction      = {dir.x(), dir.y(), dir.z()};

    root_io->track_.steps.push_back(std::move(this_step));
}

//---------------------------------------------------------------------------//
/*!
 * Populate cumulative histograms.
 */
void SteppingAction::fill_cumulative(const G4Step* step)
{
    auto                root_io = RootIO::get_instance();
    const G4ThreeVector pos     = step->GetTrack()->GetPosition() / cm;

    // Loop over bins
    for (unsigned long i = 0; i < root_io->gr_bins_.size(); i++)
    {
        const auto& bin = root_io->gr_bins_.at(i);

        // Loop over coordinates
        for (int j = 0; j < 3; j++)
        {
            if (pos[j] >= bin.x_min && pos[j] < bin.x_max)
            {
                // Add to graph energy deposited in the material
                const double dedx_i = step->GetTotalEnergyDeposit() / MeV;
                double       x, y;
                root_io->gr_dedx_[j]->GetPoint(i, x, y);
                root_io->gr_dedx_[j]->SetPoint(i, x, y + dedx_i);
            }
        }
    }
}
