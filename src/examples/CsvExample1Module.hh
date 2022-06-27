// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* CsvExample1Module.hh                                        (C) 2000-2022 */
/*                                                                           */
/* Exemple de module utilisant ISimpleTableOutput en tant que singleton.     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <arcane/IParallelMng.h>
#include <arcane/ITimeLoopMng.h>
#include <arcane/ServiceBuilder.h>
#include "csv_output/ISimpleTableOutput.hh"


#include "CsvExample1_axl.h"

using namespace Arcane;

/*!
  \brief Module CsvExample1.
 */
class CsvExample1Module : 
public ArcaneCsvExample1Object
{

 public:
  explicit CsvExample1Module(const ModuleBuildInfo& mbi)
  : ArcaneCsvExample1Object(mbi)
  {}

 public:
  void initModule() override;
  void loopModule() override;
  void endModule() override;

  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_CSVEXAMPLE1(CsvExample1Module);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
