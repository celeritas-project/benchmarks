//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RootIO.cc
//---------------------------------------------------------------------------//
#include "JsonReader.hh"

#include <iostream>

//---------------------------------------------------------------------------//
/*!
 * Singleton declaration.
 */
static JsonReader* json_reader_singleton = nullptr;

//---------------------------------------------------------------------------//
// PUBLIC
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Constructor singleton.
 */
void JsonReader::construct(std::ifstream& json_filename)
{
    if (!json_reader_singleton)
    {
        json_reader_singleton = new JsonReader(json_filename);
    }
    else
    {
        std::cout << "ROOT I/O already constructed. Nothing to do.\n";
        return;
    }
}

//---------------------------------------------------------------------------//
/*!
 * Get static JsonReader instance. 
 *
 * \c JsonReader::construct(ifstream_file) *MUST* be called before this.
 */
JsonReader* JsonReader::get_instance()
{
    return json_reader_singleton;
}

//---------------------------------------------------------------------------//
/*!
 * Get loaded json.
 */
nlohmann::json& JsonReader::json()
{
    return json_;
}

//---------------------------------------------------------------------------//
// PRIVATE
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
/*!
 * Construct from json filename.
 */
JsonReader::JsonReader(std::ifstream& json_filename)
{
    json_ = nlohmann::json::parse(json_filename);
}
