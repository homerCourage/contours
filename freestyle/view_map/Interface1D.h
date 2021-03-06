//
//  Filename         : Interface1D.h
//  Author(s)        : Emmanuel Turquin
//  Purpose          : Interface to 1D elts
//  Date of creation : 01/07/2003
//
///////////////////////////////////////////////////////////////////////////////


//
//  Copyright (C) : Please refer to the COPYRIGHT file distributed 
//   with this source distribution. 
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef  INTERFACE1D_H
# define INTERFACE1D_H

# include <string>
# include <iostream>
# include <float.h>
#include <set>
#include <assert.h>
# include "../system/Id.h"
# include "../system/Precision.h"
# include "../winged_edge/Nature.h"
# include "Functions0D.h"

using namespace std;
/*! \file Interface1D.h
 *  Interface1D and related tools definitions
 */
// Integration method
/*! The different integration
 *  methods that can be invoked
 *  to integrate into a single value the set of values obtained
 *  from each 0D element of a 1D element.
 */
typedef enum {
  MEAN,/*!< The value computed for the 1D element is the mean of the values obtained for the 0D elements.*/
  MIN,/*!< The value computed for the 1D element is the minimum of the values obtained for the 0D elements.*/
  MAX,/*!< The value computed for the 1D element is the maximum of the values obtained for the 0D elements.*/
  FIRST,/*!< The value computed for the 1D element is the first of the values obtained for the 0D elements.*/
  LAST/*!< The value computed for the 1D element is the last of the values obtained for the 0D elements.*/
} IntegrationType;

/*! Returns a single
 *  value from a set of values evaluated at each 0D element
 *  of this 1D element.
 *  \param fun
 *    The UnaryFunction0D used to compute a value at each Interface0D.
 *  \param it
 *    The Interface0DIterator used to iterate over the 0D elements of
 *    this 1D element. The integration will occur over the 0D elements
 *    starting from the one pointed by it.
 *  \param it_end
 *    The Interface0DIterator pointing the end of the 0D elements of the
 *    1D element.
 *  \param integration_type
 *    The integration method used to compute a single value from
 *    a set of values.
 *  \return the single value obtained for the 1D element.
 */
template <class T>
T integrate(UnaryFunction0D<T>& fun,
	    Interface0DIterator it,
	    Interface0DIterator it_end,
	    IntegrationType integration_type = MEAN) {
  T res;
  T res_tmp;
  unsigned size;
  switch (integration_type) {
  case MIN:
    res = fun(it);++it;
    for (; !it.isEnd(); ++it) {
      res_tmp = fun(it);
      if (res_tmp < res)
	res = res_tmp;
    }
    break;
  case MAX:
    res = fun(it);++it;
    for (; !it.isEnd(); ++it) {
      res_tmp = fun(it);
      if (res_tmp > res)
	res = res_tmp;
    }
    break;
  case FIRST:
    res = fun(it);
    break;
  case LAST:
    res = fun(--it_end);
    break;
  case MEAN:
  default:
    res = fun(it);++it;
    for (size = 1; !it.isEnd(); ++it, ++size)
      res += fun(it);
    res /= (size ? size : 1);
    break;
  }
  return res;
}

//
// Interface1D
//
//////////////////////////////////////////////////

/*! Base class for any 1D element. */
class Interface1D
{
private:

  static int _totalRefs;
  static int _livingRefs;
  static set<Interface1D*> _allI1Ds;
  static bool _erasingAllI1Ds;

public:
  /*! Default constructor */
  Interface1D() {_timeStamp=0; _totalRefs ++; _livingRefs++;     _allI1Ds.insert(this); }
    //    assert(_allI1Ds.find(this)==_allI1Ds.end()); // crazy test...
  virtual ~Interface1D();   
  static void printRefStats() { printf("**** I1D: totalRefs = %d, livingRefs = %d\n", _totalRefs, _livingRefs); }
  static void eraseAllI1Ds(); 

  /*! Returns the string "Interface1D" .*/
  virtual string getExactTypeName() const {
    return "Interface1D";
  }

  Interface1D(Interface1D &) { assert(0); } // copying would mess up the allI1Ds thing
  Interface1D(const Interface1D &) { assert(0); } // copying would mess up the allI1Ds thing

  // Iterator access

  /*! Returns an iterator over the Interface1D vertices,
   *  pointing to the first vertex.
   */
  virtual Interface0DIterator verticesBegin() = 0;
  /*! Returns an iterator over the Interface1D vertices,
   *  pointing after the last vertex.
   */
  virtual Interface0DIterator verticesEnd() = 0;
  /*! Returns an iterator over the Interface1D points,
   *  pointing to the first point. The difference with
   *  verticesBegin() is that here we can iterate over
   *  points of the 1D element at a any given sampling.
   *  Indeed, for each iteration, a virtual point is created.
   *  \param t
   *    The sampling with which we want to iterate over points of
   *    this 1D element.
   */
  virtual Interface0DIterator pointsBegin(float t=0.f) = 0;
  /*! Returns an iterator over the Interface1D points,
   *  pointing after the last point. The difference with
   *  verticesEnd() is that here we can iterate over
   *  points of the 1D element at a any given sampling.
   *  Indeed, for each iteration, a virtual point is created.
   *  \param t
   *    The sampling with which we want to iterate over points of
   *    this 1D element.
   */
  virtual Interface0DIterator pointsEnd(float t=0.f) = 0;

  // Data access methods

  /*! Returns the 2D length of the 1D element. */
  virtual real getLength2D() const {
    cerr << "Warning: method getLength2D() not implemented" << endl;
    return 0;
  }

  /*! Returns the Id of the 1D element .*/
  virtual Id getId() const {
    cerr << "Warning: method getId() not implemented" << endl;
    return Id(0, 0);
  }

  // FIXME: ce truc n'a rien a faire la...(c une requete complexe qui doit etre ds les Function1D)
  /*! Returns the nature of the 1D element. */
  virtual Nature::EdgeNature getNature() const {
    cerr << "Warning: method getNature() not implemented" << endl;
    return Nature::NO_FEATURE;
  }
  
  /*! Returns the time stamp of the 1D element. Mainly used for selection. */
  virtual unsigned getTimeStamp() const {
    return _timeStamp;
  }
  
  /*! Sets the time stamp for the 1D element. */
  inline void setTimeStamp(unsigned iTimeStamp){
    _timeStamp = iTimeStamp;
  }

protected:
  unsigned _timeStamp;
};

#endif // INTERFACE1D_H
