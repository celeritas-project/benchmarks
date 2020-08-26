Klein-Nishina benchmark
-----------------------

# About

Short code to compare Celeritas' Klein-Nishina interaction class with
Geant4.

# Dependencies

- Geant4 with
  - Qt
  - OpenGL
  - XercesC

# Characteristics

## Geometry
Geometry is a 100 km iron cube, making it effectively infinite.

## Physics
Available particles:
- Gamma
- Electron
- Positron

Available processes:
- Transport (all)
- KleinNishinaCompton (gamma only)

No energy cut added for electrons and positrons. Since only transport
was assigned to them, they can't undergo any physics process.

## Output
ROOT output file with step and vertex information.
