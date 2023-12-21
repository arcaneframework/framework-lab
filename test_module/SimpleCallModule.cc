// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-

#include "SimpleCall_axl.h"
#include <arcane/ITimeLoopMng.h>

using namespace Arcane;

//! Module SimpleCall.
class SimpleCallModule
: public ArcaneSimpleCallObject
{
 public:

  explicit SimpleCallModule(const ModuleBuildInfo& mbi) 
  : ArcaneSimpleCallObject(mbi) { }

 public:

  //! Method called at each iteration
  void compute() override;

  //! Method called at the beginning of the execution
  void startInit() override;

  //! Version of the module
  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void SimpleCallModule::
compute()
{
  info() << "Module SimpleCall COMPUTE";

  // Stop code after 10 iterations
  if (m_global_iteration()>10)
    subDomain()->timeLoopMng()->stopComputeLoop(true);
}

void SimpleCallModule::
startInit()
{
  info() << "Module SimpleCall INIT";
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_MODULE_SIMPLECALL(SimpleCallModule);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
