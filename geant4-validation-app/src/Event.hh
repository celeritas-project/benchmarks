//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file Event.h
//! \brief Event structure for Geant4 ROOT files.
//---------------------------------------------------------------------------//
#pragma once

#include <vector>
#include <string>

namespace utils
{
//---------------------------------------------------------------------------//
/*!
 * Structure used to write Event information in the ROOT output file.
 *
 * \sa RootIO.hh
 */
struct Vector3
{
    double x;
    double y;
    double z;

    double operator[](int i) const
    {
        switch (i)
        {
            case 0:
                return this->x;
                break;
            case 1:
                return this->y;
                break;
            case 2:
                return this->z;
                break;
            default:
                return 0;
        }
    }
};

struct Bin
{
    double x_min;
    double x_max;
};

struct Step
{
    std::string process;
    double      kinetic_energy; //!< [MeV]
    double      energy_loss;    //!< [MeV]
    Vector3     direction;      //!< Unit vector
    Vector3     position;       //!< [cm]
    double      global_time;    //!< [s]
};

struct Track
{
    int               pdg;
    int               id;
    int               parent_id;
    double            length;             //!< [MeV]
    double            energy_dep;         //!< [MeV]
    double            vertex_energy;      //!< [MeV]
    double            vertex_global_time; //!< [s]
    Vector3           vertex_direction;   //!< Unit vector
    Vector3           vertex_position;    //!< [cm]
    std::vector<Step> steps;
};

struct Vertex
{
    Track              incident;
    std::vector<Track> secondaries;
    Vector3            position;    //!< [cm]
    double             global_time; //!< [s]
};

struct HitData
{
    unsigned int track_id;
    unsigned int parent_id;
    double       energy_deposition;
    Vector3      position;
    Vector3      direction;
};

struct SensitiveDetectorScore
{
    std::string          sensitive_detector_name;
    std::vector<HitData> hits;
};

struct Event
{
    int                                 id;
    Track                               primary;
    std::vector<Track>                  secondaries;
    std::vector<Vertex>                 vertices;
    std::vector<SensitiveDetectorScore> sensitive_detectors;
};

//---------------------------------------------------------------------------//
} // namespace utils
