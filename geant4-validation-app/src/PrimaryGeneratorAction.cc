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
#include "Event.hh"

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

    is_hepmc3_ = hepmc3_input.empty() ? false : true;

    if (!is_hepmc3_)
    {
        // No HepMC3 input file provided; use particle gun
        this->set_particle_gun();
    }
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
    if (is_hepmc3_)
    {
        const auto hepmc3 = HepMC3Reader::get_instance();

        // Read event and set appropriate conditions
        hepmc3->read_event();
        const auto primaries = hepmc3->event_primaries();
        for (const auto& primary : primaries)
        {
            G4ParticleGun particle_gun;

            particle_gun.SetParticleDefinition(
                G4ParticleTable::GetParticleTable()->FindParticle(primary.pdg));
            particle_gun.SetParticleEnergy(primary.energy);
            particle_gun.SetParticlePosition(G4ThreeVector(
                primary.vertex.x, primary.vertex.y, primary.vertex.z));
            particle_gun.SetParticleMomentum(G4ThreeVector(
                primary.momentum.x, primary.momentum.y, primary.momentum.z));

            particle_gun.GeneratePrimaryVertex(event);
        }
    }
    else
    {
        particle_gun_->GeneratePrimaryVertex(event);
    }
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

    utils::Primary primary;
    primary.pdg    = p.at("pdg").get<int>();
    primary.energy = p.at("energy").get<double>();
    G4ThreeVector vertex(p.at("vertex")[0].get<double>(),
                         p.at("vertex")[1].get<double>(),
                         p.at("vertex")[2].get<double>());
    G4ThreeVector direction(p.at("momentum")[0].get<double>(),
                            p.at("momentum")[1].get<double>(),
                            p.at("momentum")[2].get<double>());
    direction = direction.unit();

    // Create the particle gun
    const int number_of_particles = 1;
    particle_gun_ = std::make_shared<G4ParticleGun>(number_of_particles);

    // Particle gun setup
    particle_gun_->SetParticleDefinition(
        G4ParticleTable::GetParticleTable()->FindParticle(primary.pdg));
    particle_gun_->SetParticleMomentumDirection(direction);
    particle_gun_->SetParticleEnergy(primary.energy * MeV);
    particle_gun_->SetParticlePosition(vertex);
}
