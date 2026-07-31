<#
Checks every Helbreath server process (gate, login/world, and each per-zone
game server) and launches any that aren't currently running.

Run from anywhere; paths are resolved relative to this script's location
(Files/), so it must stay in Files/ alongside GateServer.exe / MainLServer.exe
/ WorldLServer.exe / GameServers/.

GateServer -> MainLServer -> WorldLServer must come up in that order, each
one registering with the previous over TCP. Launching the next one too soon
after the previous can land a registration attempt before the previous
process has finished initializing enough to record it, leaving it connected
but never marked active - with no automatic retry. So after starting each of
those three we wait for its port to actually be listening (plus a short
grace buffer) before starting the next, instead of a fixed guessed sleep.
#>

param(
    [switch]$WhatIf
)

$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

# Must match gate-server-address / log-server-address / world-server-address
# in GateServer.cfg, HMLServer.cfg, and WLserver.cfg - those servers may bind
# their listening socket to this address rather than all interfaces, so the
# readiness check has to dial the same address other servers register against.
$serverAddress = '192.168.0.192'

function Wait-ForPort {
    param(
        [string]$Name,
        [int]$Port,
        [int]$TimeoutSeconds = 30
    )

    $deadline = (Get-Date).AddSeconds($TimeoutSeconds)
    while ((Get-Date) -lt $deadline) {
        $client = New-Object System.Net.Sockets.TcpClient
        try {
            $client.Connect($serverAddress, $Port)
            if ($client.Connected) {
                $client.Close()
                # Grace buffer: the port can accept connections slightly
                # before the app is done initializing its registration
                # handling, so give it a moment beyond "port is open".
                Start-Sleep -Seconds 2
                return $true
            }
        } catch {
            Start-Sleep -Milliseconds 250
        } finally {
            $client.Dispose()
        }
    }

    Write-Warning "$Name`: port $Port never came up within $TimeoutSeconds seconds - continuing anyway."
    return $false
}

$servers = @(
    @{ Name = 'GateServer';  ExePath = Join-Path $root 'GateServer.exe';  WaitPort = 6502 }
    @{ Name = 'MainLServer'; ExePath = Join-Path $root 'MainLServer.exe'; WaitPort = 2848 }
    @{ Name = 'WorldLServer';ExePath = Join-Path $root 'WorldLServer.exe';WaitPort = 2500 }
)

$gameServerZones = @(
    'Aresden', 'Elvine', 'Middleland', 'Dungeons', 'Heldenian',
    'Apocalypse', 'Neutrals', 'Huntzones', 'ToH'
)

foreach ($zone in $gameServerZones) {
    $servers += @{
        Name    = "HGserver-$zone"
        ExePath = Join-Path $root "GameServers\$zone\HGserver.exe"
        WaitPort = $null
    }
}

# Cache running processes once (matching by full path, since all game
# servers share the same exe name "HGserver.exe").
$runningByPath = @{}
Get-CimInstance Win32_Process -Filter "Name='GateServer.exe' OR Name='MainLServer.exe' OR Name='WorldLServer.exe' OR Name='HGserver.exe'" |
    ForEach-Object {
        if ($_.ExecutablePath) {
            $runningByPath[$_.ExecutablePath.ToLowerInvariant()] = $_.ProcessId
        }
    }

foreach ($server in $servers) {
    $name = $server.Name
    $exePath = $server.ExePath

    if (-not (Test-Path $exePath)) {
        Write-Warning "$name`: executable not found at $exePath, skipping."
        continue
    }

    $resolvedExe = (Resolve-Path $exePath).Path
    $pid_ = $runningByPath[$resolvedExe.ToLowerInvariant()]

    if ($pid_) {
        Write-Host "$name`: already running (PID $pid_)." -ForegroundColor Green
        continue
    }

    $workDir = Split-Path $resolvedExe -Parent
    if ($WhatIf) {
        Write-Host "$name`: NOT running -> would start $resolvedExe (in $workDir)." -ForegroundColor Yellow
        continue
    }

    Write-Host "$name`: NOT running -> starting..." -ForegroundColor Yellow
    Start-Process -FilePath $resolvedExe -WorkingDirectory $workDir | Out-Null
    Start-Sleep -Milliseconds 500

    if ($server.WaitPort) {
        Write-Host "$name`: waiting for port $($server.WaitPort)..." -ForegroundColor Yellow
        Wait-ForPort -Name $name -Port $server.WaitPort | Out-Null
    }

    Write-Host "$name`: launched." -ForegroundColor Cyan
}
