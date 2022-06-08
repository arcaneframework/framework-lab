// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* SimpleTableOutputTesterService.cc                                                 (C) 2000-2022 */
/*                                                                           */
/*                                                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "SimpleTableOutputTesterService.hh"
#include <iostream>

void SimpleTableOutputTesterService::
setUpForClass()
{
  ptrSTO = options()->getCsvOutput();
}


void SimpleTableOutputTesterService::
setUp()
{
  ptrSTO->init();
}

void SimpleTableOutputTesterService::
testAddRow1()
{
  ptrSTO->addRow("Ma ligne", false);
  ASSERT_EQUAL(1, ptrSTO->getNumRows());
  ASSERT_EQUAL(0, ptrSTO->getNumColumns());
  ptrSTO->addRow("Ma seconde ligne", true);
  ASSERT_EQUAL(2, ptrSTO->getNumRows());
  ASSERT_EQUAL(0, ptrSTO->getNumColumns());
}
void SimpleTableOutputTesterService::
testAddRow2()
{
  ptrSTO->addColumn("Ma colonne 1", false);
  ptrSTO->addColumn("Ma colonne 2", false);
  Integer pos = ptrSTO->addRow("Ma ligne", true);
  ptrSTO->addColumn("Ma colonne 3", false);
  ASSERT_EQUAL(1, ptrSTO->getNumRows());
  ASSERT_EQUAL(3, ptrSTO->getNumColumns());
  ASSERT_EQUAL(2, ptrSTO->getSizeRow(pos));
  ASSERT_EQUAL(2, ptrSTO->getSizeRow("Ma ligne"));
}

void SimpleTableOutputTesterService::
testAddColumn1()
{
  ptrSTO->addColumn("Ma colonne", false);
  ASSERT_EQUAL(1, ptrSTO->getNumColumns());
  ASSERT_EQUAL(0, ptrSTO->getNumRows());
  ptrSTO->addColumn("Ma seconde colonne", true);
  ASSERT_EQUAL(2, ptrSTO->getNumColumns());
  ASSERT_EQUAL(0, ptrSTO->getNumRows());
}
void SimpleTableOutputTesterService::
testAddColumn2()
{
  ptrSTO->addRow("Ma ligne 1", false);
  ptrSTO->addRow("Ma ligne 2", false);
  Integer pos = ptrSTO->addColumn("Ma colonne", true);
  ptrSTO->addRow("Ma ligne 3", false);
  ASSERT_EQUAL(1, ptrSTO->getNumColumns());
  ASSERT_EQUAL(3, ptrSTO->getNumRows());
  ASSERT_EQUAL(2, ptrSTO->getSizeColumn(pos));
  ASSERT_EQUAL(2, ptrSTO->getSizeColumn("Ma colonne"));
}

void SimpleTableOutputTesterService::
myTestMethod3()
{

}

void SimpleTableOutputTesterService::
tearDown()
{
  ptrSTO->clear();
}


void SimpleTableOutputTesterService::
tearDownForClass()
{

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
