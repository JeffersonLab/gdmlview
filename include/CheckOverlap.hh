#ifndef CheckOverlap_hh
#define CheckOverlap_hh

#include <G4VPhysicalVolume.hh>
#include <G4UnitsTable.hh>

class CheckOverlap {
  public:
    CheckOverlap() { };
    ~CheckOverlap() { };
  public:
    G4bool CheckOverlaps(G4VPhysicalVolume* volume);
    G4bool CheckOverlaps(G4VPhysicalVolume* volume, G4int res, G4double tol, G4bool verbose);
};

G4bool CheckOverlap::CheckOverlaps(G4VPhysicalVolume* volume)
{
  G4bool check = volume->CheckOverlaps(1000, 0.0, false);
  for (int i = 0; i < volume->GetLogicalVolume()->GetNoDaughters(); i++)
    check = check && CheckOverlaps(volume->GetLogicalVolume()->GetDaughter(i));
  return check;
}

G4bool CheckOverlap::CheckOverlaps(G4VPhysicalVolume* volume, G4int res, G4double tol, G4bool verbose)
{
  if (res <= 0) { return false; }

  G4VSolid* solid = volume->GetLogicalVolume()->GetSolid();
  G4LogicalVolume* motherLog = volume->GetMotherLogical();
  if (!motherLog) { return false; }

  G4VSolid* motherSolid = motherLog->GetSolid();

  if (verbose)
  {
    G4cout << "Checking overlaps for volume " << volume->GetName() << " ... ";
  }

  // Create the transformation from daughter to mother
  //
  G4AffineTransform Tm( volume->GetRotation(), volume->GetTranslation() );

  for (G4int n=0; n<res; n++)
  {
    // Generate a random point on the solid's surface
    //
    G4ThreeVector point = solid->GetPointOnSurface();

    // Transform the generated point to the mother's coordinate system
    //
    G4ThreeVector mp = Tm.TransformPoint(point);

    // Checking overlaps with the mother volume
    //
    if (motherSolid->Inside(mp) == kOutside)
    {
      G4double distin = motherSolid->DistanceToIn(mp);
      if (distin > tol)
      {
        std::ostringstream message;
        message << "Overlap with mother volume !" << G4endl
                << "          Overlap is detected for volume "
                << volume->GetName() << G4endl
                << "          with its mother volume "
                << motherLog->GetName() << G4endl
                << "          at mother local point " << mp << ", "
                << "overlapping by at least: "
                << G4BestUnit(distin, "Length");
        G4Exception("G4PVPlacement::CheckOverlaps()",
                    "GeomVol1002", JustWarning, message);
        return true;
      }
    }

    // Checking overlaps with each 'sister' volume
    //
    for (G4int i=0; i<motherLog->GetNoDaughters(); i++)
    {
      G4VPhysicalVolume* daughter = motherLog->GetDaughter(i);

      if (daughter == volume) { continue; }

      // Create the transformation for daughter volume and transform point
      //
      G4AffineTransform Td( daughter->GetRotation(),
                            daughter->GetTranslation() );
      G4ThreeVector md = Td.Inverse().TransformPoint(mp);

      G4VSolid* daughterSolid = daughter->GetLogicalVolume()->GetSolid();
      if (daughterSolid->Inside(md) == kInside)
      {
        G4double distout = daughterSolid->DistanceToOut(md);
        if (distout > tol)
        {
          std::ostringstream message;
          message << "Overlap with volume already placed !" << G4endl
                  << "          Overlap is detected for volume "
                  << volume->GetName() << G4endl
                  << "          with " << daughter->GetName() << " volume's"
                  << G4endl
                  << "          local point " << md << ", "
                  << "overlapping by at least: "
                  << G4BestUnit(distout,"Length");
          G4Exception("G4PVPlacement::CheckOverlaps()",
                      "GeomVol1002", JustWarning, message);
          return true;
        }
      }

      // Now checking that 'sister' volume is not totally included and
      // overlapping. Do it only once, for the first point generated
      //
      if (n==0)
      {
        // Generate a single point on the surface of the 'sister' volume
        // and verify that the point is NOT inside the current volume

        G4ThreeVector dPoint = daughterSolid->GetPointOnSurface();

        // Transform the generated point to the mother's coordinate system
        // and finally to current volume's coordinate system
        //
        G4ThreeVector mp2 = Td.TransformPoint(dPoint);
        G4ThreeVector msi = Tm.Inverse().TransformPoint(mp2);

        if (solid->Inside(msi) == kInside)
        {
          std::ostringstream message;
          message << "Overlap with volume already placed !" << G4endl
                  << "          Overlap is detected for volume "
                  << volume->GetName() << G4endl
                  << "          apparently fully encapsulating volume "
                  << daughter->GetName() << G4endl
                  << "          at the same level !";
          G4Exception("G4PVPlacement::CheckOverlaps()",
                      "GeomVol1002", JustWarning, message);
          return true;
        }
      }
    }
  }

  if (verbose)
  {
    G4cout << "OK! " << G4endl;
  }

  return false;
}

#endif

