Wrapping Python
---------------

ATTENTION: Ce support est très expérimental

Installation
============

Ce wrapper utilise le wrapping C# généré par Arcane et le rend
accessible en python via l'outil
[Pythonnet](https://pythonnet.github.io).

Il est nécessaire d'utiliser la version 3.6+ de Arcane.

La procédure d'installation est la suivante:

- Compiler et Installer Arcane dans le répertoire
  ${ARCANE_INSTALL_PATH}
- Installer PythonNet en suivante la procédure spécifiée dans la
  [documentation](https://github.com/pythonnet/pythonnet/wiki/Installation):
  `pip install git+https://github.com/pythonnet/pythonnet`.
- configurer le wrapper:
~~~{sh}
cmake -S /path/to/sources -B build_dir -DCMAKE_PREFIX_PATH=${ARCANE_INSTALL_PREFIX} -DCMAKE_INSTALL_PREFIX=/path/to/wrapping
cmake --build build_dir --target install
~~~

Une fois installé dans `/path/to/wrapping`, il est possible de lancer
les tests en positionnant le PYTHONPATH à ce chemin:

~~~{.sh}
PYTHONPATH=/path/to/wrapping python3 /path/to/wrapping/tests/test4.py
~~~

