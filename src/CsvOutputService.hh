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
    , m_name_computed(false)
    , m_only_P0(true)
    , m_last_row(-1)
    , m_last_column(-1)
    {
      if(sbi.creationType() == ST_CaseOption) {
        m_path_name = options()->getFile();
      }
      else {
        m_path_name = "./output.csv";
      }
    }
  
  virtual ~CsvOutputService() {};

public:
  void init() override;
  void init(String name_csv) override;
  void init(String name_csv, String separator) override;

  Integer addRow(String name_row, bool fill_start) override;
  Integer addRow(String name_row, ConstArrayView<Real> elems) override;

  Integer addColumn(String name_column, bool fill_start) override;
  Integer addColumn(String name_column, ConstArrayView<Real> elems) override;
  

  bool addElemRow(Integer pos, Real elem) override;
  bool addElemRow(String name_row, Real elem, bool create_if_not_exist) override;
  bool addElemNextRow(Real elem) override;
  bool addElemsNextRow(ConstArrayView<Real> elems) override;

  bool addElemsRow(Integer pos, ConstArrayView<Real> elems) override;


  bool addElemColumn(Integer pos, Real elem) override;
  bool addElemColumn(String name_column, Real elem, bool create_if_not_exist) override;
  bool addElemNextColumn(Real elem) override;
  bool addElemsNextColumn(ConstArrayView<Real> elems) override;

  bool addElemsColumn(Integer pos, ConstArrayView<Real> elems) override;


  void print(bool only_P0) override;
  bool writeFile() override;
  bool writeFile(String path_file) override;

  bool editElem(Integer posX, Integer posY, Real elem) override;

  Integer addAverageColumn(String name_column) override;


private:
  void computePathName();

private:
  UniqueArray2<Real> m_values_csv;

  String m_path_name;
  bool m_name_computed;
  bool m_only_P0;

  String m_name_tab;
  String m_separator;

  UniqueArray<String> m_name_rows;
  UniqueArray<String> m_name_columns;

  // Tailles des lignes/colonnes 
  // (et pas le nombre d'éléments, on compte les "trous" ici).
  UniqueArray<Integer> m_size_rows;
  UniqueArray<Integer> m_size_columns;

  // Dernier élement ajouté.
  Integer m_last_row;
  Integer m_last_column;
};

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_SERVICE_CSVOUTPUT(CsvOutput, CsvOutputService);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/