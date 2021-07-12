//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file HepMC3Reader.cc
//---------------------------------------------------------------------------//
#include "HepMC3Reader.hh"

#include <iostream>
#include <assert.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/ReaderFactory.h>
#include "JsonReader.hh"

//---------------------------------------------------------------------------//
/*!
 * Singleton declaration.
 */
static HepMC3Reader* hepmc3_singleton = nullptr;

//---------------------------------------------------------------------------//
// PUBLIC
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Constructor singleton.
 */
void HepMC3Reader::construct()
{
    if (!hepmc3_singleton)
    {
        hepmc3_singleton = new HepMC3Reader();
    }

    else
    {
        std::cout << "HepMC3 Reader already constructed. Nothing to do.\n";
        return;
    }
}

//---------------------------------------------------------------------------//
/*!
 * Get static HepMC3Reader instance. \c construct() *MUST* be called before
 * this.
 */
HepMC3Reader* HepMC3Reader::get_instance()
{
    return hepmc3_singleton;
}

//---------------------------------------------------------------------------//
/*!
 * Return the next event from file as a shared_ptr.
 */
bool HepMC3Reader::read_event()
{
    // Parse the next event
    HepMC3::GenEvent gen_event;
    if (input_file_->read_event(gen_event))
    {
        gen_event_ = std::make_shared<HepMC3::GenEvent>(gen_event);
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------//
/*!
 * Return the list of particles of the currently loaded event.
 */
HepMC3::GenParticles HepMC3Reader::event_particles()
{
    assert(gen_event_);
    return gen_event_->particles();
}

//---------------------------------------------------------------------------//
/*!
 * Return the list of vertices of the currently loaded event.
 */
std::vector<HepMC3::GenVertexPtr> HepMC3Reader::event_vertices()
{
    assert(gen_event_);
    return gen_event_->vertices();
}

//---------------------------------------------------------------------------//
/*!
 * Return the current event number.
 */
size_t HepMC3Reader::event_number()
{
    assert(gen_event_);
    return gen_event_->event_number();
}

//---------------------------------------------------------------------------//
// PRIVATE
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Construct new HepMC3Reader from json input data.
 */
HepMC3Reader::HepMC3Reader() : number_of_events_(-1)
{
    // Load input
    const auto json  = JsonReader::get_instance()->json();
    const auto input = json.at("simulation").at("hepmc3").get<std::string>();
    input_file_      = HepMC3::deduce_reader(input);

    // Fetch total number of events
    const auto file = HepMC3::deduce_reader(input);

    while (!file->failed())
    {
        // Parse the next event from the record
        HepMC3::GenEvent gen_event;
        gen_event.set_units(HepMC3::Units::MEV, HepMC3::Units::CM);

        file->read_event(gen_event);
        number_of_events_++;
    }
}