//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PrimaryGeneratorAction.cc
//---------------------------------------------------------------------------//
#include "PrimaryGeneratorAction.hh"

#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>

//---------------------------------------------------------------------------//
/*!
 * Default constructor and destructor.
 */
PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() = default;

//---------------------------------------------------------------------------//
/*!
 * Generate primary at each new event.
 */
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    particle_gun_->GeneratePrimaryVertex(event);
}

//---------------------------------------------------------------------------//
/*!
 * Setup particle gun.
 */
void PrimaryGeneratorAction::set_particle_gun(nlohmann::json json_input)
{
    auto p = json_input.at("primary");

    Primary primary;
    primary.pdg       = p.at("pdg").get<int>();
    primary.energy    = p.at("energy").get<double>();
    primary.vertex    = G4ThreeVector(p.at("vertex")[0].get<double>(),
                                   p.at("vertex")[1].get<double>(),
                                   p.at("vertex")[2].get<double>());
    primary.direction = G4ThreeVector(p.at("direction")[0].get<double>(),
                                      p.at("direction")[1].get<double>(),
                                      p.at("direction")[2].get<double>());

    // Create the particle gun
    G4int number_of_particles = 1;
    particle_gun_ = std::make_shared<G4ParticleGun>(number_of_particles);

    // Select particle
    G4ParticleDefinition* particle;
    particle = G4ParticleTable::GetParticleTable()->FindParticle(primary.pdg);

    // Particle gun setup
    particle_gun_->SetParticleDefinition(particle);
    particle_gun_->SetParticleMomentumDirection(primary.direction);
    particle_gun_->SetParticleEnergy(primary.energy * MeV);
    particle_gun_->SetParticlePosition(primary.vertex);
}
