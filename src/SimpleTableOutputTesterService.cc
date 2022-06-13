// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* SimpleTableOutputTesterService.cc                           (C) 2000-2022 */
/*                                                                           */
/* Service de test pour les services implémentant ISimpleTableOutput.        */
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
  ptrSTO->clear();
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
testAddRows1()
{
  ptrSTO->addRows(StringUniqueArray{"L1", "L2", "L3", "L4"});
  ASSERT_EQUAL(4, ptrSTO->getNumRows());
  ASSERT_EQUAL(0, ptrSTO->getNumColumns());
  ptrSTO->addRows(StringUniqueArray{"L5"});
  ASSERT_EQUAL(5, ptrSTO->getNumRows());
  ASSERT_EQUAL(0, ptrSTO->getNumColumns());
  ptrSTO->addRows(StringUniqueArray{});
  ASSERT_EQUAL(5, ptrSTO->getNumRows());
  ASSERT_EQUAL(0, ptrSTO->getNumColumns());
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
testAddColumns1()
{
  ptrSTO->addColumns(StringUniqueArray{"C1", "C2", "C3", "C4"});
  ASSERT_EQUAL(4, ptrSTO->getNumColumns());
  ASSERT_EQUAL(0, ptrSTO->getNumRows());
  ptrSTO->addColumns(StringUniqueArray{"C5"});
  ASSERT_EQUAL(5, ptrSTO->getNumColumns());
  ASSERT_EQUAL(0, ptrSTO->getNumRows());
  ptrSTO->addColumns(StringUniqueArray{});
  ASSERT_EQUAL(5, ptrSTO->getNumColumns());
  ASSERT_EQUAL(0, ptrSTO->getNumRows());
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

void SimpleTableOutputTesterService::
testAddElemColumn1()
{
  Integer pos = ptrSTO->addColumn("Ma colonne");
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");

  RealUniqueArray result = {1, 2, 3};

  ASSERT_TRUE(ptrSTO->addElemColumn(pos, 1));
  ASSERT_TRUE(ptrSTO->addElemColumn(pos, 2));
  ASSERT_TRUE(ptrSTO->addElemColumn(pos, 3));
  ASSERT_FALSE(ptrSTO->addElemColumn(pos, 4));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getColumn("Ma colonne"));
}

void SimpleTableOutputTesterService::
testAddElemColumn2()
{
  ptrSTO->addColumn("Ma colonne vide");
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");

  RealUniqueArray result = {1, 2, 3};

  ASSERT_FALSE(ptrSTO->addElemColumn("Ma colonne", 1, false));
  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne", 1));
  Integer pos = ptrSTO->getPosColumn("Ma colonne");
  ASSERT_TRUE(ptrSTO->addElemColumn(pos, 2));
  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne", 3));
  ASSERT_FALSE(ptrSTO->addElemColumn("Ma colonne", 4));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getColumn("Ma colonne"));
}

void SimpleTableOutputTesterService::
testAddElemSameColumn1()
{
  ptrSTO->addColumn("Ma colonne vide");
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");

  RealUniqueArray result = {1, 2, 3, 4};

  ASSERT_FALSE(ptrSTO->addElemColumn("Ma colonne", 1, false));
  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne", 1));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(2));

  Integer pos = ptrSTO->getPosColumn("Ma colonne");
  ASSERT_TRUE(ptrSTO->addElemColumn(pos, 3));

  ptrSTO->addRow("Ma ligne 4");
  ASSERT_TRUE(ptrSTO->addElemSameColumn(4));

  ASSERT_FALSE(ptrSTO->addElemSameColumn(5));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getColumn("Ma colonne"));
}

void SimpleTableOutputTesterService::
testAddElemsColumn1()
{
  Integer pos = ptrSTO->addColumn("Ma colonne");
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");
  RealUniqueArray test = {1, 2, 3};
  RealUniqueArray result1 = {1, 2, 3};
  RealUniqueArray result2 = {1, 2, 3, 1, 2};
  RealUniqueArray result3 = {1, 2, 3, 1, 2, 1};
  ASSERT_TRUE(ptrSTO->addElemsColumn(pos, test));
  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getColumn("Ma colonne"));

  ptrSTO->addRow("Ma ligne 4");
  ptrSTO->addRow("Ma ligne 5");

  ASSERT_FALSE(ptrSTO->addElemsColumn(pos, test));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getColumn("Ma colonne"));

  ptrSTO->addRow("Ma ligne 6");

  ASSERT_FALSE(ptrSTO->addElemsColumn(pos, test));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getColumn("Ma colonne"));
}

void SimpleTableOutputTesterService::
testAddElemsColumn2()
{
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");
  RealUniqueArray test = {1, 2, 3};
  RealUniqueArray result1 = {1, 2, 3};
  RealUniqueArray result2 = {1, 2, 3, 1, 2};
  RealUniqueArray result3 = {1, 2, 3, 1, 2, 1};

  ASSERT_TRUE(ptrSTO->addElemsColumn("Ma colonne", test));
  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getColumn("Ma colonne"));

  ptrSTO->addRow("Ma ligne 4");
  ptrSTO->addRow("Ma ligne 5");

  ASSERT_FALSE(ptrSTO->addElemsColumn("Ma colonne", test));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getColumn("Ma colonne"));

  ptrSTO->addRow("Ma ligne 6");

  Integer pos = ptrSTO->getPosColumn("Ma colonne");

  ASSERT_FALSE(ptrSTO->addElemsColumn(pos, test));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getColumn("Ma colonne"));
}


void SimpleTableOutputTesterService::
testAddElemsSameColumn1()
{
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");
  RealUniqueArray test = {1, 2, 3};
  RealUniqueArray result1 = {1, 2, 3};
  RealUniqueArray result2 = {1, 2, 3, 1, 2};
  RealUniqueArray result3 = {1, 2, 3, 1, 2, 1};

  ASSERT_TRUE(ptrSTO->addElemsColumn("Ma colonne", test));
  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getColumn("Ma colonne"));

  ptrSTO->addRow("Ma ligne 4");
  ptrSTO->addRow("Ma ligne 5");

  ASSERT_FALSE(ptrSTO->addElemsSameColumn(test));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getColumn("Ma colonne"));

  ptrSTO->addRow("Ma ligne 6");

  Integer pos = ptrSTO->getPosColumn("Ma colonne");

  ASSERT_FALSE(ptrSTO->addElemsColumn(pos, test));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getColumn("Ma colonne"));
}

void SimpleTableOutputTesterService::
testAddElemSame1()
{
  ptrSTO->addRows(StringUniqueArray{                "Ma ligne 1", "Ma ligne 2", "Ma ligne 3", "Ma ligne 4", "Ma ligne 5"});
  ptrSTO->addColumns(StringUniqueArray{ "Ma colonne 1", 
                                        "Ma colonne 2", 
                                        "Ma colonne 3", 
                                        "Ma colonne 4", 
                                        "Ma colonne 5"
  });

  RealUniqueArray result1 = {1};
  RealUniqueArray result2 = {2, 3};
  RealUniqueArray result3 = {0, 4, 5};
  RealUniqueArray result4 = {0, 0, 6, 7};
  RealUniqueArray result5 = {0, 0, 0, 8, 9};

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 1", 1));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(2));
  ASSERT_TRUE(ptrSTO->addElemSameRow(3));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(4));
  ASSERT_TRUE(ptrSTO->addElemSameRow(5));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(6));
  ASSERT_TRUE(ptrSTO->addElemSameRow(7));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(8));
  ASSERT_TRUE(ptrSTO->addElemSameRow(9));
  ASSERT_FALSE(ptrSTO->addElemSameColumn(10));

  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getRow("Ma ligne 1"));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getRow("Ma ligne 2"));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getRow("Ma ligne 3"));
  ASSERT_EQUAL_ARRAY(result4, ptrSTO->getRow("Ma ligne 4"));
  ASSERT_EQUAL_ARRAY(result5, ptrSTO->getRow("Ma ligne 5"));
}


void SimpleTableOutputTesterService::
testEditElem1()
{
  Integer posX0 = ptrSTO->addColumn("Ma colonne 1");
  Integer posX1 = ptrSTO->addColumn("Ma colonne 2");
  Integer posX2 = ptrSTO->addColumn("Ma colonne 3");

  Integer posY0 = ptrSTO->addRow("Ma ligne 1", RealUniqueArray{1, 2, 3});
  Integer posY1 = ptrSTO->addRow("Ma ligne 2", RealUniqueArray{4, 5, 6});
  Integer posY2 = ptrSTO->addRow("Ma ligne 3", RealUniqueArray{7, 8, 9});

  ASSERT_TRUE(ptrSTO->editElem(posX2, posY0, 10));
  ASSERT_TRUE(ptrSTO->editElem(posX1, posY2, 11));

  RealUniqueArray result1 = {1, 2, 10};
  RealUniqueArray result2 = {4, 5, 6};
  RealUniqueArray result3 = {7, 11, 9};

  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getRow("Ma ligne 1"));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getRow("Ma ligne 2"));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getRow("Ma ligne 3"));
}

void SimpleTableOutputTesterService::
testEditElem2()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  ptrSTO->addRow("Ma ligne 1", RealUniqueArray{1, 2, 3});
  ptrSTO->addRow("Ma ligne 2", RealUniqueArray{4, 5, 6});
  ptrSTO->addRow("Ma ligne 3", RealUniqueArray{7, 8, 9});

  ASSERT_TRUE(ptrSTO->editElem("Ma colonne 3", "Ma ligne 1", 10));
  ASSERT_TRUE(ptrSTO->editElem("Ma colonne 2", "Ma ligne 3", 11));
  ASSERT_FALSE(ptrSTO->editElem("Ma colonne 4", "Ma ligne 1", 11));
  ASSERT_FALSE(ptrSTO->editElem("Ma colonne 1", "Ma ligne 4", 11));
  ASSERT_FALSE(ptrSTO->editElem("Ma colonne 4", "Ma ligne 4", 11));

  RealUniqueArray result1 = {1, 2, 10};
  RealUniqueArray result2 = {4, 5, 6};
  RealUniqueArray result3 = {7, 11, 9};

  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getRow("Ma ligne 1"));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getRow("Ma ligne 2"));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getRow("Ma ligne 3"));
}

void SimpleTableOutputTesterService::
testGetElem1()
{
  Integer posX0 = ptrSTO->addColumn("Ma colonne 1");
  Integer posX1 = ptrSTO->addColumn("Ma colonne 2");
  Integer posX2 = ptrSTO->addColumn("Ma colonne 3");

  Integer posY0 = ptrSTO->addRow("Ma ligne 1", RealUniqueArray{1, 2, 3});
  Integer posY1 = ptrSTO->addRow("Ma ligne 2", RealUniqueArray{4, 5, 6});
  Integer posY2 = ptrSTO->addRow("Ma ligne 3", RealUniqueArray{7, 8, 9});

  ASSERT_EQUAL(5., ptrSTO->getElem(posX1, posY1));
  ASSERT_EQUAL(7., ptrSTO->getElem(posX0, posY2));
  ASSERT_EQUAL(8., ptrSTO->getElem(posX1, posY2));
}

void SimpleTableOutputTesterService::
testGetElem2()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  ptrSTO->addRow("Ma ligne 1", RealUniqueArray{1, 2, 3});
  ptrSTO->addRow("Ma ligne 2", RealUniqueArray{4, 5, 6});
  ptrSTO->addRow("Ma ligne 3", RealUniqueArray{7, 8, 9});

  ASSERT_EQUAL(5., ptrSTO->getElem("Ma colonne 2", "Ma ligne 2"));
  ASSERT_EQUAL(7., ptrSTO->getElem("Ma colonne 1", "Ma ligne 3"));
  ASSERT_EQUAL(8., ptrSTO->getElem("Ma colonne 2", "Ma ligne 3"));
}

void SimpleTableOutputTesterService::
testGetSizeRow1()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  Integer posY0 = ptrSTO->addRow("Ma ligne 1", RealUniqueArray{1, 2, 3});
  Integer posY1 = ptrSTO->addRow("Ma ligne 2", RealUniqueArray{4});
  Integer posY2 = ptrSTO->addRow("Ma ligne 3", RealUniqueArray{7, 8});

  ASSERT_EQUAL(3, ptrSTO->getSizeRow(posY0));
  ASSERT_EQUAL(1, ptrSTO->getSizeRow(posY1));
  ASSERT_EQUAL(2, ptrSTO->getSizeRow(posY2));

  ptrSTO->addColumn("Ma colonne 4", RealUniqueArray{9, 10, 11});

  ASSERT_EQUAL(4, ptrSTO->getSizeRow(posY0));
  ASSERT_EQUAL(4, ptrSTO->getSizeRow(posY1));
  ASSERT_EQUAL(4, ptrSTO->getSizeRow(posY2));

}

void SimpleTableOutputTesterService::
testGetSizeRow2()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  Integer posY0 = ptrSTO->addRow("Ma ligne 1", RealUniqueArray{1, 2, 3});
  Integer posY1 = ptrSTO->addRow("Ma ligne 2", RealUniqueArray{4});
  Integer posY2 = ptrSTO->addRow("Ma ligne 3", RealUniqueArray{7, 8});

  ASSERT_EQUAL(3, ptrSTO->getSizeRow("Ma ligne 1"));
  ASSERT_EQUAL(1, ptrSTO->getSizeRow("Ma ligne 2"));
  ASSERT_EQUAL(2, ptrSTO->getSizeRow("Ma ligne 3"));
  ASSERT_EQUAL(0, ptrSTO->getSizeRow("Ma ligne 4"));

  ptrSTO->addColumn("Ma colonne 4", RealUniqueArray{9, 10, 11});

  ASSERT_EQUAL(4, ptrSTO->getSizeRow("Ma ligne 1"));
  ASSERT_EQUAL(4, ptrSTO->getSizeRow("Ma ligne 2"));
  ASSERT_EQUAL(4, ptrSTO->getSizeRow("Ma ligne 3"));
  ASSERT_EQUAL(0, ptrSTO->getSizeRow("Ma ligne 4"));
}

void SimpleTableOutputTesterService::
testGetSizeColumn1()
{
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");

  Integer posX0 = ptrSTO->addColumn("Ma colonne 1", RealUniqueArray{1, 2, 3});
  Integer posX1 = ptrSTO->addColumn("Ma colonne 2", RealUniqueArray{4});
  Integer posX2 = ptrSTO->addColumn("Ma colonne 3", RealUniqueArray{7, 8});

  ASSERT_EQUAL(3, ptrSTO->getSizeColumn(posX0));
  ASSERT_EQUAL(1, ptrSTO->getSizeColumn(posX1));
  ASSERT_EQUAL(2, ptrSTO->getSizeColumn(posX2));

  ptrSTO->addRow("Ma ligne 4", RealUniqueArray{9, 10, 11});

  ASSERT_EQUAL(4, ptrSTO->getSizeColumn(posX0));
  ASSERT_EQUAL(4, ptrSTO->getSizeColumn(posX1));
  ASSERT_EQUAL(4, ptrSTO->getSizeColumn(posX2));

}

void SimpleTableOutputTesterService::
testGetSizeColumn2()
{
  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");

  Integer posX0 = ptrSTO->addColumn("Ma colonne 1", RealUniqueArray{1, 2, 3});
  Integer posX1 = ptrSTO->addColumn("Ma colonne 2", RealUniqueArray{4});
  Integer posX2 = ptrSTO->addColumn("Ma colonne 3", RealUniqueArray{7, 8});

  ASSERT_EQUAL(3, ptrSTO->getSizeColumn("Ma colonne 1"));
  ASSERT_EQUAL(1, ptrSTO->getSizeColumn("Ma colonne 2"));
  ASSERT_EQUAL(2, ptrSTO->getSizeColumn("Ma colonne 3"));
  ASSERT_EQUAL(0, ptrSTO->getSizeColumn("Ma colonne 4"));

  ptrSTO->addRow("Ma ligne 4", RealUniqueArray{9, 10, 11});

  ASSERT_EQUAL(4, ptrSTO->getSizeColumn("Ma colonne 1"));
  ASSERT_EQUAL(4, ptrSTO->getSizeColumn("Ma colonne 2"));
  ASSERT_EQUAL(4, ptrSTO->getSizeColumn("Ma colonne 3"));
  ASSERT_EQUAL(0, ptrSTO->getSizeColumn("Ma colonne 4"));
}

void SimpleTableOutputTesterService::
testGetPosRowColumn1()
{
  Integer posX0 = ptrSTO->addColumn("Ma colonne 1");
  Integer posX1 = ptrSTO->addColumn("Ma colonne 2");
  Integer posX2 = ptrSTO->addColumn("Ma colonne 3");

  Integer posY0 = ptrSTO->addRow("Ma ligne 1", RealUniqueArray{1, 2, 3});
  Integer posY1 = ptrSTO->addRow("Ma ligne 2", RealUniqueArray{4, 5, 6});
  Integer posY2 = ptrSTO->addRow("Ma ligne 3", RealUniqueArray{7, 8, 9});

  ASSERT_EQUAL(posX0, ptrSTO->getPosColumn("Ma colonne 1"));
  ASSERT_EQUAL(posX1, ptrSTO->getPosColumn("Ma colonne 2"));
  ASSERT_EQUAL(posX2, ptrSTO->getPosColumn("Ma colonne 3"));

  ASSERT_EQUAL(posY0, ptrSTO->getPosRow("Ma ligne 1"));
  ASSERT_EQUAL(posY1, ptrSTO->getPosRow("Ma ligne 2"));
  ASSERT_EQUAL(posY2, ptrSTO->getPosRow("Ma ligne 3"));
}

void SimpleTableOutputTesterService::
testGetNumRowColumn1()
{
  Integer posX0 = ptrSTO->addColumn("Ma colonne 1");
  Integer posX1 = ptrSTO->addColumn("Ma colonne 2");
  Integer posX2 = ptrSTO->addColumn("Ma colonne 3");

  Integer posY0 = ptrSTO->addRow("Ma ligne 1");
  Integer posY1 = ptrSTO->addRow("Ma ligne 2");

  ASSERT_EQUAL(3, ptrSTO->getNumColumns());
  ASSERT_EQUAL(2, ptrSTO->getNumRows());
}


void SimpleTableOutputTesterService::
testAddRowSameColumn1()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");

  RealUniqueArray result1 = {1, 2, 3};
  RealUniqueArray result2 = {4, 5, 6};
  RealUniqueArray result3 = {7, 8, 9};

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 1", 1));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(4));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(7));

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 1", 2));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(5));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(8));

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 1", 3));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(6));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(9));

  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getRow("Ma ligne 1"));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getRow("Ma ligne 2"));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getRow("Ma ligne 3"));
}

void SimpleTableOutputTesterService::
testAddRowSameColumn2()
{
  ptrSTO->addColumn("Ma colonne 1");

  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");
  ptrSTO->addRow("Ma ligne 4");
  ptrSTO->addRow("Ma ligne 5");
  ptrSTO->addRow("Ma ligne 6");
  ptrSTO->addRow("Ma ligne 7");
  ptrSTO->addRow("Ma ligne 8");
  ptrSTO->addRow("Ma ligne 9");

  RealUniqueArray result = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 1", 1));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(2));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(3));

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 4", 4));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(5));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(6));

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 7", 7));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(8));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(9));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getColumn("Ma colonne 1"));
}

void SimpleTableOutputTesterService::
testAddRowSameColumn3()
{
  ptrSTO->addColumn("Ma colonne 1");

  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");
  ptrSTO->addRow("Ma ligne 4");
  ptrSTO->addRow("Ma ligne 5");
  ptrSTO->addRow("Ma ligne 6");
  ptrSTO->addRow("Ma ligne 7");
  ptrSTO->addRow("Ma ligne 8");
  ptrSTO->addRow("Ma ligne 9");

  RealUniqueArray result = {1, 2, 3, 4, 0, 0, 7, 8, 9};

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 1", 1));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(2));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(3));

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 7", 7));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(8));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(9));

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 4", 4));
  ASSERT_FALSE(ptrSTO->addElemSameColumn(5));
  ASSERT_FALSE(ptrSTO->addElemSameColumn(6));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getColumn("Ma colonne 1"));
}

void SimpleTableOutputTesterService::
testAddColumnSameRow1()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");

  RealUniqueArray result1 = {1, 2, 3};
  RealUniqueArray result2 = {4, 5, 6};
  RealUniqueArray result3 = {7, 8, 9};

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 1", 1));
  ASSERT_TRUE(ptrSTO->addElemSameRow(2));
  ASSERT_TRUE(ptrSTO->addElemSameRow(3));

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 1", 4));
  ASSERT_TRUE(ptrSTO->addElemSameRow(5));
  ASSERT_TRUE(ptrSTO->addElemSameRow(6));

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 1", 7));
  ASSERT_TRUE(ptrSTO->addElemSameRow(8));
  ASSERT_TRUE(ptrSTO->addElemSameRow(9));

  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getRow("Ma ligne 1"));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getRow("Ma ligne 2"));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getRow("Ma ligne 3"));
}

void SimpleTableOutputTesterService::
testAddColumnSameRow2()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");
  ptrSTO->addColumn("Ma colonne 4");
  ptrSTO->addColumn("Ma colonne 5");
  ptrSTO->addColumn("Ma colonne 6");
  ptrSTO->addColumn("Ma colonne 7");
  ptrSTO->addColumn("Ma colonne 8");
  ptrSTO->addColumn("Ma colonne 9");

  ptrSTO->addRow("Ma ligne 1");

  RealUniqueArray result = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 1", 1));
  ASSERT_TRUE(ptrSTO->addElemSameRow(2));
  ASSERT_TRUE(ptrSTO->addElemSameRow(3));

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 4", 4));
  ASSERT_TRUE(ptrSTO->addElemSameRow(5));
  ASSERT_TRUE(ptrSTO->addElemSameRow(6));

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 7", 7));
  ASSERT_TRUE(ptrSTO->addElemSameRow(8));
  ASSERT_TRUE(ptrSTO->addElemSameRow(9));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getRow("Ma ligne 1"));
}

void SimpleTableOutputTesterService::
testAddColumnSameRow3()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");
  ptrSTO->addColumn("Ma colonne 4");
  ptrSTO->addColumn("Ma colonne 5");
  ptrSTO->addColumn("Ma colonne 6");
  ptrSTO->addColumn("Ma colonne 7");
  ptrSTO->addColumn("Ma colonne 8");
  ptrSTO->addColumn("Ma colonne 9");

  ptrSTO->addRow("Ma ligne 1");

  RealUniqueArray result = {1, 2, 3, 4, 0, 0, 7, 8, 9};

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 1", 1));
  ASSERT_TRUE(ptrSTO->addElemSameRow(2));
  ASSERT_TRUE(ptrSTO->addElemSameRow(3));

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 7", 7));
  ASSERT_TRUE(ptrSTO->addElemSameRow(8));
  ASSERT_TRUE(ptrSTO->addElemSameRow(9));

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 4", 4));
  ASSERT_FALSE(ptrSTO->addElemSameRow(5));
  ASSERT_FALSE(ptrSTO->addElemSameRow(6));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getRow("Ma ligne 1"));
}


void SimpleTableOutputTesterService::
testEditElemUDLR1()
{
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");

  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");

  RealUniqueArray result1 = {1, 2, 3};
  RealUniqueArray result2 = {4, 5, 6};
  RealUniqueArray result3 = {7, 8, 9};

  ASSERT_TRUE(ptrSTO->editElem("Ma colonne 2", "Ma ligne 2", 5));
  ASSERT_TRUE(ptrSTO->editElemDown(8));
  ASSERT_FALSE(ptrSTO->editElemDown(99));
  ASSERT_TRUE(ptrSTO->editElemLeft(7));
  ASSERT_FALSE(ptrSTO->editElemLeft(99));
  ASSERT_TRUE(ptrSTO->editElemUp(4));
  ASSERT_TRUE(ptrSTO->editElemUp(1));
  ASSERT_FALSE(ptrSTO->editElemUp(99));
  ASSERT_TRUE(ptrSTO->editElemRight(2));
  ASSERT_TRUE(ptrSTO->editElemRight(3));
  ASSERT_FALSE(ptrSTO->editElemRight(99));
  ASSERT_TRUE(ptrSTO->editElemDown(6));
  ASSERT_TRUE(ptrSTO->editElemDown(9));
  ASSERT_FALSE(ptrSTO->editElemDown(99));

  ASSERT_EQUAL_ARRAY(result1, ptrSTO->getRow("Ma ligne 1"));
  ASSERT_EQUAL_ARRAY(result2, ptrSTO->getRow("Ma ligne 2"));
  ASSERT_EQUAL_ARRAY(result3, ptrSTO->getRow("Ma ligne 3"));
}

void SimpleTableOutputTesterService::
testEditElemDown1()
{
  // Voir testAddRowSameColumn3()
  ptrSTO->addColumn("Ma colonne 1");

  ptrSTO->addRow("Ma ligne 1");
  ptrSTO->addRow("Ma ligne 2");
  ptrSTO->addRow("Ma ligne 3");
  ptrSTO->addRow("Ma ligne 4");
  ptrSTO->addRow("Ma ligne 5");
  ptrSTO->addRow("Ma ligne 6");
  ptrSTO->addRow("Ma ligne 7");
  ptrSTO->addRow("Ma ligne 8");
  ptrSTO->addRow("Ma ligne 9");

  RealUniqueArray result = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 1", 1));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(2));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(3));

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 7", 7));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(8));
  ASSERT_TRUE(ptrSTO->addElemSameColumn(9));

  ASSERT_TRUE(ptrSTO->addElemRow("Ma ligne 4", 4));
  ASSERT_TRUE(ptrSTO->editElemDown(5));
  ASSERT_TRUE(ptrSTO->editElemDown(6));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getColumn("Ma colonne 1"));
}

void SimpleTableOutputTesterService::
testEditElemRight1()
{
  // Voir testAddColumnSameRow3()
  ptrSTO->addColumn("Ma colonne 1");
  ptrSTO->addColumn("Ma colonne 2");
  ptrSTO->addColumn("Ma colonne 3");
  ptrSTO->addColumn("Ma colonne 4");
  ptrSTO->addColumn("Ma colonne 5");
  ptrSTO->addColumn("Ma colonne 6");
  ptrSTO->addColumn("Ma colonne 7");
  ptrSTO->addColumn("Ma colonne 8");
  ptrSTO->addColumn("Ma colonne 9");

  ptrSTO->addRow("Ma ligne 1");

  RealUniqueArray result = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 1", 1));
  ASSERT_TRUE(ptrSTO->addElemSameRow(2));
  ASSERT_TRUE(ptrSTO->addElemSameRow(3));

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 7", 7));
  ASSERT_TRUE(ptrSTO->addElemSameRow(8));
  ASSERT_TRUE(ptrSTO->addElemSameRow(9));

  ASSERT_TRUE(ptrSTO->addElemColumn("Ma colonne 4", 4));
  ASSERT_TRUE(ptrSTO->editElemRight(5));
  ASSERT_TRUE(ptrSTO->editElemRight(6));

  ASSERT_EQUAL_ARRAY(result, ptrSTO->getRow("Ma ligne 1"));
}


void SimpleTableOutputTesterService::
tearDown()
{
}


void SimpleTableOutputTesterService::
tearDownForClass()
{

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
