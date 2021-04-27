//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PhysicsList.cc
//---------------------------------------------------------------------------//
#include "PhysicsList.hh"

#include <G4ProcessManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicsListHelper.hh>
#include <G4EmBuilder.hh>

#include <G4ComptonScattering.hh>
#include <G4PhotoElectricEffect.hh>
#include <G4LivermorePhotoElectricModel.hh>
#include <G4eplusAnnihilation.hh>
#include <G4eeToTwoGammaModel.hh>
#include <G4eBremsstrahlung.hh>
#include <G4RayleighScattering.hh>
#include <G4eMultipleScattering.hh>
#include <G4UrbanMscModel.hh>
#include <G4WentzelVIModel.hh>
#include <G4eIonisation.hh>
#include <G4GammaConversion.hh>
#include <G4eCoulombScatteringModel.hh>
#include <G4CoulombScattering.hh>

#include "JsonReader.hh"

//---------------------------------------------------------------------------//
/*!
 * Constructor and destructor.
 */
PhysicsList::PhysicsList() : G4VUserPhysicsList()
{
    const auto json = JsonReader::get_instance()->json();

    // Update selected processes
    for (auto map : selected_processes_)
    {
        bool selection
            = json.at("physics").at("list").at(map.first).get<bool>();

        selected_processes_.at(map.first) = selection;
    }

    // Set verbosity level
    int level = json.at("physics").at("verbosity").get<int>();
    G4EmParameters::Instance()->SetVerbose(level);
}

//---------------------------------------------------------------------------//
PhysicsList::~PhysicsList() = default;

//---------------------------------------------------------------------------//
/*!
 * Build list of available particles.
 */
void PhysicsList::ConstructParticle()
{
    G4EmBuilder::ConstructMinimalEmSet();
}

//---------------------------------------------------------------------------//
/*!
 * Add transportation and selected processes.
 */
void PhysicsList::ConstructProcess()
{
    // >>> ADD TRANSPORTATION
    G4VUserPhysicsList::AddTransportation();

    // >>> ADD PHYSICS PROCESSES / MODELS TO THE SIMULATION
    auto physics_list = G4PhysicsListHelper::GetPhysicsListHelper();

    //// Gammas
    auto gamma = G4Gamma::GammaDefinition();
    if (selected_processes_.find("compton_scattering")->second)
    {
        // Compton Scattering: G4KleinNishinaCompton
        physics_list->RegisterProcess(new G4ComptonScattering(), gamma);
    }
    if (selected_processes_.find("photoelectric")->second)
    {
        // Photoelectric effect: G4LivermorePhotoElectricModel
        auto photoelectrict_effect = std::make_unique<G4PhotoElectricEffect>();
        photoelectrict_effect->SetEmModel(new G4LivermorePhotoElectricModel());
        physics_list->RegisterProcess(photoelectrict_effect.release(), gamma);
    }
    if (selected_processes_.find("rayleigh")->second)
    {
        // Rayleigh: G4LivermoreRayleighModel
        physics_list->RegisterProcess(new G4RayleighScattering(), gamma);
    }
    if (selected_processes_.find("gamma_conversion")->second)
    {
        // Gamma conversion: G4PairProductionRelModel
        physics_list->RegisterProcess(new G4GammaConversion(), gamma);
    }

    //// Electrons and positrons
    auto electron = G4Electron::ElectronDefinition();
    auto positron = G4Positron::PositronDefinition();

    if (selected_processes_.find("positron_annihilation")->second)
    {
        // Electron-positron annihilation: G4eeToTwoGammaModel
        physics_list->RegisterProcess(new G4eplusAnnihilation(), positron);
    }
    if (selected_processes_.find("bremmstrahlung")->second)
    {
        // Bremmstrahlung (e+ and e-): SB + LPM
        physics_list->RegisterProcess(new G4eBremsstrahlung(), electron);
        physics_list->RegisterProcess(new G4eBremsstrahlung(), positron);
    }
    if (selected_processes_.find("multiple_scattering")->second)
    {
        // Multiple scattering: Urban (low E) and WentzelVI (high E) models
        double msc_energy_limit = G4EmParameters::Instance()->MscEnergyLimit();
        //// e+
        auto msc_eplus           = std::make_unique<G4eMultipleScattering>();
        auto urban_msc_eplus     = std::make_unique<G4UrbanMscModel>();
        auto wentzelvi_msc_eplus = std::make_unique<G4WentzelVIModel>();
        urban_msc_eplus->SetHighEnergyLimit(msc_energy_limit);
        wentzelvi_msc_eplus->SetLowEnergyLimit(msc_energy_limit);
        msc_eplus->SetEmModel(urban_msc_eplus.release());
        msc_eplus->SetEmModel(wentzelvi_msc_eplus.release());
        //// e-
        auto msc_eminus           = std::make_unique<G4eMultipleScattering>();
        auto urban_msc_eminus     = std::make_unique<G4UrbanMscModel>();
        auto wentzelvi_msc_eminus = std::make_unique<G4WentzelVIModel>();
        urban_msc_eminus->SetHighEnergyLimit(msc_energy_limit);
        wentzelvi_msc_eminus->SetLowEnergyLimit(msc_energy_limit);
        msc_eminus->SetEmModel(urban_msc_eminus.release());
        msc_eminus->SetEmModel(wentzelvi_msc_eminus.release());

        physics_list->RegisterProcess(msc_eminus.release(), electron);
        physics_list->RegisterProcess(msc_eplus.release(), positron);
    }
    if (selected_processes_.find("e_ionization")->second)
    {
        // e-e+ ionization: G4MollerBhabhaModel + G4UniversalFluctuation
        physics_list->RegisterProcess(new G4eIonisation(), electron);
        physics_list->RegisterProcess(new G4eIonisation(), positron);
    }
    if (selected_processes_.find("coulomb_scattering")->second)
    {
        // Coulomb scattering: G4eCoulombScatteringModel
        double msc_energy_limit = G4EmParameters::Instance()->MscEnergyLimit();
        //// e+
        auto coulomb_scat_eplus = std::make_unique<G4CoulombScattering>();
        auto coulomb_model_eplus
            = std::make_unique<G4eCoulombScatteringModel>();
        coulomb_scat_eplus->SetMinKinEnergy(msc_energy_limit);
        coulomb_model_eplus->SetLowEnergyLimit(msc_energy_limit);
        coulomb_model_eplus->SetActivationLowEnergyLimit(msc_energy_limit);
        coulomb_scat_eplus->SetEmModel(coulomb_model_eplus.release());
        //// e-
        auto coulomb_scat_eminus = std::make_unique<G4CoulombScattering>();
        auto coulomb_model_eminus
            = std::make_unique<G4eCoulombScatteringModel>();
        coulomb_scat_eminus->SetMinKinEnergy(msc_energy_limit);
        coulomb_model_eminus->SetLowEnergyLimit(msc_energy_limit);
        coulomb_model_eminus->SetActivationLowEnergyLimit(msc_energy_limit);
        coulomb_scat_eminus->SetEmModel(coulomb_model_eminus.release());

        physics_list->RegisterProcess(coulomb_scat_eminus.release(), electron);
        physics_list->RegisterProcess(coulomb_scat_eplus.release(), positron);
    }
}
