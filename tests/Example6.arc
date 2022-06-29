<?xml version="1.0"?>
<case codename="csv" xml:lang="en" codeversion="1.0">
  <arcane>
    <title>Example 6</title>
    <timeloop>csvLoop</timeloop>
  </arcane>

  <meshes>
    <mesh>
      <generator name="Cartesian2D" >
        <face-numbering-version>1</face-numbering-version>

        <nb-part-x>1</nb-part-x> 
        <nb-part-y>1</nb-part-y>

        <origin>0.0 0.0</origin>

        <x>
          <length>1.0</length>
          <n>1</n>
        </x>

        <y>
          <length>1.0</length>
          <n>1</n>
        </y>

      </generator>
    </mesh>
  </meshes>

  <table-example6>
    <csv-output name="CsvOutput">
      <tableDir>./example6/</tableDir>
      <tableName>Results_Example6</tableName>
    </csv-output>
  </table-example6>

</case>
