<?xml version="1.0"?>
<case codename="ArcanePython" xml:lang="en" codeversion="1.0">
  <arcane>
    <title>Sample</title>
    <timeloop>ArcanePythonLoop</timeloop>
  </arcane>

  <meshes>
    <mesh>
      <!-- You can use a file instead of a generator using the element
           <filename>/path/to/the/file</filename> instead of <generator/> --> 
      <generator name="Cartesian2D" >
        <nb-part-x>1</nb-part-x> 
        <nb-part-y>1</nb-part-y>
        <origin>0.0 0.0</origin>
        <x><n>20</n><length>2.0</length></x>
        <y><n>20</n><length>2.0</length></y>
      </generator>
    </mesh>
  </meshes>
</case>
