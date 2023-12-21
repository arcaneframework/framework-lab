using Arcane;
using System;
using Python.Runtime;
using System.Threading.Tasks;

[Arcane.Module("SimpleCSCaller","1.2.3")]
class SimpleCSCaller
: Arcane.BasicModule
{
  public SimpleCSCaller(ModuleBuildInfo infos) : base(infos)
  {
    Console.WriteLine("Create SimpleCSCaller");
    _AddEntryPoint("Main1",MyEntryPoint,IEntryPoint.WComputeLoop,0);
    PythonEngine.Initialize();
  }

  void MyEntryPoint()
  {
    Console.WriteLine("Test calling Python");
    int c = -1;
    using (Py.GIL())
    using (var scope = Py.CreateScope())
    {
      //scope.Set(parameterName, parameter.ToPython());
      scope.Exec("a=3\nb=5\nc=a+b\nprint(c)\n");
      c = scope.Get<int>("c");
    }
    Console.WriteLine("C={0}",c);
    Console.WriteLine("End setup python");
  }
}
