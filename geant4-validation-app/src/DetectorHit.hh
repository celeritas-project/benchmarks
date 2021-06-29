//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorHit.hh
//! \brief Sensitive detector hit
//---------------------------------------------------------------------------//
#pragma once

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>

#include "Event.hh"

using utils::HitData;

//---------------------------------------------------------------------------//
/*!
 * Interface for sensitive detector hits.
 */
class DetectorHit : public G4VHit
{

  public:
    DetectorHit(HitData& hit);
    DetectorHit();
    ~DetectorHit();

    void print();

  public:
    HitData hit_;
};

typedef G4THitsCollection<DetectorHit> HitsCollection;
