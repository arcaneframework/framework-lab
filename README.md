CSV Service
---------------

### How to use it?

Build and Install Arcane.

And:

```sh
ARCANE_INSTALL_PATH= # your install path.

CSV_BUILD_TYPE=Release # or Debug
CSV_SOURCE_DIR=framework-lab # your src path
CSV_BUILD_DIR=build_lab # build dir

CSV_EXE=${CSV_BUILD_DIR}/src/csv
CSV_ARC=${CSV_SOURCE_DIR}/tests/Tester.arc
mkdir -p ${CSV_BUILD_DIR}
cd ${CSV_BUILD_DIR}

cmake -S ${CSV_SOURCE_DIR} -B ${CSV_BUILD_DIR} -DCMAKE_PREFIX_PATH=${ARCANE_INSTALL_PATH} -DCMAKE_BUILD_TYPE=${CSV_BUILD_TYPE}
make

${CSV_EXE} ${CSV_ARC}
```