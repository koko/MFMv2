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
  \file   Element_Antenna.h Element which acts as antenna walls for the array.
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
//TODO: comments, more parameters
    enum {
	 R = P::EVENT_WINDOW_RADIUS,
	 BITS = P::BITS_PER_ATOM,

	ANT_LENGTH  = P3Atom<P>::P3_STATE_BITS_POS,
	ANT_LEN_LEN = 4,
	ANT_DIR	    = ANT_LENGTH + ANT_LEN_LEN,
	ANT_DIR_LEN = 3

	//Let's use this to determine how far out the antenna builds. 
	//TODO: Add some direction bits to tell it which way to build out (just goes up and right now)
	 };
	
	typedef BitField<BitVector<BITS>, ANT_LEN_LEN, ANT_LENGTH> AntennaLength;
	typedef BitField<BitVector<BITS>, ANT_DIR_LEN, ANT_DIR> AntennaDirection;
	//typedef BitField<BitVector<BITS>, BLD_PARAM_LEN,BOX_BUILDING> IsBuilding;	

  private:

    	ElementParameterS32<CC> m_AntLen;

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
		SetAntennaLength(defaultAtom,1);
		return defaultAtom;
	}

    Element_Antenna()
      : Element<CC>(MFM_UUID_FOR("Antenna", ARRAY_VERSION)),
        m_AntLen(this, "length", "Antenna Length", "This parameter indicates the number of antenna atoms in a constructed antenna arm.", 1, 5, 16, 1)
        /* <<TEMPLATE>> Initialize all configurable parameters here. */
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

    virtual void Behavior(EventWindow<CC>& window) const
    {
//	const MDist<R> md = MDist<R>::get();
	const T& thisAtom = window.GetCenterAtom();
	s32 myLength = GetAntennaLength(thisAtom);
	//TODO: Get this using the slider value!
	if(myLength < m_AntLen.GetValue()){
		T& newAtom = NewAtomWithLength(myLength+1);
		const SPoint& rel = SPoint(1,-1);
		window.SetRelativeAtom(rel,newAtom);
	}
    }
	
};
  template <class CC>
  Element_Antenna<CC> Element_Antenna<CC>::THE_INSTANCE;

}

#endif 
