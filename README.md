# ParallelMngArcane

## Description
Ce projet permet d'utiliser le ParallelMng d'Arcane dans un programme
utilisant MPI.

## Compilation
```sh
ARCANE_BUILD_TYPE=Debug
ARCANE_INSTALL_PATH= # your install path.

POINTH_BUILD_TYPE=${ARCANE_BUILD_TYPE}
POINTH_PROJECT_DIR=framework-lab # your project path
POINTH_SOURCE_DIR=${POINTH_PROJECT_DIR}
POINTH_BUILD_DIR=build_lab # your build dir
POINTH_EXE=${POINTH_BUILD_DIR}/src/ParallelMngArcane
mkdir -p ${POINTH_BUILD_DIR}

cd ${POINTH_BUILD_DIR}

cmake \
  -S ${POINTH_SOURCE_DIR} \
  -B ${POINTH_BUILD_DIR} \
  -GNinja \
  -DCMAKE_PREFIX_PATH=${ARCANE_INSTALL_PATH} \
  -DCMAKE_BUILD_TYPE=${POINTH_BUILD_TYPE}

ninja

# Mode Mpi
mpirun -n 4 ${POINTH_EXE} ${POINTH_PROJECT_DIR}/src/nbPart4.arc

# Mode Shared Memory
${POINTH_EXE} -A,S=4 ${POINTH_PROJECT_DIR}/src/nbPart4.arc

# Mode Hybrid
mpirun -n 2 ${POINTH_EXE} -A,S=2 ${POINTH_PROJECT_DIR}/src/nbPart4.arc

```
