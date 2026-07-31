## JPEG (libjpeg)

Vendored third-party IJG libjpeg library, used by [CxImage](../CxImage/README.md) for JPEG encode/decode support.

### Structure

- `j*.c` / `j*.h` — libjpeg source: compression, decompression, DCT, Huffman coding, memory management.
- `Jpeg.vcxproj` / `Jpeg.sln` — standalone Visual Studio project, built as a dependency of CxImage.
- `change.log`, `Readme.txt` — upstream IJG release notes.
