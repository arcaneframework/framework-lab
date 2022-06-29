// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* TableExample5Module.hh                                      (C) 2000-2022 */
/*                                                                           */
/* Exemple 5 de module utilisant ISimpleTableOutput en tant que service.     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <arcane/IParallelMng.h>
#include <arcane/ITimeLoopMng.h>
#include <arcane/ServiceBuilder.h>
#include "simple_table_output/ISimpleTableOutput.hh"


#include "examples/TableExample5_axl.h"

using namespace Arcane;

/*!
  \brief Module TableExample5.
 */
class TableExample5Module : 
public ArcaneTableExample5Object
{

 public:
  explicit TableExample5Module(const ModuleBuildInfo& mbi)
  : ArcaneTableExample5Object(mbi)
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

ARCANE_REGISTER_MODULE_TABLEEXAMPLE5(TableExample5Module);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
