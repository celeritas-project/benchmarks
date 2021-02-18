//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file Event.h
//! \brief Event structure for post-processed Geant4 ROOT files.
//---------------------------------------------------------------------------//
#pragma once

#include <vector>
#include <string>

namespace utils
{
//---------------------------------------------------------------------------//
struct Vector3
{
    double x;
    double y;
    double z;
};

struct Step
{
    std::string process;
    double      kinetic_energy;
    double      energy_loss;
    Vector3     direction;
    Vector3     position;
    double      global_time;
};

struct Track
{
    int               pdg;
    int               id;
    int               parent_id;
    double            length;
    double            energy_dep;
    double            vertex_energy;
    double            vertex_global_time;
    Vector3           vertex_direction;
    Vector3           vertex_position;
    std::vector<Step> steps;
};

struct Vertex
{
    Track              incident;
    std::vector<Track> secondaries;
    Vector3            position;
    double             global_time;
};

struct Event
{
    int                 id;
    Track               primary;
    std::vector<Track>  secondaries;
    std::vector<Vertex> vertices;
};
//---------------------------------------------------------------------------//
} // namespace utils
