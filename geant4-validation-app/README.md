
Geant4 validation app
=====================


# Dependencies
- Geant4
  - Qt5
  - OpenGL
  - XercesC
- ROOT
- nlohmann/json


# Build and run
```bash
$ mkdir build
$ cmake ..
$ make
$ ./g4-app input_example.json
```


# Usage
A json input file is used to set the simulation run and is shown below. Most 
of it is self-explanatory, but here is a short help:  

- The `root_output` field also works with a full path: `/path/to/out.root`.  
- The `export_gdml` field defines the exported geometry filename. If left blank,
the gdml will not be created.      
- Geant4's GUI does not work well with very large geometries (avoid visualizing
events with `cms2018.gdml`).  
- `events` is the number of events to be simulated.  
- `step_info` will not be stored to the root file if set to `false`.  
- `primary.energy` is in **[MeV]**.  
- `primary.vertex` is in **[cm]**.  
- `primary.direction` is in arbitrary units and it is normalized at
initialization.  

```json
{
    "root_output": "g4_out.root",
    "export_gdml": "",
    "GUI": true,
    "vis_macro": "vis.mac",
    "events": "10",
    "step_info": true,
    "cumulative":
    {
        "dedx": [100, 0, 100]
    },
    "primary": 
    {
        "pdg": 22,
        "energy": 10,
        "vertex": [0, 0, 0],
        "direction": [0, 0, 1]
    },
    "physics": 
    {
        "compton_scattering": true,
        "photoelectric": false,
        "rayleigh": false,
        "gamma_conversion": true,
        "positron_annihilation": true,
        "bremmstrahlung": false,
        "multiple_scattering": false,
        "e_ionization": false,
        "coulomb_scattering": false
    },
    "verbosity":
    {
        "RunManager": 0,
        "RunAction": 0,
        "EventAction": 0,
        "PhysicsList": 0,
        "PrintProgress": 0
    }
}
```


# Utils/
A couple ROOT macros are included for reading a ROOT output file and generate a
few example histograms. To run them:

```bash
$ root
root[0] .x macro.C("/path/to/out.root")
```


# Programmatic geometry details

- The `World volume` is a box, and its dimensions are expressed in cartesian
coordinates `[x. y, z]`.  
- **All** other volumes are concentric cylinders, and their dimensions are 
expressed as `[inner radius, outer radius, length]`
- A small distance between cylinder edges is defined to avoid overlapping and is
currently set to be `d = 1e-10`;

| Volume                       | Composition      | Dimensions [cm]    |
| ---------------------------- | ---------------- | ------------------ |
| World volume                 | H (G4_Galactic)  | [1000, 1000, 2000] |
| silicon tracker              | Si               | [30, 125, 1400]    |
| electromagnetic calorimeter  | Pb               | [125, 175, 1400]   |
| hadron calorimeter           | C                | [175, 275, 1400]   |
| superconducting solenoid     | Ti               | [275, 375, 1400]   |
| iron / muon chambers         | Fe               | [375, 700, 1400]   |


# Development notes

**Qt 5.15.2** on **macOS 11.4** segfaults if `(x)` window button is clicked.
Using session command `exit` works as expected.