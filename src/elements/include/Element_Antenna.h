/*
  Element_Antenna.h Definition of an antenna element in the Data Array project.

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
#ifndef ELEMENT_ANTENNA_H
#define ELEMENT_ANTENNA_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"

namespace MFM
{

  #define ARRAY_VERSION 1

  template <class CC>
  class Element_Antenna : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
	//Right now, there is one parameter that tells the box atom whether or not it's exploring or building the box walls.
	//Another will tell it which way to go, maybe.
    enum {
	 R = P::EVENT_WINDOW_RADIUS,
	 BITS = P::BITS_PER_ATOM,

	 //BOX_BUILDING  = P3Atom<P>::P3_STATE_BITS_POS,
	 //BLD_PARAM_LEN = 3
	
	ANT_LENGTH  = P3Atom<P>::P3_STATE_BITS_POS,
	ANT_LEN_LEN = 3		

	//Let's use this to determine how far out the antenna builds. 
//TODO: figure out parameters, if any
	 };
	
	typedef BitField<BitVector<BITS>, ANT_LEN_LEN, ANT_LENGTH> AntennaLength;
	//typedef BitField<BitVector<BITS>, BLD_PARAM_LEN,BOX_BUILDING> IsBuilding;	

  private:

	s32 GetAntennaLength(const T& us) const{
		return AntennaLength::Read(this->GetBits(us));
	}

	void SetAntennaLength(T& us, const s32 length) const{
		AntennaLength::Write(this->GetBits(us), length);
	}

  public:

    static Element_Antenna<CC> THE_INSTANCE;

	static const u32 TYPE()
	{
		return THE_INSTANCE.GetType();
	}
	virtual const T& GetDefaultAtom() const{
		static T defaultAtom(TYPE(),0,0,0);
		//TODO: figure out how to use slider value for length!
		SetAntennaLength(defaultAtom,5);
		//By default, box atoms are busy travelling to the corner of the world, not building boxes.
		return defaultAtom;
	}

    Element_Antenna()
      : Element<CC>(MFM_UUID_FOR("Antenna", ARRAY_VERSION))
        /* <<TEMPLATE>> Initialize all configurable parameters here. */
	//TODO: remove this if you remove the parameter it's assoc'd with
    {
      Element<CC>::SetAtomicSymbol("An");
      Element<CC>::SetName("Antenna");
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 0;
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xFFF00000;
    }

    /*
       <<TEMPLATE>> This color will be the color rendered when your
                    element is rendered in lowlighting.
     */
    virtual u32 DefaultLowlightColor() const
    {
      return 0x77700000;

    }

    virtual const char* GetDescription() const
    {
      return "";
    }

	T& NewAtomWithLength(s32 lengthVal) const{
		static T newAtom(TYPE(),0,0,0);
		SetAntennaLength(newAtom,lengthVal);

		return newAtom;
	}

	//For Right Now, we'll focus on growing from the center outwards. The idea is that the box will keep moving itself and its brethren out until it hits the edge of the world.
	//TODO: Figure out how to see we've hit the edge!
	//It looks like the trick is to get a tile-specific coordinate for each space, and ask the Grid via IsLegaTileIndex.
	//Let's try to get it to move up to the top-right corner.
    virtual void Behavior(EventWindow<CC>& window) const
    {
//	const MDist<R> md = MDist<R>::get();
	
    }
	
};
  template <class CC>
  Element_Antenna<CC> Element_Antenna<CC>::THE_INSTANCE;

}

#endif 
