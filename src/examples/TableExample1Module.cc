// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* TableExample1Module.cc                                      (C) 2000-2022 */
/*                                                                           */
/* Exemple 1 de module utilisant ISimpleTableOutput en tant que singleton.   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "TableExample1Module.hh"

#include <iostream>
#include <random>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void TableExample1Module::
initModule()
{
  // On utilise des valeurs (pseudo-)aléatoires.
  srand(1234);

  // Initialisation du service.
  // On récupère un pointeur vers le singleton créé par Arcane.
  ISimpleTableOutput* table = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();

  // On initialise le tableau grâce à un des initialisateurs.
  // Le nom du tableau sera "Results" et le nom du fichier sortant sera "Results.X".
  // X étant selon le format choisi (.csv par exemple).
  table->init("Results_Example1");

  // On print le tableau dans son état actuel (vide, avec un titre).
  table->print();
}

void TableExample1Module::
loopModule()
{
  // On récupère un pointeur vers le singleton créé par Arcane.
  // (on pourrait aussi créer un attribut pour éviter le le récupérer à chaque fois).
  ISimpleTableOutput* table = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();

  // On crée une colonne nommé "Iteration X" (avec X = itération actuelle).
  table->addColumn("Iteration " + String::fromNumber(m_global_iteration()));

  // On génère deux valeurs (c'est pour l'exemple, sinon oui, ça sert à rien).
  Integer nb_fissions = rand()%99;
  Integer nb_collisions = rand()%99;

  // On ajoute deux valeurs à deux lignes (par défaut, 
  // si les lignes n'existe pas encore, elles sont créées).
  table->addElemRow("Nb de Fissions", nb_fissions);
  table->addElemRow("Nb de Collisions", nb_collisions);

  // On print le tableau dans son état actuel.
  table->print();

  // On effectue trois itérations.
  if (m_global_iteration() == 3)
    subDomain()->timeLoopMng()->stopComputeLoop(true);
}

void TableExample1Module::
endModule()
{
  // On récupère un pointeur vers le singleton créé par Arcane.
  ISimpleTableOutput* table = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();

  // On print le tableau dans son état actuel.
  table->print();
  
  // On enregistre le résultat dans le dossier "example1".
  // Au final, on aura un fichier ayant comme chemin : ./example1/Resultss.X
  // X étant selon le format choisi (.csv par exemple).
  table->writeFile("./example1/");

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
