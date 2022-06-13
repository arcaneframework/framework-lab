// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* SimpleTableOutputTesterService.hh                           (C) 2000-2022 */
/*                                                                           */
/* Service de test pour les services implémentant ISimpleTableOutput.        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <arcane/IParallelMng.h>
#include <arcane/ITimeLoopMng.h>
#include <arcane/ServiceBuilder.h>
#include "arcane/BasicUnitTest.h"
#include "ISimpleTableOutput.hh"


#include "SimpleTableOutputTesterService_axl.h"

using namespace Arcane;

class SimpleTableOutputTesterService
  : public ArcaneSimpleTableOutputTesterServiceObject
{
public:
 
  SimpleTableOutputTesterService(const ServiceBuildInfo& sbi)
    : ArcaneSimpleTableOutputTesterServiceObject(sbi) {
    }

  ~SimpleTableOutputTesterService() {}

  void setUpForClass() override;
  void setUp() override;


  void testAddRow1() override;
  void testAddRow2() override;
  void testAddRow3() override;
  void testAddRows1() override;

  void testAddColumn1() override;
  void testAddColumn2() override;
  void testAddColumn3() override;
  void testAddColumns1() override;

  void testAddElemRow1() override;
  void testAddElemRow2() override;
  void testAddElemSameRow1() override;

  void testAddElemsRow1() override;
  void testAddElemsRow2() override;
  void testAddElemsSameRow1() override;

  void testAddElemColumn1() override;
  void testAddElemColumn2() override;
  void testAddElemSameColumn1() override;

  void testAddElemsColumn1() override;
  void testAddElemsColumn2() override;
  void testAddElemsSameColumn1() override;

  void testAddElemSame1() override;

  void testEditElem1() override;
  void testEditElem2() override;

  void testGetElem1() override;
  void testGetElem2() override;
  
  void testGetSizeRow1() override;
  void testGetSizeRow2() override;
  
  void testGetSizeColumn1() override;
  void testGetSizeColumn2() override;

  void testGetPosRowColumn1() override;

  void testGetNumRowColumn1() override;

  void testAddRowSameColumn1() override;
  void testAddRowSameColumn2() override;
  void testAddRowSameColumn3() override;

  void testAddColumnSameRow1() override;
  void testAddColumnSameRow2() override;
  void testAddColumnSameRow3() override;

  void testEditElemUDLR1() override;

  void testEditElemDown1() override;
  void testEditElemRight1() override;


  void tearDown() override;
  void tearDownForClass() override;

private:
  template<class T>
  void ASSERT_EQUAL_ARRAY(UniqueArray<T> expected, UniqueArray<T> actual);
  
private:
  ISimpleTableOutput* ptrSTO;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_SERVICE_SIMPLETABLEOUTPUTTESTERSERVICE(SimpleTableOutputTesterService, SimpleTableOutputTesterService);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
