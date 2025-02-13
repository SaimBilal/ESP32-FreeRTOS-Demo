# ESP32-S3 SMP FreeRTOS Project Demo

## Introduction

Welcome to this tutorial on setting up and using the ESP32-S3 SMP FreeRTOS project. This guide will walk you through installing the ESP-IDF, cloning the project from GitHub, building and flashing the firmware.

## Prerequisites

Before proceeding, ensure you have the following:

- A Windows machine
- ESP32-S3 development board (ESP32-S3-DevKitC-1)
- USB cable for flashing
- Internet connection

## 1. Setting Up ESP-IDF

To develop for the ESP32-S3, you need the ESP-IDF development environment.

1. Download and install the **ESP-IDF Tools Installer** from [Espressif's website](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/windows-setup.html). (This repo is built using the esp-idf version v5.3.2)
2. Follow the installer instructions to install the ESP-IDF. (Make sure that the USB Serial Drivers are part of your installation)
3. Open **ESP-IDF PowerShell** or the **ESP-IDF Command Prompt** from the start menu (May also be available on the Desktop). One may assume that the PATH variable has now been updated and the esp-idf toolchain is now available to the windows machine system-wide. This, however, is unfortunatley not the case. In order to utilize the various esp-idf tools only the configured terminals must be used (i.e. those mentioned earlier).  
4. Run the following command to confirm installation:
   ```powershell
   idf.py --version
   ```

## 2. Cloning the Project from GitHub

To obtain the project code, follow these steps:

1. Open **Git Bash** (if you have it installed) or **PowerShell / Command Prompt**.
2. Navigate to the directory where you want to store the project:
   ```powershell
   cd path\to\your\projects
   ```
3. Clone the repository:
   ```powershell
   git clone https://github.com/SaimBilal/ESP32-FreeRTOS-Demo.git
   ```
4. Navigate into the project folder:
   ```powershell
   cd ESP32-FreeRTOS-Demo
   ```

## 3. Building and Flashing the Firmware

1. Open **ESP-IDF PowerShell** and navigate to the project directory.
2. Run the following commands:
   ```powershell
   idf.py set-target esp32s3
   idf.py build
   ```
3. Connect your ESP32-S3 board and flash the firmware, remember to connect the microUSB pin to the UART port (in case of the ESP32-S3-DevKitC-1):
   
   ```powershell
   idf.py flash monitor
   ```
    or seperatly,

    ```powershell
   idf.py flash 
   idf.py monitor
   ```
   In case you have more than one serial devices connected, you may need to find out the port number/name for the connected ESP32-S3 device. This can be done through opening the **Device Manger** on windows. And plugging in the ESP32-S3 device to see which new connections appear on the **Ports (COM & LPT)** section. The name will be in round brackets (), like so **Silicon Labs Serial Device (COM4)**.

   The commands then bceome,

   ```powershell
   idf.py -p COM4 flash monitor
   ```
    or seperatly,

    ```powershell
   idf.py -p COM4 flash 
   idf.py -p COM4 monitor
   ```

   MAKE SURE TO SPECIFY WHICH PORT THE ESP-IDF MUST FLASH TO. OTHERWISE IT MAY ASSUME AND DAMAGE OTHER SERIAL INTERFACE COMPONENTS CONNECTED TO YOU PC.


4. Press `Ctrl+]` to exit the monitor.

## 4. Running the App;ication

After running the monitor command, there should be a lot of (green) text on the screen:

```powershell
    I (33) boot: ESP-IDF v5.3.2 2nd stage bootloader
    I (33) boot: compile time Feb 13 2025 02:58:53
    I (33) boot: Multicore bootloader
    I (36) boot: chip revision: v0.1
    I (40) boot: efuse block revision: v1.2
    I (45) boot.esp32s3: Boot SPI Speed : 80MHz
    I (49) boot.esp32s3: SPI Mode       : SLOW READ
    I (55) boot.esp32s3: SPI Flash Size : 2MB
    I (59) boot: Enabling RNG early entropy source...
    I (65) boot: Partition Table:
    I (68) boot: ## Label            Usage          Type ST Offset   Length
    I (76) boot:  0 nvs              WiFi data        01 02 00009000 00006000
    I (83) boot:  1 phy_init         RF data          01 01 0000f000 00001000
    I (91) boot:  2 factory          factory app      00 00 00010000 00100000
    I (98) boot: End of partition table
    I (102) esp_image: segment 0: paddr=00010020 vaddr=3c090020 size=1a738h (108344) map
    I (136) esp_image: segment 1: paddr=0002a760 vaddr=3fc99c00 size=0498ch ( 18828) load
    I (141) esp_image: segment 2: paddr=0002f0f4 vaddr=40374000 size=00f24h (  3876) load
    I (143) esp_image: segment 3: paddr=00030020 vaddr=42000020 size=83afch (539388) map
    I (277) esp_image: segment 4: paddr=000b3b24 vaddr=40374f24 size=14c18h ( 85016) load
    I (309) boot: Loaded app from partition at offset 0x10000
    I (310) boot: Disabling RNG early entropy source...
    I (322) cpu_start: Multicore app
    I (331) cpu_start: Pro cpu start user code
    I (331) cpu_start: cpu freq: 160000000 Hz
    I (331) app_init: Application information:
    I (334) app_init: Project name:     nodma_temp_wifi
    I (340) app_init: App version:      84d8915
    I (345) app_init: Compile time:     Feb 13 2025 02:57:02
    I (351) app_init: ELF file SHA256:  f782795f5...
    I (356) app_init: ESP-IDF:          v5.3.2
    I (361) efuse_init: Min chip rev:     v0.0
    I (365) efuse_init: Max chip rev:     v0.99
    I (370) efuse_init: Chip rev:         v0.1
    I (375) heap_init: Initializing. RAM available for dynamic allocation:
    I (382) heap_init: At 3FCA29D8 len 00046D38 (283 KiB): RAM
    I (388) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
    I (395) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
    I (401) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
    W (408) spi_flash: Octal flash chip is using but dio mode is selected, will automatically switch to Octal mode
    I (418) spi_flash: detected chip: mxic (opi)
    I (423) spi_flash: flash io: opi_str
    W (427) spi_flash: Detected size(32768k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
    I (440) sleep: Configure to isolate all GPIO pins in sleep state
    I (447) sleep: Enable automatic switching of GPIO sleep configuration
    I (454) main_task: Started on CPU0
    I (464) main_task: Calling app_main()
    I (464) main: Starting application...
    I (484) temperature_sensor: Range [-10°C ~ 80°C], error < 1°C
    I (484) temp_sensor: Temperature sensor initialized and enabled
    I (484) pp: pp rom version: e7ae62f
    I (484) net80211: net80211 rom version: e7ae62f
    I (504) wifi:wifi driver task: 3fcaccec, prio:23, stack:6656, core=0
    I (504) wifi:wifi firmware version: b0fd6006b
    I (504) wifi:wifi certification version: v7.0
    I (504) wifi:config NVS flash: enabled
    I (514) wifi:config nano formating: disabled
    I (514) wifi:Init data frame dynamic rx buffer num: 32
    I (514) wifi:Init static rx mgmt buffer num: 5
    I (524) wifi:Init management short buffer num: 32
    I (524) wifi:Init dynamic tx buffer num: 32
    I (534) wifi:Init static tx FG buffer num: 2
    I (534) wifi:Init static rx buffer size: 1600
    I (534) wifi:Init static rx buffer num: 10
    I (544) wifi:Init dynamic rx buffer num: 32
    I (544) wifi_init: rx ba win: 6
    I (554) wifi_init: accept mbox: 6
    I (554) wifi_init: tcpip mbox: 32
    I (554) wifi_init: udp mbox: 6
    I (564) wifi_init: tcp mbox: 6
    I (564) wifi_init: tcp tx win: 5760
    I (574) wifi_init: tcp rx win: 5760
    I (574) wifi_init: tcp mss: 1440
    I (574) wifi_init: WiFi IRAM OP enabled
    I (584) wifi_init: WiFi RX IRAM OP enabled
    I (594) phy_init: phy_version 680,a6008b2,Jun  4 2024,16:41:10
    I (634) wifi:mode : softAP (34:85:18:a1:70:c9)
    I (644) wifi:Total power save buffer number: 16
    I (644) wifi:Init max length of beacon: 752/752
    I (644) wifi:Init max length of beacon: 752/752
    I (644) wifi_ap: WiFi AP started with SSID: ESP32-S3_AP
    I (644) esp_netif_lwip: DHCP server started on interface WIFI_AP_DEF with IP: 192.168.4.1
    I (654) wifi_ap: AP IP Address: 192.168.4.1
    I (664) main: Temperature task started
    I (664) main: Tasks created. System is now in STREAM mode.
    I (664) wifi_ap: TCP server started on port 3333
    I (674) main_task: Returned from app_main()
    I (674) main: TCP Server initialized, waiting for connections

   ```

When the "waiting for connections" prompt is reached,

1. Connect the wifi of your Computer to **ESP32-S3_AP** with password **password123**.
2. run the **test.m** script in the MATLAB.
3. You should observe new readings on the graph from matlab at regular intervals depending on the value of the constant **SAMPLE_INTERVAL_MS** in **nodma_temp_wifi.c**.   

## 5. Understanding the Project Design
This project is designed to leverage **FreeRTOS in Symmetric Multiprocessing (SMP) mode**, utilizing both cores of the ESP32-S3 for efficient task execution. The key design aspects include:

### **Temperature Data Acquisition and Buffering**
- A dedicated **temperature sensor task (`temp_task`)** runs on **Core 0**.
- The task reads temperature values at a fixed interval and stores them in a **circular buffer** to facilitate data management.
- The circular buffer ensures:
  - **Efficient storage** of sensor readings.
  - **Thread-safe access** using a **mutex**, preventing data corruption when accessed by multiple tasks.
  - **Oldest data is discarded** when full, maintaining a continuous data stream.

### **Real-Time Data Transmission via Wi-Fi**
- A **Wi-Fi task (`wifi_task`) runs on Core 1**, responsible for:
  - Initializing and managing a **TCP server**.
  - Periodically retrieving buffered temperature readings.
  - Transmitting the collected data to a connected client over Wi-Fi.
  - Handling connection failures by resetting the socket to ensure reliable communication.
- The system is configured in **Wi-Fi Access Point (AP) mode**, allowing clients to connect directly.

### **Concurrency and Synchronization**
- **FreeRTOS Semaphores** are used to protect shared resources (circular buffer), preventing race conditions.
- **Task priorities and delays** are carefully configured:
  - **Temperature task** executes frequently to capture data without unnecessary CPU load.
  - **Wi-Fi task** transmits data at larger intervals to balance network usage and power efficiency.

### **Peripheral and System Initialization**
- **NVS (Non-Volatile Storage)** is needed to ensure proper function.
- The **temperature sensor** is initialized and calibrated before data collection begins.
- The **Wi-Fi module** is set up in AP mode, enabling wireless communication without external networking dependencies.
- The system transitions into **STREAM mode**, continuously collecting and transmitting temperature data.

This architecture enables **monitoring** with SMP between data acquisition, processing, and wireless transmission.



## Further Reading and Usefull Links

- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [FreeRTOS Kernel ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/system/freertos_idf.html)
- [ESP-IDF Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-guides/build-system.html)
-[Application Startup Flow](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-guides/startup.html)

## Conclusion

The project is by no means perfect and needs improvement.

- The dataword sizes are misaligned.
- The Sempahore based message transfer between tasks is not clean and efficient. A notification should be sent to the wifi task when the circular buffer is full.
- **THE WIFI AP IS IN NO WAY SECURE, THE PASSWORD HAS BEEN HARDCODED, DO NOT LET THIS APPLICATION BECOME A NODE IN YOU HOME NETWORK. THIS DEMO IS MERELY A PROOF OF CONCEPT.**