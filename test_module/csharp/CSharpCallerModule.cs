using Arcane;
using System;
using Python.Runtime;
using System.Threading.Tasks;
using Numpy;

[Arcane.Module("SimpleCSCaller","1.2.3")]
class SimpleCSCaller
: Arcane.BasicModule
{
  VariableCellReal m_temperature;
  public SimpleCSCaller(ModuleBuildInfo infos) : base(infos)
  {
    Console.WriteLine("Create SimpleCSCaller");
    _AddEntryPoint("Main1",MyEntryPoint,IEntryPoint.WComputeLoop,0);
    _AddEntryPoint("OnExit",OnExit,IEntryPoint.WExit,0);
    PythonEngine.Initialize();
  }

  void MyEntryPoint()
  {
    Console.WriteLine("Test calling Python");
    if (m_temperature == null)
      m_temperature = new VariableCellReal(new VariableBuildInfo(DefaultMesh(),"Temperature"));    
    foreach(Cell cell in AllCells())
      m_temperature[cell] = (double)cell.LocalId;
    
    int c = -1;
    using (Py.GIL())
    using (var scope = Py.CreateScope())
    {
      var p1 = np.array<double>(new double[]{1, 3.423, 1.5, 12.5});
      scope.Set("my_array",p1);
      //scope.Set(parameterName, parameter.ToPython());
      scope.Exec("a=my_array.size\nb=5\nc=a+b\nprint(c)\n");
      c = scope.Get<int>("c");
    }
    Console.WriteLine("C={0}",c);
    Console.WriteLine("End setup python");
  }

  void OnExit()
  {
    Console.WriteLine("Exiting");
    PythonEngine.Shutdown();
  }
}
