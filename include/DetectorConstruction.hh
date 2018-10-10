#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh

#include <G4VUserDetectorConstruction.hh>

#include "CheckOverlap.hh"

class DetectorConstruction: public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(const G4String& gdmlfile, bool validate, bool overlap) {
      fGDMLValidate = validate;
      fGDMLOverlapCheck = overlap;
      SetGDMLFile(gdmlfile);
    };

    G4VPhysicalVolume* Construct() {
      G4cout << "Reading " << fGDMLFile << G4endl;
      G4cout << "- schema validation " << (fGDMLValidate? "on": "off") << G4endl;
      G4cout << "- overlap check " << (fGDMLOverlapCheck? "on": "off") << G4endl;
      // Change directory
      char cwd[MAXPATHLEN];
      if (!getcwd(cwd,MAXPATHLEN)) {
        G4cerr << __FILE__ << " line " << __LINE__ << ": ERROR no current working directory" << G4endl;
        exit(-1);
      }
      if (chdir(fGDMLPath)) {
        G4cerr << __FILE__ << " line " << __LINE__ << ": ERROR cannot change directory" << G4endl;
        exit(-1);
      }

      // Parse GDML file
      G4GDMLParser parser;
      parser.SetOverlapCheck(fGDMLOverlapCheck);
      parser.Read(fGDMLFile, fGDMLValidate);
      G4VPhysicalVolume* worldvolume = parser.GetWorldVolume();

      if (fGDMLOverlapCheck) CheckOverlaps(worldvolume);

      // Change directory back
      if (chdir(cwd)) {
        G4cerr << __FILE__ << " line " << __LINE__ << ": ERROR cannot change directory" << G4endl;
        exit(-1);
      }
      return worldvolume;
    }

    void CheckOverlaps(G4VPhysicalVolume* volume) {
      CheckOverlap check;
      check.CheckOverlaps(volume);
    }

  private:
    G4bool fGDMLValidate;
    G4bool fGDMLOverlapCheck;
    G4String fGDMLPath;
    G4String fGDMLFile;
    void SetGDMLFile(G4String gdmlfile) {
      size_t i = gdmlfile.rfind('/');
      if (i != std::string::npos) {
        fGDMLPath = gdmlfile.substr(0,i);
      } else fGDMLPath = ".";
      fGDMLFile = gdmlfile.substr(i + 1);
    }
};

#endif

