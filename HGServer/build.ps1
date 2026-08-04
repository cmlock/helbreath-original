<#
.SYNOPSIS
    Builds HGserver.sln.

.PARAMETER Configuration
    Debug or Release. Defaults to Release.

.EXAMPLE
    .\build.ps1
    .\build.ps1 -Configuration Debug
#>
param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release"
)

$ErrorActionPreference = "Stop"

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    throw "vswhere.exe not found at '$vswhere' - is Visual Studio installed?"
}

$msbuild = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
if (-not $msbuild) {
    throw "Could not locate MSBuild.exe via vswhere."
}

$solution = Join-Path $PSScriptRoot "HGserver.sln"

# The .sln exposes 32-bit configs as "x86" (Configuration Manager naming), even
# though the underlying .vcxproj platform is "Win32" - MSBuild wants the
# solution-level name ("x86") here, not the project-level one ("Win32").
& $msbuild $solution -p:Configuration=$Configuration -p:Platform=x86 -m
if ($LASTEXITCODE -ne 0) {
    throw "Build failed with exit code $LASTEXITCODE"
}

$exe = Join-Path $PSScriptRoot "$Configuration\HGserver.exe"
Write-Host "`nBuild succeeded: $exe"
