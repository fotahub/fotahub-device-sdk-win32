# Simple FOTA update example

The *Simple* example demonstrates the most straight forward manner to make firmware over-the-air updates with FotaHub. It is kind of a "Hello World" example for FOTA updating and a perfect way to get started in that field.

## Operating principle

The goal of this example is to create an executable program (i.e., a Windows EXE file) that demonstrates a full firmware update cycle using FotaHub and can be run directly from your laptop or computer. It works in almost the same way as the update of an embedded firmware on an actutal IoT device. The firmware program is going to be named after the corresponding FotaHub product (e.g., `DemoProduct.exe`) that you will create along the way (or maybe have created already).

Right after being launched, the firmware program creates and opens a file named `DemoProductUpdate.info` that is located in the same folder as the firmware program. It waits until a firmware update info string is entered and saved in this file. The latter is expected to consist of the new firmware version the device should be updated to followed by a ':' and the checksum of the new firmware version:

`<new-version>:<checksum>` 

e.g., `1.1:a15d5599d8745ff4f51690eb3996afcf75d0b1729d1b8cda2491b03940417521`

Once a valid update info string has been found in the `DemoProductUpdate.info` file and the contained new firmware version is different from the version of the already running firmware program, the firmware update procedure is triggered. It involves your laptop or computer to connect to FotaHub and download the binary (i.e., the Windows EXE file) of the new firmware version. Thereby it uses a dedicated URL including the id of the product representing the device in FotaHub, and the name and the version of the firmware binary to be retrieved.

The downloaded firmware binary is stored in a new firmware program file that has the same name as the original one followed by an imaginary partition index '1' (e.g., `DemoProduct1.exe`). At the same time, the downloaded firmware binary's checksum gets recalculated and compared to the checksum included in the previously communicated update info string. If both match the firmware update gets activated by loading and executing the new firmware program file as a new child process. This causes the original firmware program to be replaced with a running instance of the new firmware version downloaded from FotaHub.  

## Installation

A description of the tools that must be available on your laptop or computer and advise how to install them can be found [here](../../README.md#installation).

## Usage

### Create a FotaHub product

Create a FotaHub product for your imaginary IoT device as explained [here](../fotahub/create-product.md). It will be used to upload and provide firmware updates for the same. 

### Create initial firmware version

1. Start the Visual Studio Code and open (`File > Folder...`) the `Simple` example included in the FotaHub Device SDK for Windows (`<device-sdk-root>\examples\simple`).

2. Open the `DemoProductInfo.h` file, and initialize the `DEMO_PRODUCT_ID` and `DEMO_PRODUCT_NAME` constants with the id and the name of the previously created FotaHub product. Leave the `DEMO_PRODUCT_FIRMWARE_VERSION` as is for now. In case you have selected anything else than `SHA256` as the binary checksum algorithm for your FotaHub product, you also must adjust the `DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM` constant accordingly:

```c
#define DEMO_PRODUCT_ID "eb8ab3b1-0938-40ec-afba-9379363948cf"

#define DEMO_PRODUCT_NAME "Demo Product"

#define DEMO_PRODUCT_FIRMWARE_VERSION "1.0"

#define DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM FOTA_UPDATE_VERIFICATION_ALGORITHM_SHA256
```
   
> &#x1F6C8; You can look up the id of your [FotaHub](https://fotahub.com) product in the `Settings > General` section of the same.

3. Open the integrated terminal (`Terminal > New Terminal...`) and build the example by typing the following command:
   
```bat
make
```

4. You can find the resulting firmware program (i.e., Windows EXE file) in the `build\win32\debug\bin` folder (using the `Explorer` view). Its name equals the name of the FotaHub product configured in the `DemoProductInfo.h` file without spaces (e.g., `DemoProduct.exe`).

### Create and upload a new firmware version to FotaHub

1. Go back to the `DemoProductInfo.h` file, and bump the `DEMO_PRODUCT_FIRMWARE_VERSION` constant:

```c
#define DEMO_PRODUCT_FIRMWARE_VERSION "1.1"
```

2. Rebuild the example by typing the following command in the integrated terminal:

```bat
make UPDATE=y
```

3. Locate the resulting new firmware program in the  `build\win32\debug\bin` folder (using the `Explorer` view). Its name is the name of the FotaHub product configured in the `DemoProductInfo.h` file without spaces followed by an `Update` suffix (e.g., `DemoProductUpdate.exe`).
   
> &#x1F6C8; You can locate the new firmware program file also in Windows Explorer or copy its path to the clipboard using corresponding Visual Studio Code context menu actions (`Reveal in File Explorer` and `Copy Path`). This can come quite handy in the subsequent steps.

4. Upload the new firmware program (e.g., `DemoProductUpdate.exe`) as firmware version `1.1` to your FotaHub product as explained [here](../fotahub/upload-firmware.md).

### Make your first firmare over-the-air update 

1. Go back to Visual Studio Code and launch the initial firmware version by typing the following command in the integrated terminal (choose the firmware program name according to your FotaHub product name, e.g., `DemoProduct.exe`):

```bat
build\win32\debug\bin\<product-name>.exe
```

2. The firmware program prints a banner including the product name and the initial firmware version into the terminal output. It also creates and opens the `DemoProductUpdate.info` file directly in Visual Studio Code (if the latter is not installed on your machine, you will need to open the `DemoProductUpdate.info` file manually in an editor of you choice):
   
![](simple-1.png "Launch of initial firmware version") 

3. Enter the new firmware version followed by a ':' and the checksum of the same in the `DemoProductUpdate.info` file and save it.

![](simple-2.png "Trigger of FOTA update") 

> &#x1F6C8; You can find the checksum of the new firmware version by selecting it in the `Details` section of your [FotaHub](https://fotahub.com) product and locating it in the properties of the same.

4. This will trigger the firmware over-the-air update procedure. Upon successful completion, the initial firmware program exits and the new firmware version downloaded from FotaHub is started automatically. To verify that, check the firmware version in the banner being printed into the terminal output:

![](simple-3.png "Execution of FOTA update")

> &#x26A0; Under certain conditions, it may happen that you encounter an `I/O error during handling of downloaded firmware chunk` right after the firmware over-the-air update procedure has been started. In this case,  check if there are any duplicate instances of the firmware program running on your machine. Kill all of them (using the Windows Task Manager, e.g.) and restart the firmware program.