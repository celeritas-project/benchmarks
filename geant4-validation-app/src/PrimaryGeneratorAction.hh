//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PrimaryGeneratorAction.hh
//! \brief Define the particle gun.
//---------------------------------------------------------------------------//
#pragma once

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4Event.hh>
#include <G4ParticleGun.hh>

//---------------------------------------------------------------------------//
/*!
 * Set and run the particle gun.
 */
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* event) override;

  private:
    void set_particle_gun();

  private:
    struct ParticleGunSetup
    {
        int           pdg;
        double        energy;
        G4ThreeVector vertex;
        G4ThreeVector direction;
    };

    std::shared_ptr<G4ParticleGun> particle_gun_;
};
