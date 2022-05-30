// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* CsvExampleModule.cc                                                 (C) 2000-2022 */
/*                                                                           */
/*                                                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "CsvExampleModule.hh"
#include <iostream>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void CsvExampleModule::
initModule()
{
  // Initialisation de la sortie CSV.
  ISimpleOutput* csv = ServiceBuilder<ISimpleOutput>(subDomain()).getSingleton();
  info() << "Initialisation du csv";
  csv->init("CSVExample", ";");
}

void CsvExampleModule::
loopModule()
{
  ISimpleOutput* csv = ServiceBuilder<ISimpleOutput>(subDomain()).getSingleton();
  info() << "Ajout d'une colonne dans le csv";
  csv->addColumn("Iteration " + String::fromNumber(m_global_iteration()));

  info() << "Ajout d'une ligne dans le csv";
  csv->addElemRow("Ligne 1", 0);
  csv->addElemRow("Ligne 2", 1);

  if (m_global_iteration() == 3)
    subDomain()->timeLoopMng()->stopComputeLoop(true);
}

void CsvExampleModule::
endModule()
{
  ISimpleOutput* csv = ServiceBuilder<ISimpleOutput>(subDomain()).getSingleton();
  info() << "Affichage du csv";
  csv->print();
  if(options()->getCsvFile() != "") {
    info() << "Ecriture du csv dans un fichier";
    csv->writeFile(options()->getCsvFile());
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
