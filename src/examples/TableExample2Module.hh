// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* TableExample2Module.hh                                        (C) 2000-2022 */
/*                                                                           */
/* Exemple de module utilisant ISimpleTableOutput en tant que singleton.     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <arcane/IParallelMng.h>
#include <arcane/ITimeLoopMng.h>
#include <arcane/ServiceBuilder.h>
#include "simple_table_output/ISimpleTableOutput.hh"


#include "examples/TableExample2_axl.h"

using namespace Arcane;

/*!
  \brief Module TableExample2.
 */
class TableExample2Module : 
public ArcaneTableExample2Object
{

 public:
  explicit TableExample2Module(const ModuleBuildInfo& mbi)
  : ArcaneTableExample2Object(mbi)
  {}

 public:
  void initModule() override;
  void loopModule() override;
  void endModule() override;

  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_TABLEEXAMPLE2(TableExample2Module);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
