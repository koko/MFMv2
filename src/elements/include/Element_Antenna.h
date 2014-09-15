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
  \file   Element_Antenna.h Element which acts as antenna in the array.
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

#include "Element_Lens.h"

namespace MFM
{

  #define ARRAY_VERSION 1

  template <class CC>
  class Element_Antenna : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum {
	 R = P::EVENT_WINDOW_RADIUS,
	 BITS = P::BITS_PER_ATOM,
	 ANTENNA_POS = P3Atom<P>::P3_STATE_BITS_POS,
	 ANTENNA_LEN = 4,
	 TOTAL_POS = ANTENNA_POS + ANTENNA_LEN,
	 TOTAL_LEN = 4 

//TODO: figure out parameters
// Here is where we'll remember which part of the antenna this atom is,
// As well as how long this particular antenna is. (Make global?)
//ANTENNA_LEN = 4 => max antenna size is 2^4 - 1 = 15. Might enlarge.
	 };
	
	typedef BitField<BitVector<BITS>, ANTENNA_LEN,ANTENNA_POS> AntIndex;	
	typedef BitField<BitVector<BITS>, TOTAL_LEN, TOTAL_POS> AntennaLength;

  private:
//TODO: consider whether or not to leave this as a global parameter.
    ElementParameterS32<CC> m_antennaLength;

	s32 GetIndex(const T& us) const{
		return AntIndex::Read(this->GetBits(us));
	}

	void SetIndex(T& us, const s32 index) const{
		AntIndex::Write(this->GetBits(us),index);
	}

	s32 GetAntennaLength(const T& us) const{
		return AntennaLength::Read(this->GetBits(us));
	}
	void SetAntennaLength(T& us, const s32 length) const{
		AntennaLength::Write(this->GetBits(us),length);
	}

  public:

    static Element_Antenna<CC> THE_INSTANCE;

	static const u32 TYPE()
	{
		return THE_INSTANCE.GetType();
	}
	virtual const T& GetDefaultAtom() const{
		static T defaultAtom(TYPE(),0,0,0);
		this->SetIndex(defaultAtom,1);
		this->SetAntennaLength(defaultAtom,5);
		return defaultAtom;
	}

    Element_Antenna()
      : Element<CC>(MFM_UUID_FOR("Antenna", ARRAY_VERSION)),
        /* <<TEMPLATE>> Initialize all configurable parameters here. */
	//TODO: remove this if you remove the parameter it's assoc'd with
        m_antennaLength(this, "length", "Antenna length",
                  "This parameter regulates the length of the two antenna arms which the lens builds and uses.", 1, 5, 10, 1)
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
      return 0xffffbbbb;
    }

    /*
       <<TEMPLATE>> This color will be the color rendered when your
                    element is rendered in lowlighting.
     */
    virtual u32 DefaultLowlightColor() const
    {
      return 0xff772222;
    }

    virtual const char* GetDescription() const
    {
      return "The antenna object is meant to be constructed into lines of antennae by Lens objects. Then, Light atoms striking the antennae from the front should bounce off.";
    }

	T& NewAtomWithIndex(s32 index) const{
		static T newAtom(TYPE(),0,0,0);
		SetIndex(newAtom,index);

		return newAtom;
	}

    /*
      <<TEMPLATE>> This method is executed every time an atom of your
                   element is chosen for an event. See the tutorial in
                   the wiki for further information.
     */
	/* OK, the basic idea of the antenna is that it should have a direction it points in, and it should 'bounce' light cells which run into it coming from that direction. Moreover, it should constantly check for the other antennae it should know about and which it can see in its event window, (re)generating them if it doesn't see them. Let's say, for now, that each neighbor should be 1 step above (below) the currentAtom, as well as 1 step to the right (left) of it, in a diagonal pattern.
	*/
	//For Right Now, we'll focus on growing from the 1st atom out.
    virtual void Behavior(EventWindow<CC>& window) const
    {
	//Let's only look at the md = 2 case right now...
//	const MDist<R> md = MDist<R>::get();
	const MDist<2> md = MDist<2>::get();
	
	//First we need to know if this is the 1st atom.
	const T& atom = window.GetCenterAtom();
	s32 ourIndex = AntIndex::Read(this->GetBits(atom));
	s32 antennaLength = AntennaLength::Read(this->GetBits(atom));
	
	//antennaLength = 5;
	//if not, we aren't coding your behavior right now, see ya
	//TODO: Fix that lack of behavior. (Note: this limits length to the event window of #1! oops. fix that soon.)
	//TODO: have this 'bounce' light for all antennae
	if (ourIndex != 1) {
	//	LOG.Debug("damned thing\n");
		if(antennaLength != 0){}
		return;
	}
	//else, you are #1! get to work.
	//TODO: get this to work for any direction, instead of just "directly right"

	for (s32 i = 1; i <= antennaLength; i++){
		const SPoint& rel = SPoint(i,-i);
		window.SetRelativeAtom(rel,Element_Antenna<CC>::THE_INSTANCE.GetDefaultAtom());

	}

		//s32 x = GetIndex(atom);
		//const T& newAtom = Element_Antenna<CC>::THE_INSTANCE.GetDefaultAtom();
		//SetIndex(newAtom,i+1);
		//T& newAtom = 
		//NewAtomWithIndex(x);
		//window.SetRelativeAtom(rel,newAtom);
		//const T& otherAtom = window.GetRelativeAtom(rel);
		//SetIndex(otherAtom,i+1);
//	}
    }
  };

  /*
     <<TEMPLATE>> Rename the class names here to the class name of your element.
  */
  template <class CC>
  Element_Antenna<CC> Element_Antenna<CC>::THE_INSTANCE;

}

#endif 
