/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
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

#include "b2ContactSolver.h"
#include "b2Contact.h"
#include "b2Body.h"
#include "b2Fixture.h"
#include "b2World.h"
#include "b2StackAllocator.h"

#define B2_DEBUG_SOLVER 0

b2ContactSolver::b2ContactSolver(b2Contact** contacts, int contactCount,
								b2StackAllocator* allocator, b2decimal impulseRatio)
{
	m_allocator = allocator;

	m_constraintCount = contactCount;
	m_constraints = (b2ContactConstraint*)m_allocator->Allocate(m_constraintCount * sizeof(b2ContactConstraint));

	for (int i = 0; i < m_constraintCount; ++i)
	{
		b2Contact* contact = contacts[i];

		b2Fixture* fixtureA = contact->m_fixtureA;
		b2Fixture* fixtureB = contact->m_fixtureB;
		b2Shape* shapeA = fixtureA->GetShape();
		b2Shape* shapeB = fixtureB->GetShape();
		b2decimal radiusA = shapeA->m_radius;
		b2decimal radiusB = shapeB->m_radius;
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();
		b2Manifold* manifold = contact->GetManifold();

		b2decimal friction = b2MixFriction(fixtureA->GetFriction(), fixtureB->GetFriction());
		b2decimal restitution = b2MixRestitution(fixtureA->GetRestitution(), fixtureB->GetRestitution());

		b2Vec2 vA = bodyA->m_linearVelocity;
		b2Vec2 vB = bodyB->m_linearVelocity;
		b2decimal wA = bodyA->m_angularVelocity;
		b2decimal wB = bodyB->m_angularVelocity;

		b2Assert(manifold->pointCount > 0);

		b2WorldManifold worldManifold;
		worldManifold.Initialize(manifold, bodyA->m_xf, radiusA, bodyB->m_xf, radiusB);

		b2ContactConstraint* cc = m_constraints + i;
		cc->bodyA = bodyA;
		cc->bodyB = bodyB;
		cc->manifold = manifold;
		cc->normal = worldManifold.normal;
		cc->pointCount = manifold->pointCount;
		cc->friction = friction;

		cc->localNormal = manifold->localNormal;
		cc->localPoint = manifold->localPoint;
		cc->radius = radiusA + radiusB;
		cc->type = manifold->type;

		for (int j = 0; j < cc->pointCount; ++j)
		{
			b2ManifoldPoint* cp = manifold->points + j;
			b2ContactConstraintPoint* ccp = cc->points + j;

			ccp->normalImpulse = impulseRatio * cp->normalImpulse;
			ccp->tangentImpulse = impulseRatio * cp->tangentImpulse;

			ccp->localPoint = cp->localPoint;

			ccp->rA = worldManifold.points[j] - bodyA->m_sweep.c;
			ccp->rB = worldManifold.points[j] - bodyB->m_sweep.c;

			b2decimal rnA = b2Cross(ccp->rA, cc->normal);
			b2decimal rnB = b2Cross(ccp->rB, cc->normal);
			rnA *= rnA;
			rnB *= rnB;

			b2decimal kNormal = bodyA->m_invMass + bodyB->m_invMass + bodyA->m_invI * rnA + bodyB->m_invI * rnB;

			b2Assert(kNormal > b2_epsilon);
			ccp->normalMass = 1.0 / kNormal;

			b2Vec2 tangent = b2Cross(cc->normal, 1.0);

			b2decimal rtA = b2Cross(ccp->rA, tangent);
			b2decimal rtB = b2Cross(ccp->rB, tangent);
			rtA *= rtA;
			rtB *= rtB;

			b2decimal kTangent = bodyA->m_invMass + bodyB->m_invMass + bodyA->m_invI * rtA + bodyB->m_invI * rtB;

			b2Assert(kTangent > b2_epsilon);
			ccp->tangentMass = 1.0 /  kTangent;

			// Setup a velocity bias for restitution.
			ccp->velocityBias = 0.0;
			b2decimal vRel = b2Dot(cc->normal, vB + b2Cross(wB, ccp->rB) - vA - b2Cross(wA, ccp->rA));
			if (vRel < -b2_velocityThreshold)
			{
				ccp->velocityBias = -restitution * vRel;
			}
		}

		// If we have two points, then prepare the block solver.
		if (cc->pointCount == 2)
		{
			b2ContactConstraintPoint* ccp1 = cc->points + 0;
			b2ContactConstraintPoint* ccp2 = cc->points + 1;
			
			b2decimal invMassA = bodyA->m_invMass;
			b2decimal invIA = bodyA->m_invI;
			b2decimal invMassB = bodyB->m_invMass;
			b2decimal invIB = bodyB->m_invI;

			b2decimal rn1A = b2Cross(ccp1->rA, cc->normal);
			b2decimal rn1B = b2Cross(ccp1->rB, cc->normal);
			b2decimal rn2A = b2Cross(ccp2->rA, cc->normal);
			b2decimal rn2B = b2Cross(ccp2->rB, cc->normal);

			b2decimal k11 = invMassA + invMassB + invIA * rn1A * rn1A + invIB * rn1B * rn1B;
			b2decimal k22 = invMassA + invMassB + invIA * rn2A * rn2A + invIB * rn2B * rn2B;
			b2decimal k12 = invMassA + invMassB + invIA * rn1A * rn2A + invIB * rn1B * rn2B;

			// Ensure a reasonable condition number.
			const b2decimal k_maxConditionNumber = 100.0;
			if (k11 * k11 < k_maxConditionNumber * (k11 * k22 - k12 * k12))
			{
				// K is safe to invert.
				cc->K.col1.Set(k11, k12);
				cc->K.col2.Set(k12, k22);
				cc->normalMass = cc->K.GetInverse();
			}
			else
			{
				// The constraints are redundant, just use one.
				// TODO_ERIN use deepest?
				cc->pointCount = 1;
			}
		}
	}
}

b2ContactSolver::~b2ContactSolver()
{
	m_allocator->Free(m_constraints);
}

void b2ContactSolver::WarmStart()
{
	// Warm start.
	for (int i = 0; i < m_constraintCount; ++i)
	{
		b2ContactConstraint* c = m_constraints + i;

		b2Body* bodyA = c->bodyA;
		b2Body* bodyB = c->bodyB;
		b2decimal invMassA = bodyA->m_invMass;
		b2decimal invIA = bodyA->m_invI;
		b2decimal invMassB = bodyB->m_invMass;
		b2decimal invIB = bodyB->m_invI;
		b2Vec2 normal = c->normal;
		b2Vec2 tangent = b2Cross(normal, 1.0);

		for (int j = 0; j < c->pointCount; ++j)
		{
			b2ContactConstraintPoint* ccp = c->points + j;
			b2Vec2 P = ccp->normalImpulse * normal + ccp->tangentImpulse * tangent;
			bodyA->m_angularVelocity -= invIA * b2Cross(ccp->rA, P);
			bodyA->m_linearVelocity -= invMassA * P;
			bodyB->m_angularVelocity += invIB * b2Cross(ccp->rB, P);
			bodyB->m_linearVelocity += invMassB * P;
		}
	}
}

void b2ContactSolver::SolveVelocityConstraints()
{
	for (int i = 0; i < m_constraintCount; ++i)
	{
		b2ContactConstraint* c = m_constraints + i;
		b2Body* bodyA = c->bodyA;
		b2Body* bodyB = c->bodyB;
		b2decimal wA = bodyA->m_angularVelocity;
		b2decimal wB = bodyB->m_angularVelocity;
		b2Vec2 vA = bodyA->m_linearVelocity;
		b2Vec2 vB = bodyB->m_linearVelocity;
		b2decimal invMassA = bodyA->m_invMass;
		b2decimal invIA = bodyA->m_invI;
		b2decimal invMassB = bodyB->m_invMass;
		b2decimal invIB = bodyB->m_invI;
		b2Vec2 normal = c->normal;
		b2Vec2 tangent = b2Cross(normal, 1.0);
		b2decimal friction = c->friction;

		b2Assert(c->pointCount == 1 || c->pointCount == 2);

		// Solve tangent constraints
		for (int j = 0; j < c->pointCount; ++j)
		{
			b2ContactConstraintPoint* ccp = c->points + j;

			// Relative velocity at contact
			b2Vec2 dv = vB + b2Cross(wB, ccp->rB) - vA - b2Cross(wA, ccp->rA);

			// Compute tangent force
			b2decimal vt = b2Dot(dv, tangent);
			b2decimal lambda = ccp->tangentMass * (-vt);

			// b2Clamp the accumulated force
			b2decimal maxFriction = friction * ccp->normalImpulse;
			b2decimal newImpulse = b2Clamp(ccp->tangentImpulse + lambda, -maxFriction, maxFriction);
			lambda = newImpulse - ccp->tangentImpulse;

			// Apply contact impulse
			b2Vec2 P = lambda * tangent;

			vA -= invMassA * P;
			wA -= invIA * b2Cross(ccp->rA, P);

			vB += invMassB * P;
			wB += invIB * b2Cross(ccp->rB, P);

			ccp->tangentImpulse = newImpulse;
		}

		// Solve normal constraints
		if (c->pointCount == 1)
		{
			b2ContactConstraintPoint* ccp = c->points + 0;

			// Relative velocity at contact
			b2Vec2 dv = vB + b2Cross(wB, ccp->rB) - vA - b2Cross(wA, ccp->rA);

			// Compute normal impulse
			b2decimal vn = b2Dot(dv, normal);
			b2decimal lambda = -ccp->normalMass * (vn - ccp->velocityBias);

			// b2Clamp the accumulated impulse
			b2decimal newImpulse = b2Max(ccp->normalImpulse + lambda, 0.0);
			lambda = newImpulse - ccp->normalImpulse;

			// Apply contact impulse
			b2Vec2 P = lambda * normal;
			vA -= invMassA * P;
			wA -= invIA * b2Cross(ccp->rA, P);

			vB += invMassB * P;
			wB += invIB * b2Cross(ccp->rB, P);
			ccp->normalImpulse = newImpulse;
		}
		else
		{
			// Block solver developed in collaboration with Dirk Gregorius (back in 01/07 on Box2D_Lite).
			// Build the mini LCP for this contact patch
			//
			// vn = A * x + b, vn >= 0, , vn >= 0, x >= 0 and vn_i * x_i = 0 with i = 1..2
			//
			// A = J * W * JT and J = ( -n, -r1 x n, n, r2 x n )
			// b = vn_0 - velocityBias
			//
			// The system is solved using the "Total enumeration method" (s. Murty). The complementary constraint vn_i * x_i
			// implies that we must have in any solution either vn_i = 0 or x_i = 0. So for the 2D contact problem the cases
			// vn1 = 0 and vn2 = 0, x1 = 0 and x2 = 0, x1 = 0 and vn2 = 0, x2 = 0 and vn1 = 0 need to be tested. The first valid
			// solution that satisfies the problem is chosen.
			// 
			// In order to account of the accumulated impulse 'a' (because of the iterative nature of the solver which only requires
			// that the accumulated impulse is clamped and not the incremental impulse) we change the impulse variable (x_i).
			//
			// Substitute:
			// 
			// x = x' - a
			// 
			// Plug into above equation:
			//
			// vn = A * x + b
			//    = A * (x' - a) + b
			//    = A * x' + b - A * a
			//    = A * x' + b'
			// b' = b - A * a;

			b2ContactConstraintPoint* cp1 = c->points + 0;
			b2ContactConstraintPoint* cp2 = c->points + 1;

			b2Vec2 a(cp1->normalImpulse, cp2->normalImpulse);
			b2Assert(a.x >= 0.0 && a.y >= 0.0);

			// Relative velocity at contact
			b2Vec2 dv1 = vB + b2Cross(wB, cp1->rB) - vA - b2Cross(wA, cp1->rA);
			b2Vec2 dv2 = vB + b2Cross(wB, cp2->rB) - vA - b2Cross(wA, cp2->rA);

			// Compute normal velocity
			b2decimal vn1 = b2Dot(dv1, normal);
			b2decimal vn2 = b2Dot(dv2, normal);

			b2Vec2 b;
			b.x = vn1 - cp1->velocityBias;
			b.y = vn2 - cp2->velocityBias;
			b -= b2Mul(c->K, a);

			const b2decimal k_errorTol = 1e-3f;
			B2_NOT_USED(k_errorTol);

			for (;;)
			{
				//
				// Case 1: vn = 0
				//
				// 0 = A * x' + b'
				//
				// Solve for x':
				//
				// x' = - inv(A) * b'
				//
				b2Vec2 x = - b2Mul(c->normalMass, b);

				if (x.x >= 0.0 && x.y >= 0.0)
				{
					// Resubstitute for the incremental impulse
					b2Vec2 d = x - a;

					// Apply incremental impulse
					b2Vec2 P1 = d.x * normal;
					b2Vec2 P2 = d.y * normal;
					vA -= invMassA * (P1 + P2);
					wA -= invIA * (b2Cross(cp1->rA, P1) + b2Cross(cp2->rA, P2));

					vB += invMassB * (P1 + P2);
					wB += invIB * (b2Cross(cp1->rB, P1) + b2Cross(cp2->rB, P2));

					// Accumulate
					cp1->normalImpulse = x.x;
					cp2->normalImpulse = x.y;

#if B2_DEBUG_SOLVER == 1
					// Postconditions
					dv1 = vB + b2Cross(wB, cp1->rB) - vA - b2Cross(wA, cp1->rA);
					dv2 = vB + b2Cross(wB, cp2->rB) - vA - b2Cross(wA, cp2->rA);

					// Compute normal velocity
					vn1 = b2Dot(dv1, normal);
					vn2 = b2Dot(dv2, normal);

					b2Assert(b2Abs(vn1 - cp1->velocityBias) < k_errorTol);
					b2Assert(b2Abs(vn2 - cp2->velocityBias) < k_errorTol);
#endif
					break;
				}

				//
				// Case 2: vn1 = 0 and x2 = 0
				//
				//   0 = a11 * x1' + a12 * 0 + b1' 
				// vn2 = a21 * x1' + a22 * 0 + b2'
				//
				x.x = - cp1->normalMass * b.x;
				x.y = 0.0;
				vn1 = 0.0;
				vn2 = c->K.col1.y * x.x + b.y;

				if (x.x >= 0.0 && vn2 >= 0.0)
				{
					// Resubstitute for the incremental impulse
					b2Vec2 d = x - a;

					// Apply incremental impulse
					b2Vec2 P1 = d.x * normal;
					b2Vec2 P2 = d.y * normal;
					vA -= invMassA * (P1 + P2);
					wA -= invIA * (b2Cross(cp1->rA, P1) + b2Cross(cp2->rA, P2));

					vB += invMassB * (P1 + P2);
					wB += invIB * (b2Cross(cp1->rB, P1) + b2Cross(cp2->rB, P2));

					// Accumulate
					cp1->normalImpulse = x.x;
					cp2->normalImpulse = x.y;

#if B2_DEBUG_SOLVER == 1
					// Postconditions
					dv1 = vB + b2Cross(wB, cp1->rB) - vA - b2Cross(wA, cp1->rA);

					// Compute normal velocity
					vn1 = b2Dot(dv1, normal);

					b2Assert(b2Abs(vn1 - cp1->velocityBias) < k_errorTol);
#endif
					break;
				}


				//
				// Case 3: vn2 = 0 and x1 = 0
				//
				// vn1 = a11 * 0 + a12 * x2' + b1' 
				//   0 = a21 * 0 + a22 * x2' + b2'
				//
				x.x = 0.0;
				x.y = - cp2->normalMass * b.y;
				vn1 = c->K.col2.x * x.y + b.x;
				vn2 = 0.0;

				if (x.y >= 0.0 && vn1 >= 0.0)
				{
					// Resubstitute for the incremental impulse
					b2Vec2 d = x - a;

					// Apply incremental impulse
					b2Vec2 P1 = d.x * normal;
					b2Vec2 P2 = d.y * normal;
					vA -= invMassA * (P1 + P2);
					wA -= invIA * (b2Cross(cp1->rA, P1) + b2Cross(cp2->rA, P2));

					vB += invMassB * (P1 + P2);
					wB += invIB * (b2Cross(cp1->rB, P1) + b2Cross(cp2->rB, P2));

					// Accumulate
					cp1->normalImpulse = x.x;
					cp2->normalImpulse = x.y;

#if B2_DEBUG_SOLVER == 1
					// Postconditions
					dv2 = vB + b2Cross(wB, cp2->rB) - vA - b2Cross(wA, cp2->rA);

					// Compute normal velocity
					vn2 = b2Dot(dv2, normal);

					b2Assert(b2Abs(vn2 - cp2->velocityBias) < k_errorTol);
#endif
					break;
				}

				//
				// Case 4: x1 = 0 and x2 = 0
				// 
				// vn1 = b1
				// vn2 = b2;
				x.x = 0.0;
				x.y = 0.0;
				vn1 = b.x;
				vn2 = b.y;

				if (vn1 >= 0.0 && vn2 >= 0.0 )
				{
					// Resubstitute for the incremental impulse
					b2Vec2 d = x - a;

					// Apply incremental impulse
					b2Vec2 P1 = d.x * normal;
					b2Vec2 P2 = d.y * normal;
					vA -= invMassA * (P1 + P2);
					wA -= invIA * (b2Cross(cp1->rA, P1) + b2Cross(cp2->rA, P2));

					vB += invMassB * (P1 + P2);
					wB += invIB * (b2Cross(cp1->rB, P1) + b2Cross(cp2->rB, P2));

					// Accumulate
					cp1->normalImpulse = x.x;
					cp2->normalImpulse = x.y;

					break;
				}

				// No solution, give up. This is hit sometimes, but it doesn't seem to matter.
				break;
			}
		}

		bodyA->m_linearVelocity = vA;
		bodyA->m_angularVelocity = wA;
		bodyB->m_linearVelocity = vB;
		bodyB->m_angularVelocity = wB;
	}
}

void b2ContactSolver::StoreImpulses()
{
	for (int i = 0; i < m_constraintCount; ++i)
	{
		b2ContactConstraint* c = m_constraints + i;
		b2Manifold* m = c->manifold;

		for (int j = 0; j < c->pointCount; ++j)
		{
			m->points[j].normalImpulse = c->points[j].normalImpulse;
			m->points[j].tangentImpulse = c->points[j].tangentImpulse;
		}
	}
}

struct b2PositionSolverManifold
{
	void Initialize(b2ContactConstraint* cc, int index)
	{
		b2Assert(cc->pointCount > 0);

		switch (cc->type)
		{
		case b2Manifold::e_circles:
			{
				b2Vec2 pointA = cc->bodyA->GetWorldPoint(cc->localPoint);
				b2Vec2 pointB = cc->bodyB->GetWorldPoint(cc->points[0].localPoint);
				if (b2DistanceSquared(pointA, pointB) > b2_epsilon * b2_epsilon)
				{
					normal = pointB - pointA;
					normal.Normalize();
				}
				else
				{
					normal.Set(1.0, 0.0);
				}

				point = 0.5f * (pointA + pointB);
				separation = b2Dot(pointB - pointA, normal) - cc->radius;
			}
			break;

		case b2Manifold::e_faceA:
			{
				normal = cc->bodyA->GetWorldVector(cc->localNormal);
				b2Vec2 planePoint = cc->bodyA->GetWorldPoint(cc->localPoint);

				b2Vec2 clipPoint = cc->bodyB->GetWorldPoint(cc->points[index].localPoint);
				separation = b2Dot(clipPoint - planePoint, normal) - cc->radius;
				point = clipPoint;
			}
			break;

		case b2Manifold::e_faceB:
			{
				normal = cc->bodyB->GetWorldVector(cc->localNormal);
				b2Vec2 planePoint = cc->bodyB->GetWorldPoint(cc->localPoint);

				b2Vec2 clipPoint = cc->bodyA->GetWorldPoint(cc->points[index].localPoint);
				separation = b2Dot(clipPoint - planePoint, normal) - cc->radius;
				point = clipPoint;

				// Ensure normal points from A to B
				normal = -normal;
			}
			break;
		}
	}

	b2Vec2 normal;
	b2Vec2 point;
	b2decimal separation;
};

// Sequential solver.
bool b2ContactSolver::SolvePositionConstraints(b2decimal baumgarte)
{
	b2decimal minSeparation = 0.0;

	for (int i = 0; i < m_constraintCount; ++i)
	{
		b2ContactConstraint* c = m_constraints + i;
		b2Body* bodyA = c->bodyA;
		b2Body* bodyB = c->bodyB;

		b2decimal invMassA = bodyA->m_mass * bodyA->m_invMass;
		b2decimal invIA = bodyA->m_mass * bodyA->m_invI;
		b2decimal invMassB = bodyB->m_mass * bodyB->m_invMass;
		b2decimal invIB = bodyB->m_mass * bodyB->m_invI;

		// Solve normal constraints
		for (int j = 0; j < c->pointCount; ++j)
		{
			b2PositionSolverManifold psm;
			psm.Initialize(c, j);
			b2Vec2 normal = psm.normal;

			b2Vec2 point = psm.point;
			b2decimal separation = psm.separation;

			b2Vec2 rA = point - bodyA->m_sweep.c;
			b2Vec2 rB = point - bodyB->m_sweep.c;

			// Track max constraint error.
			minSeparation = b2Min(minSeparation, separation);

			// Prevent large corrections and allow slop.
			b2decimal C = b2Clamp(baumgarte * (separation + b2_linearSlop), -b2_maxLinearCorrection, 0.0);

			// Compute the effective mass.
			b2decimal rnA = b2Cross(rA, normal);
			b2decimal rnB = b2Cross(rB, normal);
			b2decimal K = invMassA + invMassB + invIA * rnA * rnA + invIB * rnB * rnB;

			// Compute normal impulse
			b2decimal impulse = K > 0.0 ? - C / K : 0.0;

			b2Vec2 P = impulse * normal;

			bodyA->m_sweep.c -= invMassA * P;
			bodyA->m_sweep.a -= invIA * b2Cross(rA, P);
			bodyA->SynchronizeTransform();

			bodyB->m_sweep.c += invMassB * P;
			bodyB->m_sweep.a += invIB * b2Cross(rB, P);
			bodyB->SynchronizeTransform();
		}
	}

	// We can't expect minSpeparation >= -b2_linearSlop because we don't
	// push the separation above -b2_linearSlop.
	return minSeparation >= -1.5f * b2_linearSlop;
}
