// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* CsvExampleModule.hh                                                 (C) 2000-2022 */
/*                                                                           */
/*                                                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <arcane/IParallelMng.h>
#include <arcane/ITimeLoopMng.h>
#include <arcane/ServiceBuilder.h>
#include "ISimpleTableOutput.hh"


#include "CsvExample_axl.h"

using namespace Arcane;

/*!
  \brief Module CsvExample.
 */
class CsvExampleModule : 
public ArcaneCsvExampleObject
{

 public:
  explicit CsvExampleModule(const ModuleBuildInfo& mbi)
  : ArcaneCsvExampleObject(mbi)
  {}

 public:
  void initModule() override;
  void loopModule() override;
  void endModule() override;

  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_CSVEXAMPLE(CsvExampleModule);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
