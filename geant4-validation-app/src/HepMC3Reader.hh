//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file HepMC3Reader.hh
//! \brief HepMC3 reader interface.
//---------------------------------------------------------------------------//
#pragma once

#include <memory>
#include <vector>
#include <HepMC3/GenParticle_fwd.h>
#include <HepMC3/GenVertex_fwd.h>

#include "Event.hh"

namespace HepMC3
{
class GenEvent;
class Reader;
} // namespace HepMC3

//---------------------------------------------------------------------------//
/*!
 * HepMC3 reading interface. It creates a singleton to load input data.
 *
 * Use \c HepMC3Reader::construct_reader() to create an instance of the
 * reader. Call \c HepMC3Reader::get_instance() to access the constructed
 * HepMC3Reader object from any class method.
 */
class HepMC3Reader
{
  public:
    // Construct by creating singleton
    static void construct();
    // Get singleton instance
    static HepMC3Reader* get_instance();
    // Get next event
    bool read_event();
    // Get total number of events
    size_t number_of_events() { return number_of_events_; }
    // Get current event number
    size_t event_number();
    // Get primaries of current event
    std::vector<utils::Primary>& event_primaries() { return event_primaries_; }

  private:
    // Store HepMC3 input file
    std::shared_ptr<HepMC3::Reader> input_file_;
    // Event data
    std::shared_ptr<HepMC3::GenEvent> gen_event_;
    // List of primaries per event
    std::vector<utils::Primary> event_primaries_;
    // Total number of events
    size_t number_of_events_;

  private:
    HepMC3Reader();
    ~HepMC3Reader() = default;
};
