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
  ISimpleTableOutput* csv = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();
  info() << "Initialisation du csv";
  csv->init("CSVExample", ";");
  csv->addRow("Ligne 1");
  csv->print();
  csv->addElemColumn("Colonne 0", 65);
  csv->print();
}

void CsvExampleModule::
loopModule()
{
  ISimpleTableOutput* csv = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();
  info() << "Ajout d'une colonne nommée " << ("Iteration " + String::fromNumber(m_global_iteration())) << " dans le csv";
  csv->addColumn("Iteration " + String::fromNumber(m_global_iteration()));
  csv->print();

  info() << "Ajout de l'élément " << 98 << " sur la ligne " << "Ligne 1";
  csv->addElemRow("Ligne 1", 98);
  csv->print();

  info() << "Ajout de l'élément " << 87 << " sur la ligne " << "Ligne 2";
  csv->addElemRow("Ligne 2", 87);
  csv->print();

  if (m_global_iteration() == 3)
    subDomain()->timeLoopMng()->stopComputeLoop(true);
}

void CsvExampleModule::
endModule()
{
  ISimpleTableOutput* csv = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();

  csv->addRow("Ligne 3");

  UniqueArray<Real> end = {99, 99, 99};
  csv->addColumn("Colonne 4", end.constView());

  csv->editElem(3, 1, 86);

  csv->addAverageColumn("Moyenne");
  csv->print();

  csv->addColumn("Colonne 5");
  csv->addElemRow("Ligne 1", 25);
  //csv->addElemRow("Ligne 2", 26);
  //csv->addElemRow("Ligne 3", 27);
  csv->addElemSameColumn(26);
  csv->addElemSameColumn(27);


  info() << "Affichage du csv";
  csv->print();
  if(options()->getCsvPath() != "") {
    info() << "Ecriture du csv dans un fichier";
    csv->writeFile(options()->getCsvPath());
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
