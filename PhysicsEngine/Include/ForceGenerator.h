/*
* Interface file for the force generators.
*
* Part of the Cyclone physics system.
*
* Copyright (c) Icosagon 2003. All Rights Reserved.
*
* This software is distributed under license. Use of this software
* implies agreement with all terms and conditions of the accompanying
* software license.
*/

/**
* @file
*
* This file contains the interface and sample force generators.
*/
#ifndef CYCLONE_FGEN_H
#define CYCLONE_FGEN_H

#include "RigidBody.h"
#include <vector>


/**
* A force generator can be asked to add a force to one or more
* bodies.
*/
class ForceGenerator
{
public:

	/**
	* Overload this in implementations of the interface to calculate
	* and update the force applied to the given rigid body.
	*/
	virtual void updateForce(RigidBody *body, real duration) = 0;
};

/**
* A force generator that applies an aerodynamic force.
*/
class Aero : public ForceGenerator
{
protected:
	/**
	* Holds the aerodynamic tensor for the surface in body
	* space.
	*/
	Matrix3 tensor;

	/**
	* Holds the relative position of the aerodynamic surface in
	* body coordinates.
	*/
	Vector3 position;

	/**
	* Holds a pointer to a vector containing the windspeed of the
	* environment. This is easier than managing a separate
	* windspeed vector per generator and having to update it
	* manually as the wind changes.
	*/
	const Vector3* windspeed;

public:
	/**
	* Creates a new aerodynamic force generator with the
	* given properties.
	*/
	Aero(const Matrix3 &tensor, const Vector3 &position,
		const Vector3 *windspeed);

	/**
	* Applies the force to the given rigid body.
	*/
	virtual void updateForce(RigidBody *body, real duration);

protected:
	/**
	* Uses an explicit tensor matrix to update the force on
	* the given rigid body. This is exactly the same as for updateForce
	* only it takes an explicit tensor.
	*/
	void updateForceFromTensor(RigidBody *body, real duration,
		const Matrix3 &tensor);
};

/**
* A force generator with a control aerodynamic surface. This
* requires three inertia tensors, for the two extremes and
* 'resting' position of the control surface.  The latter tensor is
* the one inherited from the base class, the two extremes are
* defined in this class.
*/
class AeroControl : public Aero
{
protected:
	/**
	* The aerodynamic tensor for the surface, when the control is at
	* its maximum value.
	*/
	Matrix3 maxTensor;

	/**
	* The aerodynamic tensor for the surface, when the control is at
	* its minimum value.
	*/
	Matrix3 minTensor;

	/**
	* The current position of the control for this surface. This
	* should range between -1 (in which case the minTensor value
	* is used), through 0 (where the base-class tensor value is
	* used) to +1 (where the maxTensor value is used).
	*/
	real controlSetting;

private:
	/**
	* Calculates the final aerodynamic tensor for the current
	* control setting.
	*/
	Matrix3 getTensor();

public:
	/**
	* Creates a new aerodynamic control surface with the given
	* properties.
	*/
	AeroControl(const Matrix3 &base,
		const Matrix3 &min, const Matrix3 &max,
		const Vector3 &position, const Vector3 *windspeed);

	/**
	* Sets the control position of this control. This * should
	range between -1 (in which case the minTensor value is *
	used), through 0 (where the base-class tensor value is used) *
	to +1 (where the maxTensor value is used). Values outside that
	* range give undefined results.
	*/
	void setControl(real value);

	/**
	* Applies the force to the given rigid body.
	*/
	virtual void updateForce(RigidBody *body, real duration);
};

/**
* Holds all the force generators and the bodies they apply to.
*/
class ForceRegistry
{
protected:

	/**
	* Keeps track of one force generator and the body it
	* applies to.
	*/
	struct ForceRegistration
	{
		RigidBody *body;
		ForceGenerator *fg;
	};

	/**
	* Holds the list of registrations.
	*/
	typedef std::vector<ForceRegistration> Registry;
	Registry registrations;

public:
	/**
	* Registers the given force generator to apply to the
	* given body.
	*/
	void add(RigidBody* body, ForceGenerator *fg);


	/**
	* Calls all the force generators to update the forces of
	* their corresponding bodies.
	*/
	void updateForces(real duration);
};


#endif // CYCLONE_FGEN_H