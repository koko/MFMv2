/*
  Element_Box.h Definition of a box element in the Data Array project.

  Copyright (C) 2014 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
  USA
*/

/**
  \file   Element_Box.h Element which acts as surrounding sensing container for the array.
  \author Ronnie J. H. Garduno
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_BOX_H
#define ELEMENT_BOX_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"

namespace MFM
{

  #define ARRAY_VERSION 1

  template <class CC>
  class Element_Box : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
	//Right now, there is one parameter that tells the box atom whether or not it's exploring or building the box walls.
	//Another will tell it which way to go, maybe.
    enum {
	 R = P::EVENT_WINDOW_RADIUS,
	 BITS = P::BITS_PER_ATOM,

	 BOX_BUILDING  = P3Atom<P>::P3_STATE_BITS_POS,
	 BLD_PARAM_LEN = 3
	//We'll use a 0 to mean "not building"
	//& a 1 to mean "building down",
	//  a 2 to mean "building left",
//TODO: figure out parameters, if any
	 };
	
	typedef BitField<BitVector<BITS>, BLD_PARAM_LEN,BOX_BUILDING> IsBuilding;	

  private:

	s32 GetIsBuilding(const T& us) const{
		return IsBuilding::Read(this->GetBits(us));
	}

	void SetIsBuilding(T& us, const s32 building) const{
		IsBuilding::Write(this->GetBits(us), building);
	}

  public:

    static Element_Box<CC> THE_INSTANCE;

	static const u32 TYPE()
	{
		return THE_INSTANCE.GetType();
	}
	virtual const T& GetDefaultAtom() const{
		static T defaultAtom(TYPE(),0,0,0);
		SetIsBuilding(defaultAtom,0);
		//By default, box atoms are busy travelling to the corner of the world, not building boxes.
		return defaultAtom;
	}

    Element_Box()
      : Element<CC>(MFM_UUID_FOR("Box", ARRAY_VERSION))
        /* <<TEMPLATE>> Initialize all configurable parameters here. */
	//TODO: remove this if you remove the parameter it's assoc'd with
    {
      Element<CC>::SetAtomicSymbol("Bx");
      Element<CC>::SetName("Box");
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 0;
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0x99999999;
    }

    /*
       <<TEMPLATE>> This color will be the color rendered when your
                    element is rendered in lowlighting.
     */
    virtual u32 DefaultLowlightColor() const
    {
      return 0x99993333;

    }

    virtual const char* GetDescription() const
    {
      return "";
    }

	T& NewAtomIsBuilding(s32 isBuildingVal) const{
		static T newAtom(TYPE(),0,0,0);
		SetIsBuilding(newAtom,isBuildingVal);

		return newAtom;
	}

	//For Right Now, we'll focus on growing from the center outwards. The idea is that the box will keep moving itself and its brethren out until it hits the edge of the world.
	//TODO: Figure out how to see we've hit the edge!
	//It looks like the trick is to get a tile-specific coordinate for each space, and ask the Grid via IsLegaTileIndex.
	//Let's try to get it to move up to the top-right corner.
    virtual void Behavior(EventWindow<CC>& window) const
    {
	const MDist<R> md = MDist<R>::get();
	//We'll try to move there if it's a good spot and we're not 'done' (define that later!)
	//It looks like we can instead use window.IsLiveSite(rel)!
	T us = window.GetCenterAtom();
	const T& newAtom = Element_Box<CC>::THE_INSTANCE.GetDefaultAtom();
	const SPoint& rel = SPoint(1,-1); //This is the space to the up and right from our current atom's position.
	s32 isBuilding = GetIsBuilding(us);
//1 means down, 2 means left
	if (isBuilding == 1){
		//if it's building, let's move it down.
		//TODO: have this check for location!
		const SPoint& downRel = SPoint(0,1);
		const T& otherAtom = window.GetRelativeAtom(downRel);
		//if(window.IsLiveSite(downRel)){
		if(window.IsLiveSite(downRel) && otherAtom.GetType() == Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom().GetType()){
			T& newAtom = NewAtomIsBuilding(isBuilding);
			window.SetRelativeAtom(downRel,newAtom);
		}
	//If we're out of room to go down, go left.
		else if (!window.IsLiveSite(downRel)){
			s32 x = 2;
			T& newAtom = NewAtomIsBuilding(x);
			window.SetCenterAtom(newAtom);
		}
	}
	//this is building left
	else if (isBuilding == 2){
		const SPoint& leftRel = SPoint(-1,0);
		T& newAtom = NewAtomIsBuilding(isBuilding);
		if (window.IsLiveSite(leftRel))
			window.SetRelativeAtom(leftRel,newAtom);
		else{
		s32 x = 3;
		newAtom = NewAtomIsBuilding(x);
		window.SetCenterAtom(newAtom);
		}
	}
	else if (isBuilding == 3){
		const SPoint& upRel = SPoint(0,-1);
		T& newAtom = NewAtomIsBuilding(isBuilding);
		if (window.IsLiveSite(upRel))
			window.SetRelativeAtom(upRel,newAtom);
		else{
			s32 x = 4;
			newAtom = NewAtomIsBuilding(x);
			window.SetCenterAtom(newAtom);
		}
	}
	else if (isBuilding == 4){
		const SPoint& rightRel = SPoint(1,0);
		T& newAtom = NewAtomIsBuilding(isBuilding);
		if (window.IsLiveSite(rightRel))
			window.SetRelativeAtom(rightRel,newAtom);
	}
	else if (window.IsLiveSite(rel)){
		//const T& newAtom = Element_Box<CC>::THE_INSTANCE.GetDefaultAtom();
		window.SetRelativeAtom(rel,newAtom);
		window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
	}
	//OK, if we can't move both up and right, we've hit at least one edge. Let's see if we can move up some from here:
	else{
		const SPoint& rel2 = SPoint(0,-1);
		if (window.IsLiveSite(rel2)){
			window.SetRelativeAtom(rel2,newAtom);
			window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
		}
		//if we can't move up, we'll try right.
		else{
			const SPoint& rel3 = SPoint(1,0);
			if (window.IsLiveSite(rel3)){
				window.SetRelativeAtom(rel3,newAtom);
				window.SetCenterAtom(Element_Empty<CC>::THE_INSTANCE.GetDefaultAtom());
			}
	//In this case, the atom has reached the top-right corner of the world. Let's have it build a wall along those edges!
			else{
		//First up, we'll have this atom note that it is now Building. That'll let us handle this behaviour elsewhere...
				s32 x = 1;
				//SetIsBuilding(us,x);
				//IsBuilding::Write(this->GetBits(us), x);
				T& newAtom = NewAtomIsBuilding(x);
				window.SetCenterAtom(newAtom);
			}
		}
	}
    }
	
};
  template <class CC>
  Element_Box<CC> Element_Box<CC>::THE_INSTANCE;

}

#endif 
