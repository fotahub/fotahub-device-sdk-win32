# FotaHub&reg; Device SDK for Windows&reg;
The FotaHub&reg; Device SDK for Windows&reg; enables developers to create firmware for Windows&reg;-based IoT devices that can be updated through [FotaHub](http://fotahub.com). It provides a FotaHub client C library along with a set of examples illustrating how the latter can be used and integrated in real applications.

# Installation
To use the FotaHub&reg; Device SDK for Windows&reg;, the following tools must be available on your laptop or computer:
* A GCC compiler/linker toolchain - we recommend [MSYS2](https://www.msys2.org)
* A [Python 3](https://www.python.org/downloads) interpreter
* A text editor or IDE of your choice, e.g., [Visual Studio Code](https://code.visualstudio.com)

To enable MSYS2 `make`, `gcc`, etc. to be invoked conveniently from Visual Studio Code's integrated terminal, add the following locations to your user or system level `Path` environment variable:
* `C:\msys64\mingw64\bin`
* `C:\msys64\usr\bin`

> &#x1F6C8; The indicated locations assume that you have installed MSYS2 into `C:\msys64`. If this is not the case, make sure to adjust them accordingly.

# Getting started
The best way to get started with the FotaHub&reg; Device SDK for Windows&reg; is to try out and study the included examples:
* [Simple](docs/getting-started/simple.md) - "Hello World" of firmware over-the-air updating with FotaHub&reg;
* [Rollbackable](docs/getting-started/rollbackable.md) - Downloads and launches a new firmware version provided at FotaHub&reg;, rolls it back to the previous version if something goes wrong
* [Thing Shadow](docs/getting-started/thingshadow.md) - A real-world scenario where FotaHub&reg; is used in combination with the AWS IoT Thing Shadow service to trigger firmware updates over-the-air updates through the cloud

# API Reference
View the [C API reference](include/FotaHub.h) for detailed information about the FotaHub&reg; client library (:construction: *under construction*).

# License and third-party software
Unless specifically indicated otherwise in a file included in this project, FotaHub&reg; Device SDK for Windows&reg; files are provided under the *Apache-2.0* license. See the [LICENSE](LICENSE) file for the full text of this license.

The terms of the license attributed to this project do not alter any rights or obligations you may have under the licenses of any third-party software which is redistributed through the present project. Refer to the [NOTICE](NOTICE.md) file for detailed information regarding these aspects.
