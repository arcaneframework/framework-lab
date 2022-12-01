// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
 //-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* LauncherModule.h                                            (C) 2000-2022 */
/*                                                                           */
/* Module permettant de créer l'objet gérant le message passing et de lancer */
/* le programme qui utilise des appels MPI.                                  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef LAUNCHERMODULE_H
#define LAUNCHERMODULE_H
 
#include <arcane/ITimeLoopMng.h>
#include "Launcher_axl.h"
 
using namespace Arcane;
 
class LauncherModule
: public ArcaneLauncherObject
{
 public:
  explicit LauncherModule(const ModuleBuildInfo& mbi) 
  : ArcaneLauncherObject(mbi) { }
 
 public:
  void beginCompute() override;
  void endCompute() override;
  void compute() override;
  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }
};
 
#endif