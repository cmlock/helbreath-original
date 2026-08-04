<#
Copies the freshly built HGserver.exe out to every per-zone GameServers
folder. Run this after building HGServer.vcxproj in Visual Studio (or via
msbuild) so the zone folders pick up the new binary.

Run from anywhere; paths are resolved relative to this script's location
(Files/), so it must stay in Files/ alongside GameServers/. The build output
is expected at ..\HGServer\<Configuration>\HGserver.exe relative to this
script, matching the OutDir set in HGserver.vcxproj.
#>

param(
    [ValidateSet('Release', 'Debug')]
    [string]$Configuration = 'Release',
    [switch]$WhatIf
)

$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

$source = Join-Path $root "..\HGServer\$Configuration\HGserver.exe"
if (-not (Test-Path $source)) {
    Write-Error "Build output not found at $source - build HGserver.vcxproj ($Configuration) first."
    exit 1
}
$source = (Resolve-Path $source).Path

$gameServerZones = @(
    'Aresden', 'Elvine', 'Middleland', 'Dungeons', 'Heldenian',
    'Apocalypse', 'Neutrals', 'Huntzones', 'ToH'
)

foreach ($zone in $gameServerZones) {
    $dest = Join-Path $root "GameServers\$zone\HGserver.exe"

    if ($WhatIf) {
        Write-Host "$zone`: would copy $source -> $dest" -ForegroundColor Yellow
        continue
    }

    try {
        Copy-Item -Path $source -Destination $dest -Force
        Write-Host "$zone`: deployed." -ForegroundColor Green
    } catch {
        Write-Warning "$zone`: copy failed (server likely still running - stop it first). $($_.Exception.Message)"
    }
}
