// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* ISimpleTableOutput.hh                                            (C) 2000-2022 */
/*                                                                           */
/* Interface pour simples services de sortie de tableaux de valeurs          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef ISIMPLEOUTPUT_HH
#define ISIMPLEOUTPUT_HH

#include <arcane/ItemTypes.h>
#include <optional>

using namespace Arcane;


class ISimpleTableOutput
{
public:
  ISimpleTableOutput() {};
  virtual ~ISimpleTableOutput() {};
  
public:
  virtual void init() = 0;
  virtual void init(String name_csv) = 0;
  virtual void init(String name_csv, String separator) = 0;

  virtual Integer addRow(String name_line, bool fill_start = false) = 0;
  virtual Integer addRow(String name_line, ConstArrayView<Real> elems) = 0;

  virtual Integer addColumn(String name_column, bool fill_start = false) = 0;
  virtual Integer addColumn(String name_column, ConstArrayView<Real> elems) = 0;

  virtual bool addElemRow(Integer pos, Real elem) = 0;
  virtual bool addElemRow(String name_row, Real elem, bool create_if_not_exist = true) = 0;
  virtual bool addElemNextRow(Real elem) = 0;

  virtual bool addElemsRow(Integer pos, ConstArrayView<Real> elems) = 0;
  virtual bool addElemsNextRow(ConstArrayView<Real> elems) = 0;

  virtual bool addElemColumn(Integer pos, Real elem) = 0;
  virtual bool addElemColumn(String name_column, Real elem, bool create_if_not_exist = true) = 0;
  virtual bool addElemNextColumn(Real elem) = 0;

  virtual bool addElemsColumn(Integer pos, ConstArrayView<Real> elems) = 0;
  virtual bool addElemsNextColumn(ConstArrayView<Real> elems) = 0;


  virtual bool editElem(Integer posX, Integer posY, Real elem) = 0;
  
  virtual Integer addAverageColumn(String name_column) = 0;

  virtual void print(bool only_P0 = true) = 0;
  virtual bool writeFile() = 0;
  virtual bool writeFile(String path_file) = 0;
};

#endif