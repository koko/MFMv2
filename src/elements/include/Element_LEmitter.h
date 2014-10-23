/*                                              -*- mode:C++ -*-
						Element_Light.h Unidirectional propagator for use as data in Sensor Array project.

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
  \file Light.h, a pair of atoms which travels in one direction indefinitely.
  \author Ronnie Garduno
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef LEMITTER_H
#define LEMITTER_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P3Atom.h"

#include "Element_Light.h"

namespace MFM
{

  #define ARRAY_VERSION 1

  template <class CC>
  class Element_LEmitter : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;

    enum {
      R = P::EVENT_WINDOW_RADIUS,
      BITS = P::BITS_PER_ATOM,
	//This parameter tells the Light atom which direction it should propagate in, with Left being 0 and the other directions assigned the next value in clockwise order.
	DIRECTION = P3Atom<P>::P3_STATE_BITS_POS,
	DIR_LEN   = 3
    };

	typedef BitField<BitVector<BITS>, DIR_LEN, DIRECTION> Direction;

    ElementParameterS32<CC> m_direction;
  private:
	
	s32 GetDirection(const T& us) const{
		return Direction::Read(this->GetBits(us));
	}

	void SetDirection(T& us, const s32 direction) const{
		Direction::Write(this->GetBits(us), direction);
	}

  public:

    static Element_LEmitter THE_INSTANCE;
    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    Element_LEmitter() 
      : Element<CC>(MFM_UUID_FOR("LEmitter", ARRAY_VERSION)),
	m_direction(this, "direction", "Direction",
                     "This light crystal will travel in the given direction.",0,0,7,1)
    {
      Element<CC>::SetAtomicSymbol("Lm");
      Element<CC>::SetName("Light Emitter");
    }

    virtual const T & GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);
	this->SetDirection(defaultAtom,m_direction.GetValue());
      return defaultAtom;
    }


    virtual u32 GetSymI(T &atom, EventWindow<CC>& window) const
    {
      return (u32) PSYM_NORMAL;
    }


    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 0;
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xff11bb33;
    }

    virtual u32 LocalPhysicsColor(const T& atom, u32 selector) const
    {
      return DefaultPhysicsColor();
    }

	T& NewAtomWithDir(s32 dirVal) const{
		static T newAtom(TYPE(),0,0,0);
		SetDirection(newAtom,dirVal);
		return newAtom;
	}

    virtual void Behavior(EventWindow<CC>& window) const
    {
//	const MDist<R> md = MDist<R>::get();
/*	const T& thisAtom = window.GetCenterAtom(); 
        int dir = 0;
	
	dir = this->GetDirection(thisAtom);
	T otherAtomForward;
	T otherAtomBackward;
	const SPoint& relLeft	= SPoint(-1,0);
	const SPoint& relUpL 	= SPoint(-1,-1);
	const SPoint& relUp	= SPoint(0,-1);
	const SPoint& relUpR	= SPoint(1,-1);
	const SPoint& relRight  = SPoint(1,0);
	const SPoint& relDownR	= SPoint(1,1);
	const SPoint& relDown   = SPoint(0,1);
	const SPoint& relDownL	= SPoint(-1,1);
*/
   }
  };

  template <class CC>
  Element_LEmitter<CC> Element_LEmitter<CC>::THE_INSTANCE;

}

#endif /* ELEMENT_LIGHT_H*/
