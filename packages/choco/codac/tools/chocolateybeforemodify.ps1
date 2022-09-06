$ErrorActionPreference = 'Stop'; # Stop on all errors.

# Source variables which are shared between install and uninstall.
. $PSScriptRoot\sharedVars.ps1

#Uninstall-BinFile -Name libcodac-rob.a
#Uninstall-BinFile -Name libcodac.a

if (Test-Path $CMakeRegistryPath) {
  if (Test-Path $CMakeSystemRepositoryPath\$CMakePackageName) {
      Remove-Item "$CMakeSystemRepositoryPath\$CMakePackageName"
  }
}
