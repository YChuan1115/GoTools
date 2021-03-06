/*
 * Copyright (C) 1998, 2000-2007, 2010, 2011, 2012, 2013 SINTEF ICT,
 * Applied Mathematics, Norway.
 *
 * Contact information: E-mail: tor.dokken@sintef.no                      
 * SINTEF ICT, Department of Applied Mathematics,                         
 * P.O. Box 124 Blindern,                                                 
 * 0314 Oslo, Norway.                                                     
 *
 * This file is part of GoTools.
 *
 * GoTools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version. 
 *
 * GoTools is distributed in the hope that it will be useful,        
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with GoTools. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In accordance with Section 7(b) of the GNU Affero General Public
 * License, a covered work must retain the producer line in every data
 * file that is created or manipulated using GoTools.
 *
 * Other Usage
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities involving the GoTools library without
 * disclosing the source code of your own applications.
 *
 * This file may be used in accordance with the terms contained in a
 * written agreement between you and SINTEF ICT. 
 */

#ifndef _INTERSECTOR2OBJ_H
#define _INTERSECTOR2OBJ_H


#include "GoTools/intersections/Intersector.h"
#include "GoTools/intersections/ParamGeomInt.h"


namespace Go {


/// This class is an abstract class providing an interface to the
/// intersection functionality in GoTools.

class Intersector2Obj : public Intersector {
public:

    /// Default constructor
    Intersector2Obj() {}

    /// Constructor.
    /// The last two variables are relevant only if the parent has one
    /// more parameter than the Intersector to be constructed.
    /// \param obj1 object of type ParamSurfaceInt, ParamCurveInt or
    /// ParamPointInt.
    /// \param obj2 object of type ParamSurfaceInt, ParamCurveInt or
    /// ParamPointInt.
    /// \param epsge the associated tolerance.
    /// \param prev the "parent" Intersector (0 if there is no
    /// parent).
    /// \param eliminated_parameter the index of the parameter that
    /// was removed from the parent \a prev.
    /// \param eliminated_value the value of the parameter that was
    /// removed from the parent \a prev.
    Intersector2Obj(shared_ptr<ParamGeomInt> obj1, 
		    shared_ptr<ParamGeomInt> obj2,
		    shared_ptr<GeoTol> epsge, 
		    Intersector* prev = 0,
		    int eliminated_parameter = -1,
		    double eliminated_value = 0);

    /// Constructor.
    /// The last two variables are relevant only if the parent has one
    /// more parameter than the Intersector to be constructed.
    /// \param obj1 object of type ParamSurfaceInt, ParamCurveInt or
    /// ParamPointInt.
    /// \param obj2 object of type ParamSurfaceInt, ParamCurveInt or
    /// ParamPointInt.
    /// \param epsge the associated tolerance.
    /// \param prev the "parent" Intersector (0 if there is no
    /// parent).
    /// \param eliminated_parameter the index of the parameter that
    /// was removed from the parent \a prev.
    /// \param eliminated_value the value of the parameter that was
    /// removed from the parent \a prev.
    Intersector2Obj(shared_ptr<ParamGeomInt> obj1, 
		    shared_ptr<ParamGeomInt> obj2,
		    double epsge, 
		    Intersector* prev = 0,
		    int eliminated_parameter = -1,
		    double eliminated_value = 0);

    /// Destructor.
    virtual ~Intersector2Obj();

//     // Validation of given intersection results
//     virtual void validate(int level, ValidationStat status);  @bsp

    /// Count the number of boundary objects belonging to the
    /// specified ParamGeomInt.
    /// \param idx refers to \a obj1 or \a obj2 (i.e. 0 or 1).
    /// \return The number of boundary objects.
    virtual int nmbBdObj(int idx) const
    { return (idx < 0 || idx > 1) ? 0 : obj_int_[idx]->nmbBdObj(); }

    /// Get the specified boundary object belonging to the specified
    /// ParamGeomInt.
    /// \param idx refers to \a obj1 or \a obj2 (i.e. 0 or 1).
    /// \param bd_idx index of the boundary object in the
    /// ParamGeomInt.
    /// \return The boundary object.
    virtual BoundaryGeomInt* getBoundaryObject(int idx, int bd_idx) const 
    { return (idx < 0 || idx > 1) ? 0 
	  : obj_int_[idx]->getBoundaryObject(bd_idx); }

    /// Mark that this intersection is performed in self-intersection
    /// context.
    void setSelfintCase(int type)
    { selfint_case_ = type; }

    /// Check if this intersection is performed in self-intersection
    /// context.
    /// \return \c true if this is a self-intersection, \c false
    /// otherwise
    virtual int isSelfintCase()
    { return selfint_case_; }
	    
	
protected:
    shared_ptr<ParamGeomInt> obj_int_[2];
    int selfint_case_;

    // NB: The order of the objects ot input is not arbitrary!  The
    // knowledge of what is the 'first object' and the 'second object'
    // can be used internally, and must be consistent with the parent
    // Intersector.
    virtual void print_objs();

    virtual shared_ptr<Intersector> 
    lowerOrderIntersector(shared_ptr<ParamGeomInt> obj1,
			  shared_ptr<ParamGeomInt> obj2,
			  Intersector* prev = 0,
			  int eliminated_parameter = -1,
			  double eliminated_value = 0) = 0;

    virtual int getBoundaryIntersections();

    virtual int performInterception();

    virtual bool foundIntersectionNearBoundary()
    { return false; }  // Special implementation in cases involving
		       // curves

    virtual int performRotatedBoxTest(double eps1, double eps2);

    // @@@ TODO. Perform interception using implicitization.
    // Real implementation only in SfSfIntersector for the time being
    virtual int performInterceptionByImplicitization()
    { return 1; }

    // @@@ TODO. Perform interception using implicitization when two
    // surfaces intersect along a common boundary. This result is
    // found already.
    // Real implementation only in SfSfIntersector for the time being
     virtual int interceptionBySeparationSurface()
    { return 1; }

    virtual int simpleCase();

    virtual int simpleCase2(Point& axis1, Point& axis2);

    // @@@ TODO. Perform simple case test using implicitization.
    // Real implementation only in SfSfIntersector for the time being
    virtual int simpleCaseByImplicitization()
    { return 0; }

    virtual bool isLinear();

    virtual bool complexityReduced()
    {
	// Default behaviour, continue recursion
	return true;
    }

    virtual void handleComplexity()
    {
	// Default, do nothing. Must be impelemted together with 
	// complexityReduced
    }

    virtual int checkCoincidence() = 0;

    virtual void microCase() = 0;
    
    virtual int updateIntersections() = 0;

    virtual int linearCase() = 0;

    virtual int doSubdivide() = 0;

    virtual int complexIntercept();

    virtual int complexSimpleCase();

    virtual void postIterate(int nmb_orig, int dir=-1, bool keep_endpt=true) { }

    virtual void doPostIterate()
	{
	    postIterate(0, -1);
	}

    virtual void removeDegenerateConnections() { }

    void getSeedIteration(double seed[]);

    bool atSameBoundary(const double *par1, const double *par2);

    bool atBoundary(const double *par1, std::vector<bool>& boundaries);

    virtual void printDebugInfo();

    virtual void writeOut() { }

private:

};


}  // namespace Go


#endif  // _INTERSECTOR2OBJ_H
