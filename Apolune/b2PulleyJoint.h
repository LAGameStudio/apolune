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

#ifndef B2_PULLEY_JOINT_H
#define B2_PULLEY_JOINT_H

#include "b2Joint.h"

const b2decimal b2_minPulleyLength = 2.0;

/// Pulley joint definition. This requires two ground anchors,
/// two dynamic body anchor points, max lengths for each side,
/// and a pulley ratio.
struct b2PulleyJointDef : public b2JointDef
{
	b2PulleyJointDef()
	{
		type = e_pulleyJoint;
		groundAnchorA.Set(-1.0, 1.0);
		groundAnchorB.Set(1.0, 1.0);
		localAnchorA.Set(-1.0, 0.0);
		localAnchorB.Set(1.0, 0.0);
		lengthA = 0.0;
		maxLengthA = 0.0;
		lengthB = 0.0;
		maxLengthB = 0.0;
		ratio = 1.0;
		collideConnected = true;
	}

	/// Initialize the bodies, anchors, lengths, max lengths, and ratio using the world anchors.
	void Initialize(b2Body* bodyA, b2Body* bodyB,
					const b2Vec2& groundAnchorA, const b2Vec2& groundAnchorB,
					const b2Vec2& anchorA, const b2Vec2& anchorB,
					b2decimal ratio);

	/// The first ground anchor in world coordinates. This point never moves.
	b2Vec2 groundAnchorA;

	/// The second ground anchor in world coordinates. This point never moves.
	b2Vec2 groundAnchorB;

	/// The local anchor point relative to bodyA's origin.
	b2Vec2 localAnchorA;

	/// The local anchor point relative to bodyB's origin.
	b2Vec2 localAnchorB;

	/// The a reference length for the segment attached to bodyA.
	b2decimal lengthA;

	/// The maximum length of the segment attached to bodyA.
	b2decimal maxLengthA;

	/// The a reference length for the segment attached to bodyB.
	b2decimal lengthB;

	/// The maximum length of the segment attached to bodyB.
	b2decimal maxLengthB;

	/// The pulley ratio, used to simulate a block-and-tackle.
	b2decimal ratio;
};

/// The pulley joint is connected to two bodies and two fixed ground points.
/// The pulley supports a ratio such that:
/// length1 + ratio * length2 <= constant
/// Yes, the force transmitted is scaled by the ratio.
/// The pulley also enforces a maximum length limit on both sides. This is
/// useful to prevent one side of the pulley hitting the top.
class b2PulleyJoint : public b2Joint
{
public:
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;

	b2Vec2 GetReactionForce(b2decimal inv_dt) const;
	b2decimal GetReactionTorque(b2decimal inv_dt) const;

	/// Get the first ground anchor.
	b2Vec2 GetGroundAnchorA() const;

	/// Get the second ground anchor.
	b2Vec2 GetGroundAnchorB() const;

	/// Get the current length of the segment attached to body1.
	b2decimal GetLength1() const;

	/// Get the current length of the segment attached to body2.
	b2decimal GetLength2() const;

	/// Get the pulley ratio.
	b2decimal GetRatio() const;

protected:

	friend class b2Joint;
	b2PulleyJoint(const b2PulleyJointDef* data);

	void InitVelocityConstraints(const b2TimeStep& step);
	void SolveVelocityConstraints(const b2TimeStep& step);
	bool SolvePositionConstraints(b2decimal baumgarte);

	b2Vec2 m_groundAnchor1;
	b2Vec2 m_groundAnchor2;
	b2Vec2 m_localAnchor1;
	b2Vec2 m_localAnchor2;

	b2Vec2 m_u1;
	b2Vec2 m_u2;
	
	b2decimal m_constant;
	b2decimal m_ratio;
	
	b2decimal m_maxLength1;
	b2decimal m_maxLength2;

	// Effective masses
	b2decimal m_pulleyMass;
	b2decimal m_limitMass1;
	b2decimal m_limitMass2;

	// Impulses for accumulation/warm starting.
	b2decimal m_impulse;
	b2decimal m_limitImpulse1;
	b2decimal m_limitImpulse2;

	b2LimitState m_state;
	b2LimitState m_limitState1;
	b2LimitState m_limitState2;
};

#endif
