/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B2_PRISMATIC_JOINT_H
#define B2_PRISMATIC_JOINT_H

#include "b2Joint.h"

/// Prismatic joint definition. This requires defining a line of
/// motion using an axis and an anchor point. The definition uses local
/// anchor points and a local axis so that the initial configuration
/// can violate the constraint slightly. The joint translation is zero
/// when the local anchor points coincide in world space. Using local
/// anchors and a local axis helps when saving and loading a game.
/// @warning at least one body should by dynamic with a non-fixed rotation.
struct b2PrismaticJointDef : public b2JointDef
{
	b2PrismaticJointDef()
	{
		type = e_prismaticJoint;
		localAnchorA.SetZero();
		localAnchorB.SetZero();
		localAxis1.Set(1.0, 0.0);
		referenceAngle = 0.0;
		enableLimit = false;
		lowerTranslation = 0.0;
		upperTranslation = 0.0;
		enableMotor = false;
		maxMotorForce = 0.0;
		motorSpeed = 0.0;
	}

	/// Initialize the bodies, anchors, axis, and reference angle using the world
	/// anchor and world axis.
	void Initialize(b2Body* bodyA, b2Body* bodyB, const b2Vec2& anchor, const b2Vec2& axis);

	/// The local anchor point relative to body1's origin.
	b2Vec2 localAnchorA;

	/// The local anchor point relative to body2's origin.
	b2Vec2 localAnchorB;

	/// The local translation axis in body1.
	b2Vec2 localAxis1;

	/// The constrained angle between the bodies: body2_angle - body1_angle.
	b2decimal referenceAngle;

	/// Enable/disable the joint limit.
	bool enableLimit;

	/// The lower translation limit, usually in meters.
	b2decimal lowerTranslation;

	/// The upper translation limit, usually in meters.
	b2decimal upperTranslation;

	/// Enable/disable the joint motor.
	bool enableMotor;

	/// The maximum motor torque, usually in N-m.
	b2decimal maxMotorForce;

	/// The desired motor speed in radians per second.
	b2decimal motorSpeed;
};

/// A prismatic joint. This joint provides one degree of freedom: translation
/// along an axis fixed in body1. Relative rotation is prevented. You can
/// use a joint limit to restrict the range of motion and a joint motor to
/// drive the motion or to model joint friction.
class b2PrismaticJoint : public b2Joint
{
public:
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;

	b2Vec2 GetReactionForce(b2decimal inv_dt) const;
	b2decimal GetReactionTorque(b2decimal inv_dt) const;

	/// Get the current joint translation, usually in meters.
	b2decimal GetJointTranslation() const;

	/// Get the current joint translation speed, usually in meters per second.
	b2decimal GetJointSpeed() const;

	/// Is the joint limit enabled?
	bool IsLimitEnabled() const;

	/// Enable/disable the joint limit.
	void EnableLimit(bool flag);

	/// Get the lower joint limit, usually in meters.
	b2decimal GetLowerLimit() const;

	/// Get the upper joint limit, usually in meters.
	b2decimal GetUpperLimit() const;

	/// Set the joint limits, usually in meters.
	void SetLimits(b2decimal lower, b2decimal upper);

	/// Is the joint motor enabled?
	bool IsMotorEnabled() const;

	/// Enable/disable the joint motor.
	void EnableMotor(bool flag);

	/// Set the motor speed, usually in meters per second.
	void SetMotorSpeed(b2decimal speed);

	/// Get the motor speed, usually in meters per second.
	b2decimal GetMotorSpeed() const;

	/// Set the maximum motor force, usually in N.
	void SetMaxMotorForce(b2decimal force);

	/// Get the current motor force, usually in N.
	b2decimal GetMotorForce() const;

protected:
	friend class b2Joint;
	friend class b2GearJoint;
	b2PrismaticJoint(const b2PrismaticJointDef* def);

	void InitVelocityConstraints(const b2TimeStep& step);
	void SolveVelocityConstraints(const b2TimeStep& step);
	bool SolvePositionConstraints(b2decimal baumgarte);

	b2Vec2 m_localAnchor1;
	b2Vec2 m_localAnchor2;
	b2Vec2 m_localXAxis1;
	b2Vec2 m_localYAxis1;
	b2decimal m_refAngle;

	b2Vec2 m_axis, m_perp;
	b2decimal m_s1, m_s2;
	b2decimal m_a1, m_a2;

	b2Mat33 m_K;
	b2Vec3 m_impulse;

	b2decimal m_motorMass;			// effective mass for motor/limit translational constraint.
	b2decimal m_motorImpulse;

	b2decimal m_lowerTranslation;
	b2decimal m_upperTranslation;
	b2decimal m_maxMotorForce;
	b2decimal m_motorSpeed;
	
	bool m_enableLimit;
	bool m_enableMotor;
	b2LimitState m_limitState;
};

inline b2decimal b2PrismaticJoint::GetMotorSpeed() const
{
	return m_motorSpeed;
}

#endif
