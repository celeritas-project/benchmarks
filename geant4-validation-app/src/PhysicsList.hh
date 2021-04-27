//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PhysicsList.hh
//! \brief Build the physics list.
//---------------------------------------------------------------------------//
#pragma once

#include <map>
#include <G4VUserPhysicsList.hh>

//---------------------------------------------------------------------------//
/*!
 * Constructs particles and processes to be used in the simulation run.
 */
class PhysicsList : public G4VUserPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList();

    void ConstructParticle() override;
    void ConstructProcess() override;

  private:
    // Map of selected processes. Booleans are updated at construction time.
    std::map<std::string, bool> selected_processes_
        = {{"compton_scattering", false},
           {"photoelectric", false},
           {"rayleigh", false},
           {"gamma_conversion", false},
           {"positron_annihilation", false},
           {"bremmstrahlung", false},
           {"multiple_scattering", false},
           {"e_ionization", false},
           {"coulomb_scattering", false}};
};
