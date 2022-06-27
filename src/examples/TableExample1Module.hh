// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* TableExample1Module.hh                                        (C) 2000-2022 */
/*                                                                           */
/* Exemple de module utilisant ISimpleTableOutput en tant que singleton.     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <arcane/IParallelMng.h>
#include <arcane/ITimeLoopMng.h>
#include <arcane/ServiceBuilder.h>

// On inclut juste l'interface.
// Pour le singleton, c'est "table.config" 
// qui fait le lien avec le bon service.
#include "simple_table_output/ISimpleTableOutput.hh"

#include "examples/TableExample1_axl.h"

using namespace Arcane;

/*!
  \brief Module TableExample1.
 */
class TableExample1Module : 
public ArcaneTableExample1Object
{

 public:
  explicit TableExample1Module(const ModuleBuildInfo& mbi)
  : ArcaneTableExample1Object(mbi)
  {}

 public:
  void initModule() override;
  void loopModule() override;
  void endModule() override;

  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_TABLEEXAMPLE1(TableExample1Module);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
