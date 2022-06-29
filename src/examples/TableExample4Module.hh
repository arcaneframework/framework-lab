// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* TableExample4Module.hh                                      (C) 2000-2022 */
/*                                                                           */
/* Exemple 4 de module utilisant ISimpleTableOutput en tant que service.     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <arcane/IParallelMng.h>
#include <arcane/ITimeLoopMng.h>
#include <arcane/ServiceBuilder.h>
#include "simple_table_output/ISimpleTableOutput.hh"


#include "examples/TableExample4_axl.h"

using namespace Arcane;

/*!
  \brief Module TableExample4.
 */
class TableExample4Module : 
public ArcaneTableExample4Object
{

 public:
  explicit TableExample4Module(const ModuleBuildInfo& mbi)
  : ArcaneTableExample4Object(mbi)
  {}

 public:
  void initModule() override;
  void loopModule() override;
  void endModule() override;

  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }

 protected:
  Integer pos_fis, pos_col;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_TABLEEXAMPLE4(TableExample4Module);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
