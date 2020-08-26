//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PrimaryGeneratorAction.cc
//---------------------------------------------------------------------------//

#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>
#include <g4root.hh>

#include "PrimaryGeneratorAction.hh"

//---------------------------------------------------------------------------//
//! Set up a particle gun with a gamma as primary
PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
    // Create the particle gun
    G4int number_of_particles = 1;
    particle_gun_ = std::make_shared<G4ParticleGun>(number_of_particles);

    // Select gamma
    G4ParticleDefinition *particle;
    particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");

    // Particle gun setup
    G4ThreeVector position(0, 0, 0);
    particle_gun_->SetParticleDefinition(particle);
    particle_gun_->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    particle_gun_->SetParticleEnergy(1 * GeV);
    particle_gun_->SetParticlePosition(position);
}

//---------------------------------------------------------------------------//
PrimaryGeneratorAction::~PrimaryGeneratorAction() = default;

//---------------------------------------------------------------------------//
//! Implementation of a mandatory Geant4 virtual class method
//! Method is called at the begining of each event
void PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
    particle_gun_->GeneratePrimaryVertex(event);
}
