## CxImage

Vendored third-party C++ image library used by the [Client](../README.md) to load and decode image formats (BMP, JPEG, PNG, GIF, TIFF, and more).

### Structure

- `xima*.cpp` / `xima*.h` — core `CxImage` class plus one codec per supported format.
- `CxImage.vcxproj` / `cximage.sln` — standalone Visual Studio project, built as a dependency of the Client.
- `license.txt` — CxImage license terms.
