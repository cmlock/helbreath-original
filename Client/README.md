## Helbreath Client

Win32 game client (DirectDraw/DirectInput/DirectSound), built with Visual Studio.

### Structure

- `*.cpp` / `*.h` — core client source: game loop, rendering, input, networking, UI.
- `CxImage/` — vendored third-party image-loading library used to decode textures/assets.
- `jpeg/` — vendored third-party libjpeg, used by CxImage for JPEG decoding.
- `Client.sln` / `Client.vcxproj` — Visual Studio solution and project files.
- `DDRAW.*`, `DINPUT.*`, `DSOUND.*`, `DXGUID.LIB` — DirectX 7-era SDK headers and import libs.
- `resource.rc`, `*.ico` — Win32 resources and icons.
- `LOGIN.CFG` — default client login configuration.
