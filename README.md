# Arduino Playground

Little projects to play around with an ESP. In the first place all the code was implemented using the Arduino programming language and some libraries even in C++. Once I learned about the ESPHome project I migrated all my sensors to _yaml_ files and used the ESP-flasher tool to deploy the code. In my opinion this is a very simple and fail-safe approach to easily integrate sensors into an existing home-assistant environment.

## Project Structure

- **Sources/**. A few projects implemented in the Arduino programming language.
- **Libraries/**. Libraries to simplify usage of MQTT and HTTP requests. Probably outdated.
- **Sketches/**. Some pictures and sketches to show wiring diagrams for flashing.
- **ESPHome/**. yaml files that can be directly used to flash a firmaware on the devices using [ESPHome Flasher](https://github.com/esphome/esphome-flasher).

## ESPHome

ESPHome is currently my preferred way to flash firmwares on the ESP. One just needs to create virtual python environment and install some packages.

### Installation

Install a new python environment with conda and activate it:

```bash
$ conda create --prefix envs/esphome-flasher python==3.8
$ conda activate envs/esphome-flasher
```

Then install the required libraries for ESPHome

```bash
$ pip3 install esphome esphomeflasher
```

### Usage

Usually there is no configuration needed because the flasher detects a connected ESP automatically. This was at least true for a Apple M1 with the hardware directly connected via USB or via the _TDI Adapter FT232RL_.

A _yaml_ file (see examples in the _ESPHome/_) can be easily flashed using this command (ensure to have the conda environment active):

```bash
$ esphome run my.yaml
```

The software will automatically detect the port where the ESP hardware is connected, compile a firmware from the given input file and write.
