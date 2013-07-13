#include "stdafx.h"
#include "Installer.h"
#include "InstallerBase.h"
#include "LastErrorPreserver.h"

eHidStatus PerformOperation(const wchar_t* pInfPath, void (CInstallerBase::*pMember)())
{
	// This class is declared first to ensure that its destructor is called last
	CLastErrorPreserver gle;

  try {
	  // Installer base initialization:
	  CInstallerBase base(pInfPath);

	  // Initialize the installer base as needed:
	  base.Init();

	  // Perform the operation proper
	  (base.*pMember)();

    // If restart is required we'll return here to indicate that:
    if(base.IsRestartRequired())
      return eHidInstRestartRequired;
  } catch(eHidStatus rs) {
	  // Done, capture the last error so it's restored after the CInstallerBase destructor
	  // call is made and then return here.
	  return gle.CaptureLastError(), rs;
  }

  return (eHidStatus)0;
}

eHidStatus OcuHidInstall(const wchar_t* pInfPath)
{
  return PerformOperation(pInfPath, &CInstallerBase::Install);
}

eHidStatus OcuHidUpdate(const wchar_t* pInfPath)
{
	return PerformOperation(pInfPath, &CInstallerBase::Update);
}

eHidStatus OcuHidUninstall(const wchar_t* pInfPath)
{
	return PerformOperation(pInfPath, &CInstallerBase::Uninstall);
}
