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
  m_name_tab = "";
}

void CsvOutputService::
init(String name_csv)
{
  m_separator = ";";
  m_name_tab = name_csv;
}

void CsvOutputService::
init(String name_csv, String separator_csv)
{
  m_separator = separator_csv;
  m_name_tab = name_csv;
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

  return pos;
}

Integer CsvOutputService::
addRow(String name_row, ConstArrayView<Real> elems)
{
  Integer pos = m_values_csv.dim1Size();
  m_values_csv.resize(pos+1);

  ArrayView<Real> view = m_values_csv[pos];
  Integer min_size = (elems.size() < view.size() ? elems.size() : view.size());

  for(Integer i = 0; i < min_size; i++) {
    view[i] = elems[i];
  }
    
  m_name_rows.add(name_row);
  m_size_rows.add(min_size);

  return pos;
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
    m_size_columns[i+size_row]++;
  }
  m_size_rows[pos] += min_size;

  return true;
}

bool CsvOutputService::
addElemRow(Integer pos, Real elem)
{
  if(pos < 0 || pos >= m_values_csv.dim1Size()) return false;

  ArrayView<Real> view = m_values_csv[pos];
  Integer size_row = m_size_rows[pos];

  if(view.size() < size_row + 1) return false;

  view[size_row] = elem;

  m_size_rows[pos]++;
  m_size_columns[size_row]++;

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
addElemColumn(Integer pos, Real elem)
{
  if(pos < 0 || pos >= m_values_csv.dim2Size()) return false;

  Integer size_column = m_size_columns[pos];

  if(m_values_csv.dim2Size() < size_column + 1) return false;

  m_values_csv[size_column][pos] = elem;

  m_size_columns[pos]++;
  m_size_rows[size_column]++;

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

  return pos;
}

Integer CsvOutputService::
addColumn(String name_column, ConstArrayView<Real> elems)
{
  Integer pos = m_values_csv.dim2Size();
  m_values_csv.resize(m_values_csv.dim1Size(), pos+1);

  Integer min_size = (elems.size() < m_values_csv.dim1Size() ? elems.size() : m_values_csv.dim1Size());

  for(Integer i = 0; i < min_size; i++) {
    m_values_csv[i][pos] = elems[i];
  }
    
  m_name_columns.add(name_column);
  m_size_columns.add(min_size);

  return pos;
}

bool CsvOutputService::
addElemsColumn(Integer pos, ConstArrayView<Real> elems)
{
  if(pos < 0 || pos >= m_values_csv.dim2Size()) return false;

  Integer size_column = m_size_columns[pos];
  Integer min_size = (elems.size() < m_values_csv.dim1Size()-size_column ? elems.size() : m_values_csv.dim1Size()-size_column);

  for(Integer i = 0; i < min_size; i++) {
    m_values_csv[i+size_column][pos] = elems[i];
    m_size_rows[i+size_column]++;
  }
  m_size_columns[pos] += min_size;

  return true;
}

void CsvOutputService::
computePathName()
{
  // On ne lit le nom qu'une seule fois.
  if(m_name_computed) return;
  m_name_computed = true;

  // On découpe la string là où se trouve les @.
  StringUniqueArray string_splited;
  m_path_name.split(string_splited, '@');

  // On traite les mots entre les "@".
  if(string_splited.size() > 1) {
    // On recherche "proc_id" dans le tableau (donc @proc_id@ dans le nom).
    std::optional<Integer> proc_id = string_splited.span().findFirst("proc_id");
    // On remplace "@proc_id@" par l'id du proc.
    if(proc_id)
    {
      string_splited[proc_id.value()] = String::fromNumber(mesh()->parallelMng()->commRank());
      m_only_P0 = false;
    }
    // Il n'y a que P0 qui write.
    else{
      m_only_P0 = true;
    }
  }

  // Autres traitements.
  if(!m_path_name.endsWith(".csv")) {
    string_splited.add(".csv");
  }

  // On recombine la chaine.
  String combined = "";
  for(String str : string_splited){
    combined = combined + str;
  }
  m_path_name = combined;
}

void CsvOutputService::
print(bool only_P0)
{
  computePathName();
  if(only_P0 && mesh()->parallelMng()->commRank() != 0) return;
  pinfo() << "P" << mesh()->parallelMng()->commRank() << " - Ecriture du .csv dans la sortie standard :";
  
  std::cout << m_name_tab << m_separator;

  for(Integer j = 0; j < m_name_columns.size(); j++) {
    std::cout << m_name_columns[j] << m_separator;
  }
  std::cout << std::endl;

  for(Integer i = 0; i < m_values_csv.dim1Size(); i++) {
    if(m_name_rows.size() > i) std::cout << m_name_rows[i];
    std::cout << m_separator;
    ConstArrayView<Real> view = m_values_csv[i];
    for(Integer j = 0; j < m_values_csv.dim2Size(); j++) {
      std::cout << view[j] << m_separator;
    }
    std::cout << std::endl;
  }

  pinfo() << "P" << mesh()->parallelMng()->commRank() << " - Fin écriture .csv";
}

bool CsvOutputService::
writeFile()
{
  computePathName();
  if(m_only_P0 && mesh()->parallelMng()->commRank() != 0) return true;

  std::ofstream ofile(m_path_name.localstr());
  if(ofile.fail()) return false;

  ofile << m_name_tab << m_separator;

  for(Integer j = 0; j < m_name_columns.size(); j++) {
    ofile << m_name_columns[j] << m_separator;
  }
  ofile << std::endl;

  for(Integer i = 0; i < m_values_csv.dim1Size(); i++) {
    if(m_name_rows.size() > i) ofile << m_name_rows;
    ofile << m_separator;
    ConstArrayView<Real> view = m_values_csv[i];
    for(Integer j = 0; j < m_values_csv.dim2Size(); j++) {
      ofile << view[j] << m_separator;
    }
    ofile << std::endl;
  }

  ofile.close();
  return true;
}

bool CsvOutputService::
writeFile(String path_file)
{
  m_path_name = path_file;
  m_name_computed = false;
  return writeFile();
}

bool CsvOutputService::
editElem(Integer posX, Integer posY, Real elem)
{
  if(posX < 0 || posX >= m_values_csv.dim2Size() 
  || posY < 0 || posY >= m_values_csv.dim1Size()) 
    return false;

  m_values_csv[posY][posX] = elem;
  return true;
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
