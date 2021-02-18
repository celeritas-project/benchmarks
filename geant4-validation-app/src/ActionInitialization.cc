//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file ActionInitialization.cc
//---------------------------------------------------------------------------//
#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "StackingAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"

//---------------------------------------------------------------------------//
/*!
 * Constructor and destructor.
 */
ActionInitialization::ActionInitialization(nlohmann::json json_input)
    : G4VUserActionInitialization(), json_input_(json_input)
{
}

ActionInitialization::~ActionInitialization() = default;

//---------------------------------------------------------------------------//
/*!
 * Construct and invoke all SetUserAction() kind of classes.
 */
void ActionInitialization::Build() const
{
    std::string root_output = json_input_.at("root_output").get<std::string>();
    const bool  step_data   = json_input_.at("step_info").get<bool>();

    RunAction* run_action = new RunAction();
    run_action->set_output_filename(root_output);

    PrimaryGeneratorAction* primary_action = new PrimaryGeneratorAction();
    primary_action->set_particle_gun(json_input_);

    SetUserAction(run_action);                    //! Create root file
    SetUserAction(primary_action);                //! Set particle gun
    SetUserAction(new EventAction());             //! Set event rules
    SetUserAction(new TrackingAction());          //! Save track info
    SetUserAction(new SteppingAction(step_data)); //! save step data
    // SetUserAction(new StackingAction()); //! Kill secondaries
}
