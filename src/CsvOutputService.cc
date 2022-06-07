// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* CsvOutputService.cc                                         (C) 2000-2022 */
/*                                                                           */
/* Service permettant de construire et de sortir un .csv.                    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "CsvOutputService.hh"
#include <arcane/IMesh.h>
#include <arcane/IParallelMng.h>
#include <optional>

using namespace Arcane;

void CsvOutputService::
init()
{
  m_separator = ";";
  m_name_tab = _computeAt("Table_%proc_id%", m_name_tab_only_P0);
  m_name_tab_computed = true;
}

void CsvOutputService::
init(String name_table)
{
  m_separator = ";";
  m_name_tab = _computeAt(name_table, m_name_tab_only_P0);
  m_name_tab_computed = true;
}

void CsvOutputService::
init(String name_table, String separator_csv)
{
  m_separator = separator_csv;
  m_name_tab = _computeAt(name_table, m_name_tab_only_P0);
  m_name_tab_computed = true;
}


Integer CsvOutputService::
addRow(String name_row, bool fill_start)
{
  Integer pos = m_values_csv.dim1Size();
  m_values_csv.resize(pos+1);

  m_name_rows.add(name_row);
  if(fill_start) {
    m_size_rows.add(m_values_csv.dim2Size());
  }
  else{
    m_size_rows.add(0);
  }

  m_last_row = pos;

  return pos;
}

Integer CsvOutputService::
addRow(String name_row, ConstArrayView<Real> elems)
{
  Integer pos = m_values_csv.dim1Size();
  m_values_csv.resize(pos+1);
    
  m_name_rows.add(name_row);
  m_size_rows.add(0);

  addElemsRow(pos, elems);

  return pos;
}

bool CsvOutputService::
addElemRow(Integer pos, Real elem)
{
  if(pos < 0 || pos >= m_values_csv.dim1Size()) return false;

  ArrayView<Real> view = m_values_csv[pos];
  Integer size_row = m_size_rows[pos];

  if(view.size() < size_row + 1) return false;

  view[size_row] = elem;

  m_last_row = pos;
  m_last_column = size_row;

  m_size_rows[pos]++;
  // Il peut y avoir des élements sur la ligne d'après à la même colonne.
  // Exemple : addElemRow(pos=L01, elem=NEW):
  // aaa|C00|C01|C02
  // L00|123|456|789
  // L01|147|NEW|
  // L02|159|753|852
  // Il y a 753 donc la taille de la colonne reste égale à 3.
  m_size_columns[size_row] = std::max(pos+1, m_size_columns[size_row]);

  return true;
}

bool CsvOutputService::
addElemRow(String name_row, Real elem, bool create_if_not_exist)
{
  std::optional<Integer> pos = m_name_rows.span().findFirst(name_row);

  if(pos)                       return addElemRow(pos.value(), elem);
  else if(create_if_not_exist)  return addElemRow(addRow(name_row, false), elem);
  else                          return false;
}

bool CsvOutputService::
addElemNextRow(Real elem)
{
  if(m_last_row == -1 || m_last_column == -1) return false;
  return addElemRow(m_last_row+1, elem);
}

bool CsvOutputService::
addElemsNextRow(ConstArrayView<Real> elems)
{
  if(m_last_row == -1 || m_last_column == -1) return false;
  return addElemsRow(m_last_row+1, elems);
}

bool CsvOutputService::
addElemsRow(Integer pos, ConstArrayView<Real> elems)
{
  if(pos < 0 || pos >= m_values_csv.dim1Size()) return false;

  ArrayView<Real> view = m_values_csv[pos];
  Integer size_row = m_size_rows[pos];
  Integer min_size = (elems.size() < view.size()-size_row ? elems.size() : view.size()-size_row);

  for(Integer i = 0; i < min_size; i++) {
    view[i+size_row] = elems[i];
    m_size_columns[i+size_row] = std::max(pos+1, m_size_columns[i+size_row]);
  }
  m_size_rows[pos] += min_size;

  m_last_row = pos;
  m_last_column = m_size_rows[pos]-1;

  return true;
}


Integer CsvOutputService::
addColumn(String name_column, bool fill_start)
{
  Integer pos = m_values_csv.dim2Size();
  m_values_csv.resize(m_values_csv.dim1Size(), pos+1);

  m_name_columns.add(name_column);
  if(fill_start) {
    m_size_columns.add(m_values_csv.dim1Size());
  }
  else{
    m_size_columns.add(0);
  }

  m_last_column = pos;

  return pos;
}

Integer CsvOutputService::
addColumn(String name_column, ConstArrayView<Real> elems)
{
  Integer pos = m_values_csv.dim2Size();
  m_values_csv.resize(m_values_csv.dim1Size(), pos+1);

  m_name_columns.add(name_column);
  m_size_columns.add(0);

  addElemsColumn(pos, elems);

  return pos;
}

bool CsvOutputService::
addElemColumn(Integer pos, Real elem)
{
  if(pos < 0 || pos >= m_values_csv.dim2Size()) return false;

  Integer size_column = m_size_columns[pos];

  if(m_values_csv.dim2Size() < size_column + 1) return false;

  m_values_csv[size_column][pos] = elem;

  m_last_column = pos;
  m_last_row = size_column;

  m_size_columns[pos]++;
  m_size_rows[size_column] = std::max(pos+1, m_size_rows[size_column]);

  return true;
}

bool CsvOutputService::
addElemColumn(String name_column, Real elem, bool create_if_not_exist)
{
  std::optional<Integer> pos = m_name_columns.span().findFirst(name_column);

  if(pos)                       return addElemColumn(pos.value(), elem);
  else if(create_if_not_exist)  return addElemColumn(addColumn(name_column, false), elem);
  else                          return false;
}

bool CsvOutputService::
addElemsColumn(Integer pos, ConstArrayView<Real> elems)
{
  if(pos < 0 || pos >= m_values_csv.dim2Size()) return false;

  Integer size_column = m_size_columns[pos];
  Integer min_size = (elems.size() < m_values_csv.dim1Size()-size_column ? elems.size() : m_values_csv.dim1Size()-size_column);

  for(Integer i = 0; i < min_size; i++) {
    m_values_csv[i+size_column][pos] = elems[i];
    m_size_rows[i+size_column] = std::max(pos+1, m_size_rows[i+size_column]);
    info() << m_size_rows[i+size_column] << " " << pos;
  }
  m_size_columns[pos] += min_size;

  m_last_column = pos;
  m_last_row = m_size_columns[pos] - 1;

  return true;
}

bool CsvOutputService::
addElemNextColumn(Real elem)
{
  if(m_last_row == -1 || m_last_column == -1) return false;
  return addElemColumn(m_last_column+1, elem);
}

bool CsvOutputService::
addElemsNextColumn(ConstArrayView<Real> elems)
{
  if(m_last_row == -1 || m_last_column == -1) return false;
  return addElemsColumn(m_last_column+1, elems);
}


String CsvOutputService::
_computeFinalPath()
{
  if(!m_path_computed) {
    m_path = _computeAt(m_path, m_path_only_P0);
    m_path_computed = true;
  }
  if(!m_name_tab_computed) {
    m_name_tab = _computeAt(m_name_tab, m_name_tab_only_P0);
    m_name_tab_computed = true;
  }
  return m_path + m_name_tab + ".csv";
}

String CsvOutputService::
_computeAt(String name, bool& only_P0)
{
  // On découpe la string là où se trouve les @.
  StringUniqueArray string_splited;
  name.split(string_splited, '@');

  // On traite les mots entre les "@".
  if (string_splited.size() > 1)
  {
    // On recherche "proc_id" dans le tableau (donc @proc_id@ dans le nom).
    std::optional<Integer> proc_id = string_splited.span().findFirst("proc_id");
    // On remplace "@proc_id@" par l'id du proc.
    if (proc_id)
    {
      string_splited[proc_id.value()] = String::fromNumber(mesh()->parallelMng()->commRank());
      only_P0 = false;
    }
    // Il n'y a que P0 qui write.
    else
    {
      only_P0 = true;
    }
  }

  // On recombine la chaine.
  String combined = "";
  for (String str : string_splited)
  {
    combined = combined + str;
  }
  return combined;
}


void CsvOutputService::
print(bool only_P0)
{
  if(only_P0 && mesh()->parallelMng()->commRank() != 0) return;
  pinfo() << "P" << mesh()->parallelMng()->commRank() << " - Ecriture du .csv dans la sortie standard :";
  _print(std::cout);
  pinfo() << "P" << mesh()->parallelMng()->commRank() << " - Fin écriture .csv";
}

void CsvOutputService::
print(Integer only_proc)
{
  if(mesh()->parallelMng()->commRank() != only_proc) return;
  pinfo() << "P" << mesh()->parallelMng()->commRank() << " - Ecriture du .csv dans la sortie standard :";
  _print(std::cout);
  pinfo() << "P" << mesh()->parallelMng()->commRank() << " - Fin écriture .csv";
}

void CsvOutputService::
_print(std::ostream& stream)
{
  stream << m_name_tab << m_separator;

  for(Integer j = 0; j < m_name_columns.size(); j++) {
    stream << m_name_columns[j] << m_separator;
  }
  stream << std::endl;

  for(Integer i = 0; i < m_values_csv.dim1Size(); i++) {
    if(m_name_rows.size() > i) stream << m_name_rows[i];
    stream << m_separator;
    ConstArrayView<Real> view = m_values_csv[i];
    for(Integer j = 0; j < m_values_csv.dim2Size(); j++) {
      stream << view[j] << m_separator;
    }
    stream << std::endl;
  }
}


bool CsvOutputService::
writeFile(bool only_P0)
{
  String path = _computeFinalPath();
  bool all_only_P0 = m_path_only_P0 && m_name_tab_only_P0;
  // Le seul cas où tout le monde écrit est si only_P0 == false et all_only_P0 == false.
  if((only_P0 || all_only_P0) && mesh()->parallelMng()->commRank() != 0) return true;

  std::ofstream ofile(path.localstr());
  if(ofile.fail()) return false;

  _print(ofile);

  ofile.close();
  return true;
}

bool CsvOutputService::
writeFile(String path, bool only_P0)
{
  m_path = path;
  m_path_computed = false;
  return writeFile(only_P0);
}

bool CsvOutputService::
editElem(Integer posX, Integer posY, Real elem)
{
  if(posX < 0 || posX >= m_values_csv.dim2Size() 
  || posY < 0 || posY >= m_values_csv.dim1Size()) 
    return false;


  if(m_size_columns[posX] <= posY) m_size_columns[posX] = posY+1;
  if(m_size_rows[posY] <= posX) m_size_rows[posY] = posX+1;


  m_values_csv[posY][posX] = elem;
  return true;
}

bool CsvOutputService::
editElem(String rowName, String columnName, Real elem)
{
  std::optional<Integer> posX = m_name_columns.span().findFirst(columnName);
  std::optional<Integer> posY = m_name_rows.span().findFirst(rowName);

  if(posX && posY) return editElem(posX.value(), posY.value(), elem);
  return false;
}

Real CsvOutputService::
getElem(Integer posX, Integer posY)
{
  if(posX < 0 || posX >= m_values_csv.dim2Size() 
  || posY < 0 || posY >= m_values_csv.dim1Size()) 
    return 0;

  return m_values_csv[posY][posX];
}

Real CsvOutputService::
getElem(String rowName, String columnName)
{
  std::optional<Integer> posX = m_name_columns.span().findFirst(columnName);
  std::optional<Integer> posY = m_name_rows.span().findFirst(rowName);

  if(posX && posY) return getElem(posX.value(), posY.value());
  return 0;
}

Integer CsvOutputService::
getNumRows()
{
  return m_values_csv.dim1Size();
}

Integer CsvOutputService::
getNumColumns()
{
  return m_values_csv.dim2Size();
}


Integer CsvOutputService::
addAverageColumn(String name_column)
{
  Integer pos = addColumn(name_column, false);
  for(Integer i = 0; i < m_values_csv.dim1Size(); i++) {
    Real avg = 0.0;
    ConstArrayView<Real> view = m_values_csv[i];
    for(Integer j = 0; j < view.size()-1; j++) {
      avg += view[j];
    }
    avg /= view.size()-1;
    addElemColumn(pos, avg);
  }
  return pos;
}