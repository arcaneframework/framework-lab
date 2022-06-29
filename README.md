Simple Table Output Service (STOS)
---------------

Ce service permet de créer un tableau de valeur très facilement.
Pour l'instant, la seule implémentation disponible est le CSV.

Exemple de fichier .csv :
```csv
Results_Example3;Iteration 1;Iteration 2;Iteration 3;
Nb de Fissions;36;0;85;
Nb de Collisions;29;84;21;
```
Sous Excel (ou un autre tableur), on obtient ce tableau :
| Results_Example3 | Iteration 1 | Iteration 2 | Iteration 3 |
|------------------|-------------|-------------|-------------|
| Nb de Fissions   | 36          | 0           | 85          |
| Nb de Collisions | 29          | 84          | 21          |

____
## Utilisation
Pour l'utiliser, il suffit d'ajouter le repértoire *simple_table_output* dans
vos sources et de placer ces lignes dans votre **CMakeLists.txt** :
```cmake
set (BUILD_SHARED_LIBS TRUE)

add_library(CsvOutput simple_table_output/CsvOutputService.cc)
target_include_directories(CsvOutput PUBLIC .)
target_link_libraries(CsvOutput PUBLIC arcane_core)

# ${PROJECT_NAME} = le nom de votre projet
target_link_libraries(${PROJECT_NAME} PRIVATE CsvOutput)

arcane_target_add_axl(CsvOutput simple_table_output/CsvOutput)
```
____
### Singleton
(voir *TableExample1 et/ou 2* pour un exemple complet)

Pour une utilisation en tant que singleton (même objet pour tous les modules) :

Placer ces lignes dans le .config de votre projet :
```xml
<singleton-services>
  <service name="CsvOutput" need="required" />
</singleton-services>
```
Et dans votre/vos module(s) :
```c
#include "simple_table_output/ISimpleTableOutput.hh"

ISimpleTableOutput* table = ServiceBuilder<ISimpleTableOutput>(subDomain()).getSingleton();
table->init("Example_Name");
//...
table->writeFile("./example/");
```
____
### Service
(voir *TableExample3* pour un exemple complet)

Pour une utilisation en tant que service (objet different pour chaque module) : 

Placer ces lignes dans le .axl de votre module :
```xml
<!-- <options> -->
  <service-instance name="csv-output" type="ISimpleTableOutput">
    <description>CSV Output</description>
  </service-instance>
<!-- </options> -->
```
Dans le .arc, vous pouvez configurer les options du services. Par exemple :
```xml
<!-- <mon-module> -->
  <csv-output name="CsvOutput">
    <!-- Le nom du répertoire à créer/utiliser. -->
    <tableDir>./example/</tableDir>
    <!-- Le nom du fichier à créer/écraser. -->
    <tableName>Results_Example</tableName>

    <!-- Au final, on aura un fichier ayant comme chemin : ./example/Results_Example.csv -->
  </csv-output>
<!-- </mon-module> -->
```
Et dans votre module :
```c
#include "simple_table_output/ISimpleTableOutput.hh"

options()->csvOutput()->init();
//...
options()->csvOutput()->writeFile();
```

Vous pouvez aussi utiliser le service des deux façons en même temps, selon vos besoins.

Pour un exemple plus concret, voir la [mini-app Quicksilver](https://github.com/arcaneframework/arcane-benchs/)

____
### Utilisation des exemples/du testeur
Pour lancer les exemples, il suffit de :
- installer Arcane
- cloner le dépôt
- modifier le fichier *csv.config* (commenter/décommenter les lignes correspndantes à l'exemple voulu)
- compiler le projet
- choisir le *.arc* correspondant à l'exemple
- lancer

```sh
ARCANE_INSTALL_PATH= # your install path.

CSV_BUILD_TYPE=Release # or Debug
CSV_SOURCE_DIR=framework-lab # your src path
CSV_BUILD_DIR=build_lab # your build dir

CSV_EXE=${CSV_BUILD_DIR}/src/csv

CSV_ARC=${CSV_SOURCE_DIR}/tests/Example1.arc
# OR
CSV_ARC=${CSV_SOURCE_DIR}/tests/Example2.arc
# OR
CSV_ARC=${CSV_SOURCE_DIR}/tests/Example3.arc

mkdir -p ${CSV_BUILD_DIR}
cd ${CSV_BUILD_DIR}

cmake -S ${CSV_SOURCE_DIR} -B ${CSV_BUILD_DIR} -DCMAKE_PREFIX_PATH=${ARCANE_INSTALL_PATH} -DCMAKE_BUILD_TYPE=${CSV_BUILD_TYPE}
make

${CSV_EXE} ${CSV_ARC}
```
Pour le testeur, inutile de modifier le *csv.config*. Suivre les mêmes instructions et choisir ce .arc :

```sh
CSV_ARC=${CSV_SOURCE_DIR}/tests/Tester.arc
```

____
### Documentation
Une ébauche de documentation est disponible. Pour la générer, utiliser doxygen.