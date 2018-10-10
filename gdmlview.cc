#include "G4Types.hh"
#include "G4Version.hh"

#include "G4GDMLParser.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIQt.hh"
#include "G4VisExecutive.hh"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <string>
using std::string;

#include <sys/param.h>

#include "PhysicsList.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

int main(int argc, char** argv)
{
  bool validate = false;
  bool overlap = false;
  string gdmlfile;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Produce this help message")
    ("gdmlfile,g", po::value<string>(&gdmlfile), "top level gdml file")
    ("validate,v", po::bool_switch(&validate), "enable schema validation")
    ("overlap,o",  po::bool_switch(&overlap),  "enable overlap check")
  ;
  po::positional_options_description p;
  p.add("gdmlfile", -1);

  // Parse command line options
  po::variables_map povm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), povm);
  po::notify(povm);

  if (povm.count("h") || gdmlfile.size() == 0) {
    G4cout << desc << G4endl;
    return 1;
  }

  // Run manager
  G4RunManager* rm = new G4RunManager;
  rm->SetUserInitialization(new DetectorConstruction(gdmlfile,validate,overlap));
  rm->SetUserInitialization(new PhysicsList);
  rm->SetUserAction(new PrimaryGeneratorAction);
  rm->Initialize();

  // Visualization
  G4VisManager* vm = new G4VisExecutive("quiet");
  vm->Initialize();

  // Start user interface
  G4UIQt* ui = new G4UIQt(argc, argv);
  ui->GetUserInterfaceWidget()->setVisible(false);
  ui->GetCoutDockWidget()->setVisible(false);
  G4UImanager* um = G4UImanager::GetUIpointer();
  um->ApplyCommand("/vis/open OGLSQt 1200x800");
  um->ApplyCommand("/vis/drawVolume worlds");
  ui->SessionStart();

  delete ui; delete vm; delete rm;
  return 0;
}
