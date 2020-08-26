//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PhysicsList.hh
//! \brief Builds the physics list
//---------------------------------------------------------------------------//
#pragma once

#include <G4VUserPhysicsList.hh>

//---------------------------------------------------------------------------//
/*!
 * Constructs particles and processes to be used in the simulation run.
 * Klein-Nishina Compton process needs 3 particles to run: gamma, e-, and e+
 * Electrons and positrons will need to be killed in order to provide a better
 * comparison with Celeritas
 */
class PhysicsList : public G4VUserPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList();

    void ConstructParticle() override;
    void ConstructProcess() override;
};