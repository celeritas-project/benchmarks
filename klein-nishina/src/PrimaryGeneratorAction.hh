//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PrimaryGeneratorAction.hh
//! \brief Defines the particle gun used in the Geant4 run
//---------------------------------------------------------------------------//
#pragma once

#include <memory>

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4Event.hh>
#include <G4ParticleGun.hh>

//---------------------------------------------------------------------------//
//! Implementation of a mandatory Geant4 virtual class
//! The particle gun used in each event is created in this class
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* event) override;

  private:
    std::shared_ptr<G4ParticleGun> particle_gun_;
};
