//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file ActionInitialization.hh
//! \brief Initialize all SetUserAction() type of classes.
//---------------------------------------------------------------------------//
#pragma once

#include <string>
#include <G4VUserActionInitialization.hh>
#include <nlohmann/json.hpp>

//---------------------------------------------------------------------------//
/*!
 * Call all SetUserAction() type of classes, such as RunAction, EventAction,
 * PrimaryGeneratorAction, TrackingAction, SteppingAction, and so on.
 */
class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(nlohmann::json json_input);
    ~ActionInitialization();

    void Build() const override;

  private:
    nlohmann::json json_input_;
};
