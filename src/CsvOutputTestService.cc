// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* CsvOutputTestService.cc                                         (C) 2000-2022 */
/*                                                                           */
/*                     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


#include "CsvOutputService.hh"

void CsvOutputService::
setUpForClass()
{

}


void CsvOutputService::
setUp()
{

}

void CsvOutputService::
testInitProcId()
{
  init("Bonjour @proc_id@", ";");
  ASSERT_EQUAL("Bonjour 0", m_name_tab);
}
void CsvOutputService::
testClear()
{
  init();
  addRow("Ma ligne 1");
  addRow("Ma ligne 2");
  addElemColumn("Ma colonne 1", 0, true);
  addElemColumn("Ma colonne 1", 1, false);
  addColumn("Ma colonne 2");
  addElemRow("Ma ligne 1", 2, false);
  addElemRow("Ma ligne 2", 3, false);
  clear();
  
  ASSERT_EQUAL(0, m_values_csv.dim1Size());
  ASSERT_EQUAL(0, m_values_csv.dim2Size());

  ASSERT_EQUAL(0, m_name_rows.size());
  ASSERT_EQUAL(0, m_name_columns.size());
  ASSERT_EQUAL(0, m_size_rows.size());
  ASSERT_EQUAL(0, m_size_columns.size());

}
void CsvOutputService::
testComputeAt()
{
  bool only_P0;
  ASSERT_EQUAL("0", _computeAt("@proc_id@", only_P0));
  ASSERT_FALSE(only_P0);
  ASSERT_EQUAL("nimp", _computeAt("@nimp@", only_P0));
  ASSERT_TRUE(only_P0);
  ASSERT_EQUAL("abcd", _computeAt("a@b@c@d@", only_P0));
  ASSERT_TRUE(only_P0);
}
void CsvOutputService::
testComputeFinalPath()
{
  init("Bonjour_P@proc_id@", ";");
  m_path = "./its4P@proc_id@/";

  ASSERT_EQUAL("./its4P0/Bonjour_P0.csv", _computeFinalPath());
  ASSERT_EQUAL("./its4P0/", m_path);
  ASSERT_EQUAL("Bonjour_P0", m_name_tab);
}

void CsvOutputService::
tearDown()
{
  clear();
}

void CsvOutputService::
tearDownForClass()
{

}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


#ifndef REGISTER_CSV_SERVICE
#define REGISTER_CSV_SERVICE

ARCANE_REGISTER_SERVICE_CSVOUTPUT(CsvOutput, CsvOutputService);

#endif
