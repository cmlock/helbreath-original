<#
Stops the running per-zone HGserver.exe instances, copies in the freshly
built binary, and launches them again. Combines what Deploy-HGServer.ps1 and
StartServers.ps1 do separately, but only for the zone game servers - it never
touches GateServer/MainLServer/WorldLServer.

That's safe on its own: each HGserver zone process holds two sockets (to
GateServer and to WorldLServer/"log server") and both have automatic
reconnect-and-re-register logic on disconnect (see CGame::OnGateSocketEvent
and the sub-log-socket retry path in Game.cpp). So a zone can be bounced by
itself at any time without restarting the rest of the stack, as long as
GateServer and WorldLServer are already up.

Run from anywhere; paths are resolved relative to this script's location
(Files/), so it must stay in Files/ alongside GameServers/. The build output
is expected at ..\HGServer\<Configuration>\HGserver.exe relative to this
script, matching the OutDir set in HGserver.vcxproj.

Each zone's GameLogs\*.log files are cleared right before it's relaunched,
so every restart starts with a fresh log instead of appending to the
previous run's.
#>

param(
    [ValidateSet('Release', 'Debug')]
    [string]$Configuration = 'Release',

    [ValidateSet('Aresden', 'Elvine', 'Middleland', 'Dungeons', 'Heldenian',
                 'Apocalypse', 'Neutrals', 'Huntzones', 'ToH')]
    [string[]]$Zones,

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

$allZones = @(
    'Aresden', 'Elvine', 'Middleland', 'Dungeons', 'Heldenian',
    'Apocalypse', 'Neutrals', 'Huntzones', 'ToH'
)
$targetZones = if ($Zones) { $Zones } else { $allZones }

function Wait-ForProcessExit {
    param(
        [int]$ProcessId,
        [int]$TimeoutSeconds = 15
    )

    $deadline = (Get-Date).AddSeconds($TimeoutSeconds)
    while ((Get-Date) -lt $deadline) {
        if (-not (Get-Process -Id $ProcessId -ErrorAction SilentlyContinue)) {
            return $true
        }
        Start-Sleep -Milliseconds 250
    }
    return $false
}

function Clear-GameLogs {
    param([string]$Zone)

    $logsDir = Join-Path $root "GameServers\$Zone\GameLogs"
    if (Test-Path $logsDir) {
        Get-ChildItem -Path $logsDir -File | Remove-Item -Force
    }
}

function Copy-WithRetry {
    param(
        [string]$Source,
        [string]$Destination,
        [int]$Retries = 5
    )

    for ($i = 1; $i -le $Retries; $i++) {
        try {
            Copy-Item -Path $Source -Destination $Destination -Force
            return $true
        } catch {
            if ($i -eq $Retries) { throw }
            # File can stay locked for a moment after the process exits
            # (AV scan, delayed handle release) - back off and retry.
            Start-Sleep -Milliseconds 500
        }
    }
    return $false
}

# Cache running HGserver.exe processes by full path once, since all zones
# share the same exe name.
$runningByPath = @{}
Get-CimInstance Win32_Process -Filter "Name='HGserver.exe'" | ForEach-Object {
    if ($_.ExecutablePath) {
        $runningByPath[$_.ExecutablePath.ToLowerInvariant()] = $_.ProcessId
    }
}

foreach ($zone in $targetZones) {
    $exePath = Join-Path $root "GameServers\$zone\HGserver.exe"

    if (-not (Test-Path $exePath)) {
        Write-Warning "$zone`: no existing HGserver.exe at $exePath, skipping."
        continue
    }

    $resolvedExe = (Resolve-Path $exePath).Path
    $procId = $runningByPath[$resolvedExe.ToLowerInvariant()]

    if ($WhatIf) {
        if ($procId) {
            Write-Host "$zone`: would stop PID $procId, deploy build, and relaunch." -ForegroundColor Yellow
        } else {
            Write-Host "$zone`: not running -> would deploy build and launch." -ForegroundColor Yellow
        }
        continue
    }

    if ($procId) {
        Write-Host "$zone`: stopping PID $procId..." -ForegroundColor Yellow
        Stop-Process -Id $procId -Force
        if (-not (Wait-ForProcessExit -ProcessId $procId)) {
            Write-Warning "$zone`: process $procId did not exit in time - skipping."
            continue
        }
    }

    try {
        Copy-WithRetry -Source $source -Destination $resolvedExe | Out-Null
        Write-Host "$zone`: deployed." -ForegroundColor Green
    } catch {
        Write-Warning "$zone`: copy failed, not relaunching. $($_.Exception.Message)"
        continue
    }

    Clear-GameLogs -Zone $zone

    $workDir = Split-Path $resolvedExe -Parent
    Start-Process -FilePath $resolvedExe -WorkingDirectory $workDir | Out-Null
    Write-Host "$zone`: launched." -ForegroundColor Cyan
}
