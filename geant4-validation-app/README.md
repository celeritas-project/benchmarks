
Geant4 validation app
=====================


# Dependencies
- Geant4
  - Qt
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

- If the `gdml` field is left blank, a programmatic geometry is loaded. 
Otherwise, the app will load the provided geometry.  
- The `root_output` field also works with a full path: `/path/to/out.root`.    
- Geant4's GUI does not work well with very large geometries (avoid visualizing
events with `cms2018.gdml`).  
- `events` is the number of events to be simulated.  
- `step_info` will not be stored to the root file if set to `false`.  
- `primary.energy` is in **[MeV]**.  
- `primary.vertex` is in **[cm]**.  
- `primary.direction` is in arbitrary units, as it is normalized during
initialization.  

```json
{
    "gdml": "",
    "root_output": "out.root",
    "GUI": true,
    "vis_macro": "vis.mac",
    "events": "10",
    "step_info": true,
    "primary": {
        "pdg": 22,
        "energy": 10,
        "vertex": [0, 0, 0],
        "direction": [0, 0, 1]
    },
    "physics": {
        "verbosity": 1,
        "list": {
            "compton_scattering": true,
            "photoelectric": false,
            "rayleigh": false,
            "gamma_conversion": true,
            "positron_annihilation": true,
            "bremmstrahlung": false,
            "multiple_scattering": false,
            "e_ionization": false,
            "coulomb_scattering": false
        }
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

# Notes
Although this requires recompiling the code, one can tweak the programmatic
geometry by updating `DetectorConstruction` and use
`DetectorConstruction::export_gdml("geo.gdml")` to export said geometry to
a GDML file.
