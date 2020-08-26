//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.hh
//! \brief Sets up event by event executions
//---------------------------------------------------------------------------//
#pragma once

#include <G4UserEventAction.hh>

//---------------------------------------------------------------------------//
/*!
 * Some help
 */
class EventAction : public G4UserEventAction
{
public:
  EventAction();
  ~EventAction();

  void BeginOfEventAction(const G4Event* event) override;
  //void EndOfEventAction(const G4Event* event) override;

  int event_id_;
};