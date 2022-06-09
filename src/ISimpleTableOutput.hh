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
  /**
   * @brief Méthode permettant d'initialiser le tableau.
   * 
   */
  virtual void init() = 0;
  
  /**
   * @brief Méthode permettant d'initialiser le tableau.
   * 
   * @param name_table Le nom du tableau (et du fichier de sortie).
   */
  virtual void init(String name_table) = 0;

  /**
   * @brief Méthode permettant d'initialiser le tableau.
   * 
   * @param name_table Le nom du tableau (et du fichier de sortie).
   * @param separator La chaine de caractère à inserer entre les valeurs lors de la sortie.
   */
  virtual void init(String name_table, String separator) = 0;

  /**
   * @brief Méthode permettant de nettoyer le tableau pour le réutiliser après.
   * 
   */
  virtual void clear() = 0;

  /**
   * @brief Méthode permettant d'ajouter une ligne.
   * 
   * @param name_row Le nom de la ligne.
   * @return Integer La position de la ligne dans le tableau.
   */
  virtual Integer addRow(String name_row) = 0;

  /**
   * @brief Méthode permettant d'ajouter une ligne.
   * 
   * @param name_row Le nom de la ligne.
   * @param elems Les éléments à insérer sur la ligne.
   * @return Integer La position de la ligne dans le tableau.
   */
  virtual Integer addRow(String name_row, ConstArrayView<Real> elems) = 0;

  /**
   * @brief Méthode permettant d'ajouter une colonne.
   * 
   * @param name_column Le nom de la colonne.
   * @return Integer La position de la colonne dans le tableau.
   */
  virtual Integer addColumn(String name_column) = 0;

  /**
   * @brief Méthode permettant d'ajouter une colonne.
   * 
   * @param name_column Le nom de la colonne.
   * @param elems Les éléments à ajouter sur la colonne.
   * @return Integer La position de la colonne dans le tableau.
   */
  virtual Integer addColumn(String name_column, ConstArrayView<Real> elems) = 0;

  // Globalement, return true si tous les éléments ont été ajoutés.
  /**
   * @brief Méthode permettant d'ajouter un élément à une ligne.
   * 
   * @param pos La position de la ligne.
   * @param elem L'élément à ajouter.
   * @return true Si l'élément a pu être ajouté.
   * @return false Si l'élément n'a pas pu être ajouté.
   */
  virtual bool addElemRow(Integer pos, Real elem) = 0;
  /**
   * @brief Méthode permettant l'ajouter un élément sur une ligne.
   * 
   * @param name_row Le nom de la ligne.
   * @param elem L'élément à ajouter.
   * @param create_if_not_exist Pour savoir si l'on doit créer la ligne si elle n'existe pas encore.
   * @return true Si l'élément a pu être ajouté.
   * @return false Si l'élément n'a pas pu être ajouté.
   */
  virtual bool addElemRow(String name_row, Real elem, bool create_if_not_exist = true) = 0;
  /**
   * @brief Méthode permettant d'ajouter un élément sur la ligne dernièrement manipulée.
   * 
   * @param elem L'élément à ajouter.
   * @return true Si l'élément a été ajouté.
   * @return false Si l'élément n'a pas pu être ajouté.
   */
  virtual bool addElemSameRow(Real elem) = 0;

  /**
   * @brief Méthode permettant d'ajouter plusieurs éléments sur une ligne.
   * 
   * @param pos La position de la ligne.
   * @param elems Le tableau d'élement à ajouter.
   * @return true Si tous les éléments ont été ajoutés.
   * @return false Si [0;len(elems)[ éléments ont été ajoutés.
   */
  virtual bool addElemsRow(Integer pos, ConstArrayView<Real> elems) = 0;
  /**
   * @brief Méthode permettant d'ajouter plusieurs éléments sur une ligne.
   * 
   * @param name_row Le nom de la ligne.
   * @param elems Le tableau d'élement à ajouter.
   * @param create_if_not_exist Pour savoir si l'on doit créer la ligne si elle n'existe pas encore.
   * @return true Si tous les éléments ont été ajoutés.
   * @return false Si [0;len(elems)[ éléments ont été ajoutés.
   */
  virtual bool addElemsRow(String name_row, ConstArrayView<Real> elems, bool create_if_not_exist = true) = 0;
  /**
   * @brief Méthode permettant d'ajouter plusieurs éléments sur la ligne dernièrement manipulée.
   * 
   * @param elems Le tableau d'élement à ajouter.
   * @return true Si tous les éléments ont été ajoutés.
   * @return false Si [0;len(elems)[ éléments ont été ajoutés.
   */
  virtual bool addElemsSameRow(ConstArrayView<Real> elems) = 0;

  /**
   * @brief 
   * 
   * @param pos 
   * @param elem 
   * @return true 
   * @return false 
   */
  virtual bool addElemColumn(Integer pos, Real elem) = 0;
  /**
   * @brief 
   * 
   * @param name_column 
   * @param elem 
   * @param create_if_not_exist 
   * @return true 
   * @return false 
   */
  virtual bool addElemColumn(String name_column, Real elem, bool create_if_not_exist = true) = 0;
  /**
   * @brief 
   * 
   * @param elem 
   * @return true 
   * @return false 
   */
  virtual bool addElemSameColumn(Real elem) = 0;

  /**
   * @brief 
   * 
   * @param pos 
   * @param elems 
   * @return true 
   * @return false 
   */
  virtual bool addElemsColumn(Integer pos, ConstArrayView<Real> elems) = 0;
  /**
   * @brief 
   * 
   * @param name_column 
   * @param elems 
   * @param create_if_not_exist 
   * @return true 
   * @return false 
   */
  virtual bool addElemsColumn(String name_column, ConstArrayView<Real> elems, bool create_if_not_exist = true) = 0;
  /**
   * @brief 
   * 
   * @param elems 
   * @return true 
   * @return false 
   */
  virtual bool addElemsSameColumn(ConstArrayView<Real> elems) = 0;


  /**
   * @brief 
   * 
   * @param pos_x 
   * @param pos_y 
   * @param elem 
   * @return true 
   * @return false 
   */
  virtual bool editElem(Integer pos_x, Integer pos_y, Real elem) = 0;
  /**
   * @brief 
   * 
   * @param name_column 
   * @param name_row 
   * @param elem 
   * @return true 
   * @return false 
   */
  virtual bool editElem(String name_column, String name_row, Real elem) = 0;

  /**
   * @brief Get the Elem object
   * 
   * @param pos_x 
   * @param pos_y 
   * @return Real 
   */
  virtual Real getElem(Integer pos_x, Integer pos_y) = 0;
  /**
   * @brief Get the Elem object
   * 
   * @param name_column 
   * @param name_row 
   * @return Real 
   */
  virtual Real getElem(String name_column, String name_row) = 0;

  /**
   * @brief Get the Row object
   * 
   * @param pos 
   * @return RealUniqueArray 
   */
  virtual RealUniqueArray getRow(Integer pos) = 0;
  /**
   * @brief Get the Row object
   * 
   * @param name_row 
   * @return RealUniqueArray 
   */
  virtual RealUniqueArray getRow(String name_row) = 0;

  /**
   * @brief Get the Column object
   * 
   * @param pos 
   * @return RealUniqueArray 
   */
  virtual RealUniqueArray getColumn(Integer pos) = 0;
  /**
   * @brief Get the Column object
   * 
   * @param name_column 
   * @return RealUniqueArray 
   */
  virtual RealUniqueArray getColumn(String name_column) = 0;

  /**
   * @brief Get the Size Row object
   * 
   * @param pos 
   * @return Integer 
   */
  virtual Integer getSizeRow(Integer pos) = 0;
  /**
   * @brief Get the Size Row object
   * 
   * @param name_row 
   * @return Integer 
   */
  virtual Integer getSizeRow(String name_row) = 0;
  
  /**
   * @brief Get the Size Column object
   * 
   * @param pos 
   * @return Integer 
   */
  virtual Integer getSizeColumn(Integer pos) = 0;
  /**
   * @brief Get the Size Column object
   * 
   * @param name_column 
   * @return Integer 
   */
  virtual Integer getSizeColumn(String name_column) = 0;

  /**
   * @brief Get the Pos Row object
   * 
   * @param name_row 
   * @return Integer 
   */
  virtual Integer getPosRow(String name_row) = 0;
  /**
   * @brief Get the Pos Column object
   * 
   * @param name_column 
   * @return Integer 
   */
  virtual Integer getPosColumn(String name_column) = 0;
  
  /**
   * @brief Get the Num Rows object
   * 
   * @return Integer 
   */
  virtual Integer getNumRows() = 0;
  /**
   * @brief Get the Num Columns object
   * 
   * @return Integer 
   */
  virtual Integer getNumColumns() = 0;
  
  /**
   * @brief 
   * 
   * @param name_column 
   * @return Integer 
   */
  virtual Integer addAverageColumn(String name_column) = 0;

  /**
   * @brief 
   * 
   * @param only_P0 
   */
  virtual void print(bool only_P0 = true) = 0;
  /**
   * @brief 
   * 
   * @param only_proc 
   */
  virtual void print(Integer only_proc) = 0;
  /**
   * @brief 
   * 
   * @param only_P0 
   * @return true 
   * @return false 
   */
  virtual bool writeFile(bool only_P0 = false) = 0;
  /**
   * @brief 
   * 
   * @param path 
   * @param only_P0 
   * @return true 
   * @return false 
   */
  virtual bool writeFile(String path, bool only_P0 = false) = 0;
};

#endif