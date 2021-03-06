//
//  Filename         : CurveIterators.h
//  Author(s)        : Stephane Grabli
//  Purpose          : Iterators used to iterate over the elements of the Curve
//  Date of creation : 01/08/2003
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

#ifndef  CURVEITERATORS_H
# define CURVEITERATORS_H

#include "Stroke.h"
#include "Curve.h"

namespace CurveInternal {

  /*! iterator on a curve. Allows an iterating outside 
   *  initial vertices. A CurvePoint is instanciated an returned 
   *  when the iterator is dereferenced.
   */

  class CurvePointIterator : public Interface0DIteratorNested
  { 
  public:
    friend class ::Curve; 
  public:
    float _CurvilinearLength;
    float _step;
    ::Curve::vertex_container::iterator __A;
    ::Curve::vertex_container::iterator __B;
    ::Curve::vertex_container::iterator _begin;
    ::Curve::vertex_container::iterator _end;
    int _n;
    int _currentn;
    float _t;
    mutable CurvePoint _Point;
    float _CurveLength;

  public:
    
  public:
    inline CurvePointIterator(float step = 0.f)
      : Interface0DIteratorNested()
    {
      _step = step;
      _CurvilinearLength = 0.f;
      _t = 0.f;
      //_Point = 0;
      _n = 0;
      _currentn = 0;
      _CurveLength=0;
    }
    
    inline CurvePointIterator(const CurvePointIterator& iBrother)
      : Interface0DIteratorNested()
    {
      __A = iBrother.__A;
      __B = iBrother.__B;
      _begin = iBrother._begin;
      _end = iBrother._end;
      _CurvilinearLength = iBrother._CurvilinearLength;
      _step = iBrother._step;
      _t = iBrother._t;
      _Point = iBrother._Point;
      _n = iBrother._n;
      _currentn = iBrother._currentn;
      _CurveLength = iBrother._CurveLength;
    }
    inline CurvePointIterator& operator=(const CurvePointIterator& iBrother)
    {
      __A = iBrother.__A;
      __B = iBrother.__B;
      _begin = iBrother._begin;
      _end = iBrother._end;
      _CurvilinearLength = iBrother._CurvilinearLength;
      _step = iBrother._step;
      _t = iBrother._t;
      _Point = iBrother._Point;
      _n = iBrother._n;
      _currentn = iBrother._currentn;
      _CurveLength = iBrother._CurveLength;
      return *this;
    }
    virtual ~CurvePointIterator()
    {
    }
  protected:
    inline CurvePointIterator(::Curve::vertex_container::iterator iA, 
                              ::Curve::vertex_container::iterator iB, 
                              ::Curve::vertex_container::iterator ibegin, 
                              ::Curve::vertex_container::iterator iend,
                              int currentn,
      int n,
      float iCurveLength,
      float step, float t=0.f, float iCurvilinearLength = 0.f)
      : Interface0DIteratorNested()
    {
      __A = iA;
      __B = iB;
      _begin = ibegin;
      _end = iend;
      _CurvilinearLength = iCurvilinearLength;
      _step = step;
      _t = t;
      _n = n;
      _currentn = currentn;
      _CurveLength = iCurveLength;
    }
 
  public:

    virtual CurvePointIterator* copy() const {
      return new CurvePointIterator(*this);
    }

    inline Interface0DIterator CastToInterface0DIterator() const{
      Interface0DIterator ret(new CurveInternal::CurvePointIterator(*this));
      return ret;
    }
    virtual string getExactTypeName() const {
      return "CurvePointIterator";
    }
    
    // operators
    inline CurvePointIterator& operator++()  // operator corresponding to ++i
    { 
      increment();
      return *this;
    }
   
    inline CurvePointIterator& operator--()  // operator corresponding to ++i
    { 
      decrement();
      return *this;
    }
    
    // comparibility
    virtual bool operator==(const Interface0DIteratorNested& b) const
    {
      const CurvePointIterator* it_exact = dynamic_cast<const CurvePointIterator*>(&b);
      if (!it_exact)
	  return false;
      return ((__A==it_exact->__A) && (__B==it_exact->__B) && (_t == it_exact->_t));
    }
    
    // dereferencing
    virtual CurvePoint& operator*()  
    {
      return (_Point = CurvePoint(*__A,*__B,_t));
    }
    virtual CurvePoint* operator->() { return &(operator*());}
  public:
    virtual bool isBegin() const 
    {
      if((__A == _begin) && (_t < (float)M_EPSILON))
        return true;
      return false;
    }
    virtual bool isEnd() const 
    {
      if(__B == _end)
        return true;
      return false;
    }
  protected:
    virtual void increment() 
    {
      if((_currentn == _n-1) && (_t == 1.f))
      {
        // we're setting the iterator to end
        ++__A;
        ++__B;
        ++_currentn;
        _t = 0.f;
        return;
      }
      
      if(0 == _step) // means we iterate over initial vertices
      {
	      Vec3r vec_tmp((*__B)->point2d() - (*__A)->point2d());
        _CurvilinearLength += (float)vec_tmp.norm();
        if(_currentn == _n-1)
        { 
          _t = 1.f;   
          return;
        }
        ++__B;
        ++__A;
        ++_currentn;
        return;
      }
      
      // compute the new position:
      Vec3r vec_tmp2((*__A)->point2d() - (*__B)->point2d());
      float normAB = (float)vec_tmp2.norm();

      if(normAB > M_EPSILON)
      {
        _CurvilinearLength += _step;
        _t = _t + _step/normAB;
      }
      else
        _t = 1.f; // AB is a null segment, we're directly at its end
        //if normAB ~= 0, we don't change these values
      if(_t >= 1)
      { 
        _CurvilinearLength -= normAB*(_t-1);
        if(_currentn == _n-1)
          _t=1.f;
        else
        {
          _t = 0.f;
          ++_currentn;
          ++__A;++__B;
        }
      }
    }
    virtual void decrement() 
    {
      if(_t == 0.f) //we're at the beginning of the edge
      {
        _t = 1.f;
        --_currentn;
        --__A; --__B;
        if(_currentn == _n-1)
          return;
      }

      if(0 == _step) // means we iterate over initial vertices
      {
	      Vec3r vec_tmp((*__B)->point2d() - (*__A)->point2d());
        _CurvilinearLength -= (float)vec_tmp.norm();
        _t = 0;
        return;
      }
      
      // compute the new position:
      Vec3r vec_tmp2((*__A)->point2d() - (*__B)->point2d());
      float normAB = (float)vec_tmp2.norm();
      
      if(normAB >M_EPSILON)
      {
        _CurvilinearLength -= _step;
        _t = _t - _step/normAB;
      }
      else
        _t = -1.f; // We just need a negative value here

      // round value
      if(fabs(_t) < (float)M_EPSILON)
        _t = 0.0;
      if(_t < 0)
      { 
        if(_currentn == 0)
          _CurvilinearLength = 0.f;
        else
        _CurvilinearLength += normAB*(-_t);
        _t = 0.f;
      } 
    }

    virtual float t() const{
      return _CurvilinearLength;
    } 
    virtual float u() const{
      return _CurvilinearLength/_CurveLength;
    } 
  };

  

} // end of namespace StrokeInternal

#endif // CURVEITERATORS_H
