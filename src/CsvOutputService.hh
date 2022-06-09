// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* CsvOutputService.hh                                         (C) 2000-2022 */
/*                                                                           */
/* Service permettant de construire et de sortir un .csv.                    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef CSVOUTPUTSERVICE_HH
#define CSVOUTPUTSERVICE_HH

#include "ISimpleTableOutput.hh"
#include "arcane/BasicUnitTest.h"
#include "CsvOutput_axl.h"
#include <arcane/ServiceBuilder.h>
#include <arcane/ServiceFactory.h>

using namespace Arcane;

class CsvOutputService
: public ArcaneCsvOutputObject
{
public:
  CsvOutputService(const ServiceBuildInfo & sbi)
    : ArcaneCsvOutputObject(sbi)
    , m_size_rows(0)
    , m_size_columns(0)
    , m_name_rows(0)
    , m_name_columns(0)
    , m_path_computed(false)
    , m_name_tab_computed(false)
    , m_path_only_P0(true)
    , m_name_tab_only_P0(true)
    , m_last_row(-1)
    , m_last_column(-1)
    {
      if(sbi.creationType() == ST_CaseOption) {
        m_path = options()->getPath();
        withOption = true;
      }
      else {
        m_path = "./";
        withOption = false;
      }
      
    }
  
  virtual ~CsvOutputService() {};

public:
  void init() override;
  void init(String name_table) override;
  void init(String name_table, String separator) override;

  void clear() override;


  Integer addRow(String name_row) override;
  Integer addRow(String name_row, ConstArrayView<Real> elems) override;

  Integer addColumn(String name_column) override;
  Integer addColumn(String name_column, ConstArrayView<Real> elems) override;
  

  bool addElemRow(Integer pos, Real elem) override;
  bool addElemRow(String name_row, Real elem, bool create_if_not_exist) override;
  bool addElemSameRow(Real elem) override;
  
  bool addElemsRow(Integer pos, ConstArrayView<Real> elems) override;
  bool addElemsRow(String name_row, ConstArrayView<Real> elems, bool create_if_not_exist) override;
  bool addElemsSameRow(ConstArrayView<Real> elems) override;


  bool addElemColumn(Integer pos, Real elem) override;
  bool addElemColumn(String name_column, Real elem, bool create_if_not_exist) override;
  bool addElemSameColumn(Real elem) override;

  bool addElemsColumn(Integer pos, ConstArrayView<Real> elems) override;
  bool addElemsColumn(String name_column, ConstArrayView<Real> elems, bool create_if_not_exist) override;
  bool addElemsSameColumn(ConstArrayView<Real> elems) override;


  bool editElem(Integer posX, Integer posY, Real elem) override;
  bool editElem(String columnName, String rowName, Real elem) override;

  Real getElem(Integer posX, Integer posY) override;
  Real getElem(String columnName, String rowName) override;


  RealUniqueArray getRow(Integer pos) override;
  RealUniqueArray getColumn(Integer pos) override;
  
  RealUniqueArray getRow(String rowName) override;
  RealUniqueArray getColumn(String columnName) override;


  Integer getSizeRow(Integer pos) override;
  Integer getSizeColumn(Integer pos) override;
  
  Integer getSizeRow(String rowName) override;
  Integer getSizeColumn(String columnName) override;


  Integer getPosRow(String rowName) override;
  Integer getPosColumn(String columnName) override;

  Integer getNumRows() override;
  Integer getNumColumns() override;


  Integer addAverageColumn(String name_column) override;

  void print(bool only_P0) override;
  void print(Integer only_proc) override;
  bool writeFile(bool only_P0) override;
  bool writeFile(String path, bool only_P0) override;

public:
  void setUpForClass() override;
  void setUp() override;
  void testInitProcId() override;
  void testClear() override;
  void testComputeAt() override;
  void testComputeFinalPath() override;
  void tearDown() override;
  void tearDownForClass() override;


private:
  String _computeAt(String name, bool& only_P0);
  String _computeFinalPath();
  void _print(std::ostream& stream);

private:
  String m_path;
  bool m_path_computed;
  bool m_path_only_P0;

  String m_name_tab;
  bool m_name_tab_computed;
  bool m_name_tab_only_P0;

  String m_separator;

  UniqueArray2<Real> m_values_csv;

  UniqueArray<String> m_name_rows;
  UniqueArray<String> m_name_columns;

  // Tailles des lignes/colonnes 
  // (et pas le nombre d'éléments, on compte les "trous" ici).
  UniqueArray<Integer> m_size_rows;
  UniqueArray<Integer> m_size_columns;

  // Dernier élement ajouté.
  Integer m_last_row;
  Integer m_last_column;

  bool withOption;
};

#endif
