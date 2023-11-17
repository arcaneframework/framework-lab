Wrapping Python
---------------

ATTENTION: Ce support est très expérimental

Installation
============

Ce wrapper utilise le wrapping C# généré par Arcane et le rend
accessible en python via l'outil
[pythonnet](https://pythonnet.github.io).

Il est nécessaire d'utiliser la version 3.6+ de Arcane ainsi que la version 3.0+ de pythonnet.

La procédure d'installation est la suivante:

- Compiler et Installer Arcane dans le répertoire
  ${ARCANE_INSTALL_PATH}
- Installer pythonnet en suivante la procédure spécifiée dans la
  [documentation](https://github.com/pythonnet/pythonnet/wiki/Installation):
  `pip install pythonnet`.
- configurer le wrapper:
~~~{sh}
cmake -S /path/to/sources -B build_dir -DCMAKE_PREFIX_PATH=${ARCANE_INSTALL_PREFIX} -DCMAKE_INSTALL_PREFIX=/path/to/wrapping
cmake --build build_dir --target install
~~~

Pour lancer les tests:

~~~{sh}
ctest
~~~

Une fois installé dans `/path/to/wrapping`, il est possible de lancer
les tests en positionnant le PYTHONPATH à ce chemin:

~~~{.sh}
PYTHONPATH=/path/to/wrapping python3 /path/to/wrapping/tests/test4.py
~~~

