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

#ifndef B2_MOUSE_JOINT_H
#define B2_MOUSE_JOINT_H

#include "b2Joint.h"

/// Mouse joint definition. This requires a world target point,
/// tuning parameters, and the time step.
struct b2MouseJointDef : public b2JointDef
{
	b2MouseJointDef()
	{
		type = e_mouseJoint;
		target.Set(0.0, 0.0);
		maxForce = 0.0;
		frequencyHz = 5.0;
		dampingRatio = 0.7f;
	}

	/// The initial world target point. This is assumed
	/// to coincide with the body anchor initially.
	b2Vec2 target;

	/// The maximum constraint force that can be exerted
	/// to move the candidate body. Usually you will express
	/// as some multiple of the weight (multiplier * mass * gravity).
	b2decimal maxForce;

	/// The response speed.
	b2decimal frequencyHz;

	/// The damping ratio. 0 = no damping, 1 = critical damping.
	b2decimal dampingRatio;
};

/// A mouse joint is used to make a point on a body track a
/// specified world point. This a soft constraint with a maximum
/// force. This allows the constraint to stretch and without
/// applying huge forces.
/// NOTE: this joint is not documented in the manual because it was
/// developed to be used in the testbed. If you want to learn how to
/// use the mouse joint, look at the testbed.
class b2MouseJoint : public b2Joint
{
public:

	/// Implements b2Joint.
	b2Vec2 GetAnchorA() const;

	/// Implements b2Joint.
	b2Vec2 GetAnchorB() const;

	/// Implements b2Joint.
	b2Vec2 GetReactionForce(b2decimal inv_dt) const;

	/// Implements b2Joint.
	b2decimal GetReactionTorque(b2decimal inv_dt) const;

	/// Use this to update the target point.
	void SetTarget(const b2Vec2& target);
	const b2Vec2& GetTarget() const;

	/// Set/get the maximum force in Newtons.
	void SetMaxForce(b2decimal force);
	b2decimal GetMaxForce() const;

	/// Set/get the frequency in Hertz.
	void SetFrequency(b2decimal hz);
	b2decimal GetFrequency() const;

	/// Set/get the damping ratio (dimensionless).
	void SetDampingRatio(b2decimal ratio);
	b2decimal GetDampingRatio() const;

protected:
	friend class b2Joint;

	b2MouseJoint(const b2MouseJointDef* def);

	void InitVelocityConstraints(const b2TimeStep& step);
	void SolveVelocityConstraints(const b2TimeStep& step);
	bool SolvePositionConstraints(b2decimal baumgarte) { B2_NOT_USED(baumgarte); return true; }

	b2Vec2 m_localAnchor;
	b2Vec2 m_target;
	b2Vec2 m_impulse;

	b2Mat22 m_mass;		// effective mass for point-to-point constraint.
	b2Vec2 m_C;				// position error
	b2decimal m_maxForce;
	b2decimal m_frequencyHz;
	b2decimal m_dampingRatio;
	b2decimal m_beta;
	b2decimal m_gamma;
};

#endif
