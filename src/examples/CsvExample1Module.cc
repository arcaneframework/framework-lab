// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* CsvExample1Module.cc                                        (C) 2000-2022 */
/*                                                                           */
/* Exemple de module utilisant ISimpleTableOutput en tant que singleton.     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "CsvExample1Module.hh"
#include <iostream>
#include <random>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void CsvExample1Module::
initModule()
{
  srand(1234);
  // Initialisation de la sortie CSV.
  ISimpleTableOutput* csv = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();
  info() << "Initialisation du csv";
  csv->init("CsvExample1", ";");
  csv->print();
}

void CsvExample1Module::
loopModule()
{
  ISimpleTableOutput* csv = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();
  info() << "Ajout d'une colonne nommée \"Iteration " + String::fromNumber(m_global_iteration()) << "\" dans le csv";
  csv->addColumn("Iteration " + String::fromNumber(m_global_iteration()));

  csv->addElemRow("Ligne 1", rand()%99);
  csv->addElemRow("Ligne 2", rand()%99);
  csv->print();

  if (m_global_iteration() == 3)
    subDomain()->timeLoopMng()->stopComputeLoop(true);
}

void CsvExample1Module::
endModule()
{
  ISimpleTableOutput* csv = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();

  csv->addRows(StringUniqueArray{"Ligne 3", "Ligne 4"});

  csv->addElemRow("Ligne 3", 24);

  UniqueArray<Real> end = {99, 99};
  csv->addElemsColumn("Iteration 2", end.constView());

  csv->editElem("Iteration 1", "Ligne 4", 86);


  UniqueArray<Real> end2 = {99, 99, 99};
  csv->addColumn("Colonne 4", end2.constView());

  csv->editElem(3, 1, 86);

  csv->addAverageColumn("Moyenne");
  csv->print();

  csv->addColumn("Colonne 5");
  csv->addElemRow("Ligne 1", 25);
  csv->addElemSameColumn(26);
  csv->addElemSameColumn(27);

  csv->print();

  if(options()->getCsvPath() != "") {
    info() << "Ecriture du csv dans un fichier";
    csv->writeFile(options()->getCsvPath());
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
