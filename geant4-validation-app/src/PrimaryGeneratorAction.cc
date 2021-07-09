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

#include "JsonReader.hh"
#include "HepMC3Reader.hh"

//---------------------------------------------------------------------------//
/*!
 * Construct with json input information.
 */
PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
    const auto  json_input = JsonReader::get_instance()->json();
    std::string hepmc3_input
        = json_input.at("simulation").at("hepmc3").get<std::string>();

    // TEMPORARILY DISABLED
#if 0
    if (hepmc3_input.empty())
    {
        // No HepMC3 input file provided; use particle gun
        this->set_particle_gun();
    }

    else
    {
        // Use HepMC3 input
        this->set_hepmc3();
    }
#endif

    this->set_particle_gun();
}

//---------------------------------------------------------------------------//
/*!
 * Default destructor.
 */
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
// PRIVATE
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Set up particle gun.
 */
void PrimaryGeneratorAction::set_particle_gun()
{
    const auto json_input = JsonReader::get_instance()->json();
    const auto p          = json_input.at("simulation").at("particle_gun");

    ParticleGunSetup primary;
    primary.pdg       = p.at("pdg").get<int>();
    primary.energy    = p.at("energy").get<double>();
    primary.vertex    = G4ThreeVector(p.at("vertex")[0].get<double>(),
                                   p.at("vertex")[1].get<double>(),
                                   p.at("vertex")[2].get<double>());
    primary.direction = G4ThreeVector(p.at("direction")[0].get<double>(),
                                      p.at("direction")[1].get<double>(),
                                      p.at("direction")[2].get<double>());
    primary.direction = primary.direction.unit();

    // Create the particle gun
    const int number_of_particles = 1;
    particle_gun_ = std::make_shared<G4ParticleGun>(number_of_particles);

    // Particle gun setup
    particle_gun_->SetParticleDefinition(
        G4ParticleTable::GetParticleTable()->FindParticle(primary.pdg));
    particle_gun_->SetParticleMomentumDirection(primary.direction);
    particle_gun_->SetParticleEnergy(primary.energy * MeV);
    particle_gun_->SetParticlePosition(primary.vertex);
}

//---------------------------------------------------------------------------//
/*!
 * Set up hepmc3 input.
 */
void PrimaryGeneratorAction::set_hepmc3() {}
