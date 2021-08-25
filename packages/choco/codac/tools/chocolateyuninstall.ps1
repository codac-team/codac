$ErrorActionPreference = 'Stop'; # Stop on all errors.

# Source registry key values which are shared between install and uninstall.
. $PSScriptRoot\regKeys.ps1

#Uninstall-BinFile -Name libcodac-pyibex.a
#Uninstall-BinFile -Name libcodac-rob.a
#Uninstall-BinFile -Name libcodac.a

if (Test-Path $CMakeRegistryPath) {
  if (Test-Path $CMakeSystemRepositoryPath) {
      Remove-Item "$CMakeSystemRepositoryPath\$CMakePackageName"
  }
}
