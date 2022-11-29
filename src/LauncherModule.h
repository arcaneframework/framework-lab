// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
#ifndef LAUNCHERMODULE_H
#define LAUNCHERMODULE_H
 
#include <arcane/ITimeLoopMng.h>
#include "Launcher_axl.h"
 
using namespace Arcane;
 
class LauncherModule
: public ArcaneLauncherObject
{
 public:
  explicit LauncherModule(const ModuleBuildInfo& mbi) 
  : ArcaneLauncherObject(mbi) { }
 
 public:
  void beginCompute() override;
  void endCompute() override;
  void compute() override;
  VersionInfo versionInfo() const override { return VersionInfo(1, 0, 0); }
};
 
#endif