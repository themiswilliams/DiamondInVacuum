

#include "TrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<TrackerHit> TrackerHitAllocator;


TrackerHit::TrackerHit() {}

TrackerHit::~TrackerHit() {}


TrackerHit::TrackerHit(const TrackerHit& right)
  : G4VHit()
{
  trackID   = right.trackID;
  edep      = right.edep;
  pos       = right.pos;
}


const TrackerHit& TrackerHit::operator=(const TrackerHit& right)
{
  trackID   = right.trackID;
  edep      = right.edep;
  pos       = right.pos;
  return *this;
}


G4int TrackerHit::operator==(const TrackerHit& right) const
{
  return (this==&right) ? 1 : 0;
}



void TrackerHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}



void TrackerHit::Print()
{
  G4cout << "  trackID: " << trackID << "  energy deposit: " << G4BestUnit(edep,"Energy")
	 << "  position: " << G4BestUnit(pos,"Length") << G4endl;
}



