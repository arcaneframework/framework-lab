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

template<class T>
void SimpleTableOutputTesterService::
ASSERT_EQUAL_ARRAY(UniqueArray<T> expected, UniqueArray<T> actual)
{
  ASSERT_EQUAL(expected.size(), actual.size());
  for(Integer i = 0; i < actual.size(); i++){
    ASSERT_EQUAL(expected[i], actual[i]);
  }
}

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
  ptrSTO->addRow("Ma ligne");
  ASSERT_EQUAL(1, ptrSTO->getNumRows());
  ASSERT_EQUAL(0, ptrSTO->getNumColumns());
  ptrSTO->addRow("Ma seconde ligne");
  ASSERT_EQUAL(2, ptrSTO->getNumRows());
  ASSERT_EQUAL(0, ptrSTO->getNumColumns());
}
void SimpleTableOutputTesterService::
testAddRow2()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  Integer pos = ptrSTO->addRow("Ma ligne");
  ptrSTO->addColumn("Ma colonne 3");
  ASSERT_EQUAL(1, ptrSTO->getNumRows());
  ASSERT_EQUAL(3, ptrSTO->getNumColumns());
  ASSERT_EQUAL(0, ptrSTO->getSizeRow(pos));
  ASSERT_EQUAL(0, ptrSTO->getSizeRow("Ma ligne"));
}
void SimpleTableOutputTesterService::
testAddRow3()
{
  RealUniqueArray test = {1, 2, 3, 4};
  RealUniqueArray result = {1, 2, 3};
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");
  Integer pos = ptrSTO->addRow("Ma ligne", test);
  ptrSTO->addColumn("Ma colonne 4");
  ASSERT_EQUAL_ARRAY(result, ptrSTO->getRow("Ma ligne"));
  ASSERT_EQUAL(3, ptrSTO->getSizeRow("Ma ligne"));
}

void SimpleTableOutputTesterService::
testAddColumn1()
{
  ptrSTO->addColumn("Ma colonne");
  ASSERT_EQUAL(1, ptrSTO->getNumColumns());
  ASSERT_EQUAL(0, ptrSTO->getNumRows());
  ptrSTO->addColumn("Ma seconde colonne");
  ASSERT_EQUAL(2, ptrSTO->getNumColumns());
  ASSERT_EQUAL(0, ptrSTO->getNumRows());
}
void SimpleTableOutputTesterService::
testAddColumn2()
{
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  Integer pos = ptrSTO->addColumn("Ma colonne");
  ptrSTO->addRow("Ma ligne 3");
  ASSERT_EQUAL(1, ptrSTO->getNumColumns());
  ASSERT_EQUAL(3, ptrSTO->getNumRows());
  ASSERT_EQUAL(0, ptrSTO->getSizeColumn(pos));
  ASSERT_EQUAL(0, ptrSTO->getSizeColumn("Ma colonne"));
}
void SimpleTableOutputTesterService::
testAddColumn3()
{
  RealUniqueArray test = {1, 2, 3, 4};
  RealUniqueArray result = {1, 2, 3};
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");
  Integer pos = ptrSTO->addColumn("Ma colonne", test);
  ptrSTO->addRow("Ma ligne 4");
  ASSERT_EQUAL_ARRAY(result, ptrSTO->getColumn("Ma colonne"));
  ASSERT_EQUAL(3, ptrSTO->getSizeColumn("Ma colonne"));
}

void SimpleTableOutputTesterService::
testAddElemRow1()
{
  Integer pos = ptrSTO->addRow("Ma ligne");
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  RealUniqueArray result = {1, 2, 3};

  ASSERT_TRUE(ptrSTO->addElemRow(pos, 1));
  ASSERT_TRUE(ptrSTO->addElemRow(pos, 2));
  ASSERT_TRUE(ptrSTO->addElemRow(pos, 3));
  ASSERT_FALSE(ptrSTO->addElemRow(pos, 4));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getRow("Ma ligne"));
}

void SimpleTableOutputTesterService::
testAddElemRow2()
{
  ptrSTO->addRow("Ma ligne vide");
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  RealUniqueArray result = {1, 2, 3};

  ASSERT_FALSE(ptrSTO->addElemRow("Ma ligne", 1, false));
  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne", 1));
  Integer pos = ptrSTO->getPosRow("Ma ligne");
  ASSERT_TRUE(ptrSTO->addElemRow(pos, 2));
  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne", 3));
  ASSERT_FALSE(ptrSTO->addElemRow("Ma ligne", 4));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getRow("Ma ligne"));
}

void SimpleTableOutputTesterService::
testAddElemSameRow1()
{
  ptrSTO->addRow("Ma ligne vide");
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  RealUniqueArray result = {1, 2, 3, 4};

  ASSERT_FALSE(ptrSTO->addElemRow("Ma ligne", 1, false));
  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne", 1));
  ASSERT_TRUE(ptrSTO->addElemSameRow(2));

  Integer pos = ptrSTO->getPosRow("Ma ligne");
  ASSERT_TRUE(ptrSTO->addElemRow(pos, 3));

  ptrSTO->addColumn("Ma colonne 4");
  ASSERT_TRUE(ptrSTO->addElemSameRow(4));

  ASSERT_FALSE(ptrSTO->addElemSameRow(5));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getRow("Ma ligne"));
}

void SimpleTableOutputTesterService::
testAddElemsRow1()
{
  Integer pos = ptrSTO->addRow("Ma ligne");
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");
  RealUniqueArray test = {1, 2, 3};
  RealUniqueArray result1 = {1, 2, 3};
  RealUniqueArray result2 = {1, 2, 3, 1, 2};
  RealUniqueArray result3 = {1, 2, 3, 1, 2, 1};
  ASSERT_TRUE(ptrSTO->addElemsRow(pos, test));
  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getRow("Ma ligne"));

  ptrSTO->addColumn("Ma colonne 4");
  ptrSTO->addColumn("Ma colonne 5");

  ASSERT_FALSE(ptrSTO->addElemsRow(pos, test));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getRow("Ma ligne"));

  ptrSTO->addColumn("Ma colonne 6");

  ASSERT_FALSE(ptrSTO->addElemsRow(pos, test));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getRow("Ma ligne"));
}

void SimpleTableOutputTesterService::
testAddElemsRow2()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");
  RealUniqueArray test = {1, 2, 3};
  RealUniqueArray result1 = {1, 2, 3};
  RealUniqueArray result2 = {1, 2, 3, 1, 2};
  RealUniqueArray result3 = {1, 2, 3, 1, 2, 1};

  ASSERT_TRUE(ptrSTO->addElemsRow("Ma ligne", test));
  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getRow("Ma ligne"));

  ptrSTO->addColumn("Ma colonne 4");
  ptrSTO->addColumn("Ma colonne 5");

  ASSERT_FALSE(ptrSTO->addElemsRow("Ma ligne", test));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getRow("Ma ligne"));

  ptrSTO->addColumn("Ma colonne 6");

  Integer pos = ptrSTO->getPosRow("Ma ligne");

  ASSERT_FALSE(ptrSTO->addElemsRow(pos, test));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getRow("Ma ligne"));
}

void SimpleTableOutputTesterService::
testAddElemsSameRow1()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");
  RealUniqueArray test = {1, 2, 3};
  RealUniqueArray result1 = {1, 2, 3};
  RealUniqueArray result2 = {1, 2, 3, 1, 2};
  RealUniqueArray result3 = {1, 2, 3, 1, 2, 1};

  ASSERT_TRUE(ptrSTO->addElemsRow("Ma ligne", test));
  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getRow("Ma ligne"));

  ptrSTO->addColumn("Ma colonne 4");
  ptrSTO->addColumn("Ma colonne 5");

  ASSERT_FALSE(ptrSTO->addElemsSameRow(test));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getRow("Ma ligne"));

  ptrSTO->addColumn("Ma colonne 6");

  Integer pos = ptrSTO->getPosRow("Ma ligne");

  ASSERT_FALSE(ptrSTO->addElemsRow(pos, test));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getRow("Ma ligne"));
}

// void SimpleTableOutputTesterService::
// testAddElemColumn1()
// {
//   Integer pos = ptrSTO->addColumn("Ma colonne");
//   ptrSTO->addRow("Ma ligne 1");
//   ptrSTO->addRow("Ma ligne 2");
//   ptrSTO->addRow("Ma ligne 3");

//   RealUniqueArray result = {1, 2, 3};

//   ASSERT_TRUE(ptrSTO->addElemColumn(pos, 1));
//   ASSERT_TRUE(ptrSTO->addElemColumn(pos, 2));
//   ASSERT_TRUE(ptrSTO->addElemColumn(pos, 3));
//   ASSERT_FALSE(ptrSTO->addElemColumn(pos, 4));

//   ASSERT_EQUAL_ARRAY(result, ptrSTO->getColumn("Ma colonne"));
// }



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
