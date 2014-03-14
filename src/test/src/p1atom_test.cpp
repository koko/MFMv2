#include "assert.h"
#include "EventWindow.h"
#include "p1atom_test.h"
#include "P1Atom.h"
#include "MDist.h"


namespace MFM {


void P1AtomTest::Test_p1atomState()
{
  P1Atom atom(32,0,0,0);

  assert(atom.GetType() == 32);
  
  atom = P1Atom(15,0,0,0);
  
  assert(atom.GetType() == 15);
}

void P1AtomTest::Test_p1atomLBCount()
{
  P1Atom atom(1,1,0,0);

  assert(atom.GetLongBondCount() == 1);
}

void P1AtomTest::Test_p1atomSBCount()
{
  P1Atom atom(1,0,2,0);
  SPoint offset(1, -1);

  //atom.AddShortBond(offset);
  //atom.AddShortBond(offset);

  assert(atom.GetShortBondCount() == 2);
}

void P1AtomTest::Test_p1atomReadBody()
{
  /*
  u32 blocks[2];
  P1Atom atom(0,4,4,0);

  SPoint points[8];
  u8 pvals[8];
  
  points[0].Set(0, 1);
  points[1].Set(0, 2);
  points[2].Set(-2, 0);
  points[3].Set(-1, -1);
  points[4].Set(2, 1);
  points[5].Set(2, 0);
  points[6].Set(-2, 0);
  points[7].Set(1, 3);

  for(int i = 0; i < 8; i++)
  {

    pvals[i] = MDist<4>::get().FromPoint(points[i], i);
  }

  for(int i = 0; i < 8; i++)
  {
    if(i < 4)
    {
      assert(atom.SetShortBond(i,points[i]));
    }
    else
    {
      assert(atom.SetLongBond(i-4,points[i]));
    }
  }

  atom.ReadVariableBodyInto(blocks);
  
  BitVector<64> bf(blocks);

  //bf.Remove(0, 16);

  for(int i = 0; i < 4; i++)
  {
    assert(bf.Read(16+i * 8, 8) == pvals[i + 4]);
    assert(bf.Read(16+32 + i * 4, 4) == pvals[i]);
  }
  */
}

void P1AtomTest::Test_p1atomAddLB()
{
  SPoint lbonds[4];
  P1Atom atom(1,4,0,0);
  
  lbonds[0].Set(0, 1);
  lbonds[1].Set(0, 2);
  lbonds[2].Set(0, 3);
  lbonds[3].Set(-4, 0);

  for(int i = 0; i < 4; i++)
  {
    assert(atom.SetLongBond(i,lbonds[i]));
    SPoint pt;
    assert(atom.GetLongBond(i,pt));
    assert(pt == lbonds[i]);
  }

  assert(atom.GetLongBondCount() == 4);
}

// XXX DEPRECATED:  # of short or long bonds should not change when type is same!
void P1AtomTest::Test_p1atomAddSB()
{
  SPoint sbond(1, 1);
  SPoint out;
  P1Atom atom(32,0,10,0);

  for(int i = 0; i < 10; i++)
  {
    assert(atom.SetShortBond(i,sbond));
    SPoint pt;
    assert(atom.GetShortBond(i,pt));
    assert(pt==sbond);
  }

  assert(atom.GetShortBondCount() == 10);
}

#if 0
// XXX DEPRECATED:  # of short or long bonds should not change when type is same!
void P1AtomTest::Test_p1atomRemoveLB()
{
  SPoint lbond1(2, 2);
  SPoint lbond2(3, 1);
  P1Atom atom(12,0,0,0);

  atom.AddLongBond(lbond1);
  atom.AddLongBond(lbond2);

  atom.RemoveLongBond(0);

  assert(atom.GetLongBondCount() == 1);

  u32 bits[2];
  atom.ReadVariableBodyInto(bits);
  
  BitVector<64> bf(bits);

  bf.Remove(0, 16);
  
  assert(bf.Read(8, 8) == 0);
  assert(bf.Read(16, 32) == 0);
  
}

void P1AtomTest::Test_p1atomRemoveSB()
{
  SPoint sb1(0, 2);
  SPoint sb2(1, -1);
  P1Atom atom(9,0,0,0);

  atom.AddShortBond(sb1);
  atom.AddShortBond(sb2);

  atom.RemoveShortBond(0);

  assert(atom.GetShortBondCount() == 1);

  u32 bits[2];
  atom.ReadVariableBodyInto(bits);

  BitVector<64> bf(bits);

  bf.Remove(0, 16);

  assert(bf.Read(4, 4) == 0);
  assert(bf.Read(8, 8) == 0);
  assert(bf.Read(16, 32) == 0);

  atom.RemoveShortBond(0);

  assert(atom.GetShortBondCount() == 0);

  atom.ReadVariableBodyInto(bits);

  assert(bits[0] == 0);
  assert(bits[1] == 0);

  atom.AddLongBond(sb1);
  atom.AddShortBond(sb2);
  atom.AddShortBond(sb1);

  atom.RemoveShortBond(0);

  atom.ReadVariableBodyInto(bits);

  assert((bits[0] & 0xf) == 0);
  assert(bits[1] == 0);
}
#endif

} /* namespace MFM */

