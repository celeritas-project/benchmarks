//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TrackingAction.hh
//! \brief Controls tracking information
//---------------------------------------------------------------------------//
#pragma once

#include <G4UserTrackingAction.hh>

//---------------------------------------------------------------------------//
/*!
 * Some help
 */
class TrackingAction : public G4UserTrackingAction
{
public:
  TrackingAction();
  ~TrackingAction();

  void PreUserTrackingAction(const G4Track* track) override;
  void PostUserTrackingAction(const G4Track* track) override;
};