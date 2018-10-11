# gdmlview

Simple gdml viewer based on the Geant4 libraries

# Usage

```
Allowed options:
  -h [ --help ]           Produce this help message
  -g [ --gdmlfile ] arg   top level gdml file
  -v [ --validate ]       enable schema validation
  -o [ --overlap ]        enable overlap check
  -t [ --tolerance ] arg  overlap tolerance in mm
  -r [ --resolution ] arg overlap resolution as int
```

# Installation

```
mkdir build
cd build
cmake ..
make
make install
```

# Requirements

* Geant4 with G4UIQt and GDML support
* Boost program options
