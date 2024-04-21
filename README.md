# GMAS Extended

This project is meant to provide a very basic starting point for a game built atop the **paid** [General Movement Component v2](https://www.unrealengine.com/marketplace/en-US/product/general-movement-component) movement and networking system for Unreal 5 (hereafter referred to as **GMCv2**), along with the free and open source [GMC Ability System](https://github.com/Reznok/GMCAbilitySystem) (or **GMAS**) and [GMCExtended framework](https://github.com/Rooibot/GMCExtended) (or **GMCEx**).

It is worth noting this is not meant to be a full foundation for a game—certainly not something along the scope of the `LyraSampleGame` project, or the venerable `ShooterGame` from the UE4 era. Think of this template as akin to the `ThirdPerson` or `FirstPerson` templates you can create a new Unreal project from: it doesn't do much, but it should hopefully be enough to get you started.

## Basic Features

This template tries to exercise a few basic features across all parts of the ecosystem, as well as having some convenience classes already made:

* A baseline Pawn class (in C++) which provides both the interface required by GMCEx's motion warping functionality _and_ has components already provided and hooked up, as well as a skeletal mesh and camera for third-person functionality.
* A baseline movement component in C++ integrating all the bits required for GMAS to work and all the GMCEx functionality.
* Example blueprint subclasses for each of the above.

In addition, the project comes with a couple of GMAS abilities (Jump, Roll, and Sprint) already provided.

This template does not attempt to exercise every bit of functionality in all three pieces; it currently makes no use of GMCEx's motion warping (though has all the pieces in place to support it), for instance, nor does it dive into more advanced bits of GMAS or GMCv2. Consider it a starting place, nothing more.

## Installation

### Prerequisites

In order to install the latest version of this template, you will want to have a Git client available. While there will be bundled release .zip files eventually, currently you must pull from the repository. If you don't currently have a client installed, you'll want to pull it down from https://git-scm.com/downloads and install it.

You will also need the most recent version of the General Movement Component installed to your Unreal Engine installation, *or* you will want to be prepared to add it to the project manually after you set it up; if you don't have it installed to the engine, then the first attempt to open the project after creating it will fail, as noted below.+

### Installation Process

Find where your copy of Unreal 5.3 is installed -- on Windows, by default this is `C:\Program Files\Epic Games\UE_5.3` though obviously your copy may be installed somewhere else.

Once you've located your Unreal 5.3 install directory:

1. Make sure that you've installed GMCv2 into 5.3 from the Epic launcher.
  * **NOTE:** You _can_ install the plugin directly into the final project, just be aware that if you don't have GMCv2 in your 5.3 engine install, the project _will_ fail to open the first time.
2. Open a command prompt, such as the Windows `cmd.exe`.
3. Go to the Unreal Engine 5.3 installation directory you've located; if using `cmd.exe` you would do `cd <directory>`, for instance `cd "C:\Program Files\Epic Games\UE_5.3"` -- notice that if there are spaces in the path anywhere, you will want to put the path in quotes.
4. Type `git clone --recursive https://github.com/Rooibot/GMASExTemplate.git` and wait for the command to finish. This will pull not only the GMASEx template but also appropriate versions of GMAS and GMCEx.

This will pull the base template _and_ the appropriate copies of GMAS and GMCEx all in one go.

### Usage

Once you've installed the template as described above, you can launch Unreal 5.3 from the launcher and you'll see that under "Games" a new template is available:

![An example of the GMAS Extended template in the Unreal launch window.](Media/Example-TemplatePanel.png)

When you select the template and create a new project, it will attempt to build the project from source. If you do not have GMCv2 installed to the engine _this will fail_. If so, abort the process and go to the project you just made, then copy GMCv2 into the `Plugins` directory under the project and attempt to build again.

## Project Layout

Once you have the project open, you'll find that in addition to the usual Unreal `Characters` folder, there is a `GMASExtended` folder containing the template's assets. Under that folder are several more:

* **Abilities**: This folder contains example blueprint implementations of three GMAS abilities (Jump, Roll, and Sprint).
* **Animations**: While most of the animation sequences (and the meshes and control rigs) are pulled from the default Unreal character package, the montage used for the "Roll" command and the animation blueprint for our default pawn can be found here.
* **Blueprints**: The `BP_DefaultPawn` used by this project can be found here, along with the `BP_Movement` component.
* **Data**: This contains the data assets used by the project. The two `DA_` assets are GMAS-defined formats, while the `DT_` is an Unreal data table defining what gameplay tags are available.
* **Effects**: This directory contains blueprint implementations of all the GMAS gameplay effects used by this demo (and some which are not).
* **Input**: This template is built atop Unreal's newer "Enhanced Input" system; all the Input Actions (and the default Input Mapping Context) can be found in this directory.
* **LevelPrototyping**: This just contains all the building blocks of the oh-so-familiar default Unreal 5 map.
* **Maps**: This contains the project's copy of the aforementioned default Unreal 5 map.
* **Particles**: This contains some Niagara particle systems utilized by gameplay effects (though at present none of those effects are used in the template).

## Common Questions

### Where is input being handled?

For the main movement and camera controls, GMCv2 can handle the basics internally. If you look at the `BP_Movement` blueprint component, you'll see that `IA_Look` and `IA_Movement` are set in the two input action properties.

The actual mapping that defines what those inputs are bound to is in the `IMC_Default` file in the Input folder; if you look at `BP_DefaultPawn` you'll see that this project's pawn will automatically use an input mapping context if one is set in its properties, and the default pawn is set to use `IMC_Default`.

You will also see a number of `IA_<whatever>` events in the default pawn; these events are called when a given input action is triggered. This is how the Jump, Sprint, and Roll abilities get called.