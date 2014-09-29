/*
  Element_Lens.h Definition of a Lens element in the Data Array project.
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
  \file   Element_Lens.h Element which focuses an antenna, as well as building it.
  \author Ronnie J. H. Garduno
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_LENS_H
#define ELEMENT_LENS_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"

namespace MFM
{

  #define ARRAY_VERSION 1

  template <class CC>
  class Element_Lens : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum {
	 R = P::EVENT_WINDOW_RADIUS,
	 BITS = P::BITS_PER_ATOM
	 };

  private:

    ElementParameterS32<CC> m_antennaLength;

    /* <<TEMPLATE>> Add any configurable parameters here. */

  public:

    static Element_Lens<CC> THE_INSTANCE;

    Element_Lens()
      : Element<CC>(MFM_UUID_FOR("Lens", ARRAY_VERSION)),
        /* <<TEMPLATE>> Initialize all configurable parameters here. */
        m_antennaLength(this, "length", "Antenna length",
                  "This parameter regulates the length of the two antenna arms which the lens builds and uses.", 1, 5, 10, 1)
    {
      Element<CC>::SetAtomicSymbol("Ln");
      Element<CC>::SetName("Lens");
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 0;
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xffffffff;
    }

    /*
       <<TEMPLATE>> This color will be the color rendered when your
                    element is rendered in lowlighting.
     */
    virtual u32 DefaultLowlightColor() const
    {
      return 0xff777777;
    }

    virtual const char* GetDescription() const
    {
      return "The Lens object builds an Antenna structure with itself as the focus, then uses it to redirect Light data elements into the Sensor Walls around it.";
    }

    /*
      <<TEMPLATE>> This method is executed every time an atom of your
                   element is chosen for an event. See the tutorial in
                   the wiki for further information.
     */
    virtual void Behavior(EventWindow<CC>& window) const
    {
    }
  };

  /*
     <<TEMPLATE>> Rename the class names here to the class name of your element.
  */
  template <class CC>
  Element_Lens<CC> Element_Lens<CC>::THE_INSTANCE;

}

#endif 
