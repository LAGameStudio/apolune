/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "GLWindow.h"
#include "TextureLibrary.h"
#include "Box2d.h"
#include "Art.h"

#if defined(NEVER)

class Pyramid : public TouchableBox2dWorld {
public:
	enum	{	e_count = 20 };
 static TouchableBox2dWorld* Create() {	return new Pyramid;	}

	void Inflation()	{
  // Place Ground
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsEdge(b2Vec2(-80.0, 0.0), b2Vec2(80.0, 0.0));
			ground->CreateFixture(&shape, 0.0);
		}

  // Create the pyramid
		{
			double a = 0.5f;
			b2PolygonShape shape;
			shape.SetAsBox(a, a);

			b2Vec2 x(-7.0, 0.75f);
			b2Vec2 y;
			b2Vec2 deltaX(0.5625f, 1.25f);
			b2Vec2 deltaY(1.125f, 1.4f);

			for (int i = 0; i < e_count; ++i)
			{
				y = x;

				for (int j = i; j < e_count; ++j)
				{
					b2BodyDef bd;
					bd.type = b2_dynamicBody;
					bd.position = y;
					b2Body* body = m_world->CreateBody(&bd);
					body->CreateFixture(&shape, 25.0);

					y += deltaY;
				}

				x += deltaX;
			}
		}
	}
};

class ApplyForce : public TouchableBox2dWorld
{
public:
 b2Body *m_Starship;
 
	void Inflation()	{
		m_world->SetGravity(b2Vec2(0.0, 0.0));

		const double k_restitution = 0.4f;

		b2Body* ground;
		{
			b2BodyDef bd;
			bd.position.Set(0.0, 20.0);
			ground = m_world->CreateBody(&bd);

			b2PolygonShape shape;

			b2FixtureDef sd;
			sd.shape = &shape;
			sd.density = 0.0;
			sd.restitution = k_restitution;

			// Left vertical
			shape.SetAsEdge(b2Vec2(-20.0, -20.0), b2Vec2(-20.0, 20.0));
			ground->CreateFixture(&sd);

			// Right vertical
			shape.SetAsEdge(b2Vec2(20.0, -20.0), b2Vec2(20.0, 20.0));
			ground->CreateFixture(&sd);

			// Top horizontal
			shape.SetAsEdge(b2Vec2(-20.0, 20.0), b2Vec2(20.0, 20.0));
			ground->CreateFixture(&sd);

			// Bottom horizontal
			shape.SetAsEdge(b2Vec2(-20.0, -20.0), b2Vec2(20.0, -20.0));
			ground->CreateFixture(&sd);
		}

		{
			b2Transform xf1;
			xf1.R.Set(0.3524f * b2_pi);
			xf1.position = b2Mul(xf1.R, b2Vec2(1.0, 0.0));

			b2Vec2 vertices[3];
			vertices[0] = b2Mul(xf1, b2Vec2(-1.0, 0.0));
			vertices[1] = b2Mul(xf1, b2Vec2(1.0, 0.0));
			vertices[2] = b2Mul(xf1, b2Vec2(0.0, 0.5f));
			
			b2PolygonShape poly1;
			poly1.Set(vertices, 3);

			b2FixtureDef sd1;
			sd1.shape = &poly1;
			sd1.density = 4.0;

			b2Transform xf2;
			xf2.R.Set(-0.3524f * b2_pi);
			xf2.position = b2Mul(xf2.R, b2Vec2(-1.0, 0.0));

			vertices[0] = b2Mul(xf2, b2Vec2(-1.0, 0.0));
			vertices[1] = b2Mul(xf2, b2Vec2(1.0, 0.0));
			vertices[2] = b2Mul(xf2, b2Vec2(0.0, 0.5f));
			
			b2PolygonShape poly2;
			poly2.Set(vertices, 3);

			b2FixtureDef sd2;
			sd2.shape = &poly2;
			sd2.density = 2.0;

			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.angularDamping = 5.0;
			bd.linearDamping = 0.1f;

			bd.position.Set(0.0, 2.0);
			bd.angle = b2_pi;
			bd.allowSleep = false;
			m_body=m_Starship= m_world->CreateBody(&bd);
			m_Starship->CreateFixture(&sd1);
			m_Starship->CreateFixture(&sd2);
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.5f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0;
			fd.friction = 0.3f;

			for (int i = 0; i < 10; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;

				bd.position.Set(0.0, 5.0 + 1.54f * i);
				b2Body* body = m_world->CreateBody(&bd);

				body->CreateFixture(&fd);

				double gravity = 10.0;
				double I = body->GetInertia();
				double mass = body->GetMass();

				// For a circle: I = 0.5 * m * r * r ==> r = sqrt(2 * I / m)
				double radius = b2Sqrt(2.0 * I / mass);

				b2FrictionJointDef jd;
				jd.localAnchorA.SetZero();
				jd.localAnchorB.SetZero();
				jd.bodyA = ground;
				jd.bodyB = body;
				jd.collideConnected = true;
				jd.maxForce = mass * gravity;
				jd.maxTorque = mass * radius * gravity;

				m_world->CreateJoint(&jd);
			}
		}
	}

	void KeyDown(KeyList *keys) {
  if ( keys->InList(DX_W) ) {
			b2Vec2 f = m_body->GetWorldVector(b2Vec2(0.0, -200.0));
			b2Vec2 p = m_body->GetWorldPoint(b2Vec2(0.0, 3.0));
 		m_body->ApplyForce(f, p);
  }

  if ( keys->InList(DX_S) ) {
			b2Vec2 f = m_body->GetWorldVector(b2Vec2(0.0, 200.0));
			b2Vec2 p = m_body->GetWorldPoint(b2Vec2(0.0, 3.0));
 		m_body->ApplyForce(f, p);
  }

  if ( keys->InList(DX_D) ) {
 		m_body->ApplyTorque(50.0);
  }

 	if ( keys->InList(DX_A) ) {
				m_body->ApplyTorque(-50.0);
		}
	}

	static TouchableBox2dWorld* Create()
	{
		return new ApplyForce;
	}

	b2Body* m_body;
};


class Chain : public TouchableBox2dWorld
{
public:
 
	b2Body* head;
 b2Body* tail;
 int links;

	void Inflation()
	{
  links=20;
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsEdge(b2Vec2(-40.0, 0.0), b2Vec2(40.0, 0.0));
			ground->CreateFixture(&shape, 0.0);
		}

		{
			b2PolygonShape shape;
			shape.SetAsBox(0.6f, 0.125f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 40.0;
			fd.friction = 0.5f;

			b2RevoluteJointDef jd;
   jd.collideConnected=false;
   jd.enableLimit=true;
   jd.lowerAngle=-90.0;
   jd.upperAngle=90.0;

			const double y = 65.0;
			b2Body* prevBody = ground;
			for (int i = 0; i < links; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.5f + i, y);
				b2Body* body = m_world->CreateBody(&bd);
    
    if ( i == links-2 ) {
  			jd.collideConnected = true;
     fd.density=60.0;
    } else
    if ( i == links-1 ) {
  			jd.collideConnected = true;
     jd.enableLimit=false;
     fd.density= 40.0;
     tail=body;
    }

				body->CreateFixture(&fd);

				b2Vec2 anchor((double)(i), y);
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);

				prevBody = body;
    if ( i == 0 ) head=body;
			}
   /*
   b2MassData tail_mass;
   tail->GetMassData(&tail_mass);
   tail_mass.mass=50.0; //kg
   tail->SetMassData(&tail_mass);
   */
		}
	}

 
	void KeyDown(KeyList *keys)
	{
  if ( keys->InList(DX_W) ) {
			b2Vec2 f = tail->GetWorldVector(b2Vec2(0.0, -200.0));
			b2Vec2 p = tail->GetWorldPoint(b2Vec2(0.0, 3.0));
 		tail->ApplyForce(f, p);
  }

  if ( keys->InList(DX_S) ) {
			b2Vec2 f = tail->GetWorldVector(b2Vec2(0.0, 200.0));
			b2Vec2 p = tail->GetWorldPoint(b2Vec2(0.0, 3.0));
 		tail->ApplyForce(f, p);
  }

  if ( keys->InList(DX_D) ) {
 		tail->ApplyTorque(50.0);
  }

 	if ( keys->InList(DX_A) ) {
				tail->ApplyTorque(-50.0);
		}
	}

	static TouchableBox2dWorld* Create()
	{
		return new Chain;
	}
};


class ChainTest : public PhysicsToy {
 
 Chain p;
 Zpointer<GLImage> arrow;
 Zdouble hlimit,wlimit;

 void KeyDown( KeyList *keys ) {
  p.KeyDown(keys);
 }

 void MouseMove() {
		p.view.ConvertScreenToWorld(input->mxi-this->x,input->myi-this->y);
  p.MouseMove();
 }

 void LeftDown() {
  int mx=input->mxi-this->x,
      my=input->myi-this->y;
  if ( !p.m_mouseJoint && tail_position.within(mx,my) ) {
   b2Body* body = p.tail;
 		b2MouseJointDef md;
 		md.bodyA = p.m_groundBody;
 		md.bodyB = body;
 		md.target = p.m_mouseWorld;
 		md.maxForce = 1000.0 * body->GetMass();
 		p.m_mouseJoint = (b2MouseJoint*)p.m_world->CreateJoint(&md);
 		body->SetAwake(true);
  } else {
 		p.view.ConvertScreenToWorld(input->mxi-this->x,input->myi-this->y);
   p.MouseDown();
  }
 }

 void LeftUp() {
		p.view.ConvertScreenToWorld(input->mxi-this->x,input->myi-this->y);
  p.MouseUp();
 }

 void OnLoad() {
  Anchor(0.0,0.0,0.0,0.0);
  background.Float(0.0,0.0,0.1f,1.0);
  p.Attach(this);
  p.hz=FPS;
  arrow=library.find( "data/images/spiked.png" );
  box=library.find( "data/images/flares/ball4.png" );
  p.enableContinuous=false;
  hlimit=(double)h*0.5f;
  wlimit=p.view.camera.x-w*0.05f;
  simulated=&p;
  p.BigBang();
 }

 void Between() {
  CameraArrows();
  p.Step();
//  if ( camera.fy > hlimit ) camera.fy--;
//  if ( camera.fx > wlimit ) camera.fx--;
 }

 Cartesian tail_position;
 virtual void Each( b2Body *body, b2Vec2 position, double x, double y ) {
  tint.Any();

  if ( body == p.head ) {
   RotatedRectf(this,arrow,crayons.Pick(alabaster),additive,
    x+5.0,
    y,
    10.0,5.0, 
    rad2deg(body->GetAngle())-180.0
   );
  } else {   
   RotatedRectf(this,box,crayons.Pick(alabaster),additive,
    x,
    y,
    10.0,5.0, 
    -double_range(0.0,rad2deg(body->GetAngle()))
   );
   RotatedRectf(this,box,tint,additive,
    x,
    y,
    10.0,5.0, 
    rad2deg(body->GetAngle())
   );
   RotatedRectf(this,box,tint,additive,
    x,
    y,
    10.0,5.0, 
    double_range(0.0,rad2deg(body->GetAngle()))
   );
  }
 }

 void PostProcessing() {
  Debug();
  b2Vec2 position=p.tail->GetPosition();
  PivotedRotatedRectf(this,arrow,crayons.Pick(alabaster),transparency,
   p.view.camera.x+(position.x*10.0),
   p.view.camera.y-(position.y*10.0-20.0),
   0.0, 0.0,
   20.0,20.0, 
   rad2deg(p.tail->GetAngle())-180.0
  );
  tail_position.SetRect((int)(p.view.camera.x+(position.x*10.0-20.0)),
                        (int)(p.view.camera.y-(position.y*10.0)),
                        40,40);
  Rectangle(this,crayons.Pick(yellow),&tail_position);
  //p.view.PrintDebugInfo(this);
 }
};

class ApplyForceTest : public PhysicsToy {
 ApplyForce p;
 Zpointer<GLImage> arrow;
 Zdouble hlimit,wlimit;

 void KeyDown( KeyList *keys ) {
  p.KeyDown(keys);
 }

 void MouseMove() {
		p.view.ConvertScreenToWorld(input->mxi-this->x,input->myi-this->y);
  p.MouseMove();
 }

 void LeftDown() {
  int mx=input->mxi-this->x,
      my=input->myi-this->y;
  if ( !p.m_mouseJoint && body_position.within(mx,my) ) {
   b2Body* body = p.m_body;
 		b2MouseJointDef md;
 		md.bodyA = p.m_groundBody;
 		md.bodyB = body;
 		md.target = p.m_mouseWorld;
 		md.maxForce = 1000.0 * body->GetMass();
 		p.m_mouseJoint = (b2MouseJoint*)p.m_world->CreateJoint(&md);
 		body->SetAwake(true);
  } else {
 		p.view.ConvertScreenToWorld(input->mxi-this->x,input->myi-this->y);
   p.MouseDown();
  }
 }

 void LeftUp() {
		p.view.ConvertScreenToWorld(input->mxi-this->x,input->myi-this->y);
  p.MouseUp();
 }

 void OnLoad() {
  Anchor(0.0,0.0,0.0,0.0);
  background.Double(0.0, 0.0, 0.1, 1.0);
  p.Attach(this);
  p.hz=FPS;
  arrow=library.find( "data/images/spiked.png" );
  box=library.find( "data/images/flares/ball4.png" );
  p.enableContinuous=false;
  hlimit=h*0.5f;
  wlimit=p.view.camera.x-w*0.05f;
  simulated=&p;
 }

 void Between() {
  CameraArrows();
  p.Step();
//  if ( camera.fy > hlimit ) camera.fy--;
//  if ( camera.fx > wlimit ) camera.fx--;
 }

 Cartesian body_position;
 void Each( b2Body *body, b2Vec2 position, double x, double y ) {
  tint.Any();

  if ( body != p.m_body ) {   
   RotatedRectf(this,box,crayons.Pick(alabaster),additive,
    x,
    y,
    10.0,5.0, 
    -double_range(0.0,rad2deg(body->GetAngle()))
   );
   RotatedRectf(this,box,tint,additive,
    x,
    y,
    10.0,5.0, 
    rad2deg(body->GetAngle())
   );
   RotatedRectf(this,box,tint,additive,
    x,
    y,
    10.0,5.0, 
    double_range(0.0,rad2deg(body->GetAngle()))
   );
  }
 }

 void PostProcessing() {
  Debug();
  b2Vec2 position=p.m_body->GetPosition();
  art.Pivot(arrow,crayons.Pick(alabaster),transparency,
   p.view.camera.x+(position.x*10.0),
   p.view.camera.y-(position.y*10.0-20.0),
   0.0, 0.0,
   20.0,20.0, 
   rad2deg(p.m_body->GetAngle())-180.0
  );
  body_position.SetRect( (int) (p.view.camera.x+(position.x*10.0-20.0)),
                         (int) (p.view.camera.y-(position.y*10.0)),
                        40,40);
  Rectangle(this,crayons.Pick(yellow),&body_position);
  //p.view.PrintDebugInfo(this);
 }
};


class Box2DTest : public PhysicsToy {
 Pyramid p;
 Zdouble hlimit,wlimit;

 void KeyDown( KeyList *keys ) {
 }

 void MouseMove() {
		p.view.ConvertScreenToWorld(input->mxi-this->x,input->myi-this->y);
  p.MouseMove();
 }

 void LeftDown() {
  int mx=input->mxi-this->x,
      my=input->myi-this->y;
		p.view.ConvertScreenToWorld(input->mxi-this->x,input->myi-this->y);
  p.MouseDown();
 }

 void LeftUp() {
		p.view.ConvertScreenToWorld(input->mxi-this->x,input->myi-this->y);
  p.MouseUp();
 }

 void Init() {
  p.gravity.SetZero();
 }

 void OnLoad() {
  Anchor(0.0,0.0,0.0,0.0);
  background.Float(0.0,0.0,0.1f,1.0);
  p.Attach(this);
  p.hz=FPS;
  box=library.find( "data/images/elements/rounded_1024.png" );
  p.enableContinuous=false;
  hlimit=(double)h*0.5f;
  wlimit=p.view.camera.x-w*0.05f;
  simulated=&p;
  p.BigBang();
 }

 void Between() {
  CameraArrows();
  p.Step();
//  if ( camera.fy > hlimit ) camera.fy--;
//  if ( camera.fx > wlimit ) camera.fx--;
 }

 void Each( b2Body *body, b2Vec2 position, double x, double y ) {
  tint.Any();

  art.Pivot(box,crayons.Pick(alabaster),transparency,
   x,
   y,
   0.0, 0.0,
   5.0,5.0, 
   rad2deg(body->GetAngle())-180.0
  );
 }

 void PostProcessing() {
  Debug();
  //p.view.PrintDebugInfo(this);
 }
};
#endif