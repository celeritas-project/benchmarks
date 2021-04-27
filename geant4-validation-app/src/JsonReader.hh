//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file JsonReader.hh
//! \brief nlohmann/json singleton.
//---------------------------------------------------------------------------//
#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

//---------------------------------------------------------------------------//
/*!
 * JSON singleton for allowing global access to the \e nlohmann/json parser.
 */
class JsonReader
{
  public:
    // Construct by creating singleton from the json filename
    static void construct(std::ifstream& json_filename);

    // Get singleton instance
    static JsonReader* get_instance();

    // Get parsed json for reading
    nlohmann::json& json();

  private:
    JsonReader(std::ifstream& json_filename);

  private:
    nlohmann::json json_;
};
