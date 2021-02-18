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

//---------------------------------------------------------------------------//
/*!
 * Constructor and default destructor.
 */
SteppingAction::SteppingAction(bool save_step_data)
    : G4UserSteppingAction(), save_step_data_(save_step_data)
{
}

SteppingAction::~SteppingAction() = default;

//---------------------------------------------------------------------------//
/*!
 * Fetch data at every new step and populate Event object.
 */
void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto root_io = RootIO::get_instance();
    root_io->track_.energy_dep += step->GetTotalEnergyDeposit() / MeV;

    if (save_step_data_)
    {
        fill_rootio_step_data(step);
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

    root_io->track_.steps.push_back(this_step);
}
