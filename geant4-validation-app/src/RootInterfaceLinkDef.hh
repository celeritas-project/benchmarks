//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RootInterfaceLinkDef.hh
//! \brief Define classes added to the ROOT dictionary.
//---------------------------------------------------------------------------//
#ifdef __CINT__

// clang-format off
#pragma link C++ struct utils::Vector3+;
#pragma link C++ struct utils::Step+;
#pragma link C++ struct utils::Track+;
#pragma link C++ struct utils::Vertex+;
#pragma link C++ struct utils::Event+;
// clang-format on

#endif
