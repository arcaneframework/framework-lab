// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* TableExample3Module.hh                                      (C) 2000-2022 */
/*                                                                           */
/* Exemple 3 de module utilisant ISimpleTableOutput en tant que service.     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <arcane/IParallelMng.h>
#include <arcane/ITimeLoopMng.h>
#include <arcane/ServiceBuilder.h>
#include "simple_table_output/ISimpleTableOutput.hh"


#include "examples/TableExample3_axl.h"

using namespace Arcane;

/*!
  \brief Module TableExample3.
 */
class TableExample3Module : 
public ArcaneTableExample3Object
{

 public:
  explicit TableExample3Module(const ModuleBuildInfo& mbi)
  : ArcaneTableExample3Object(mbi)
  {}

 public:
  void initModule() override;
  void loopModule() override;
  void endModule() override;

  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_TABLEEXAMPLE3(TableExample3Module);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
