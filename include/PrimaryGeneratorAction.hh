#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh

#include <G4VUserPrimaryGeneratorAction.hh>

class PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
  public:
    virtual void GeneratePrimaries(G4Event*) { };
};

#endif

