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

#include "LinkedList.h"
#include "Vertex.h"
#include "Proce55or.h"
#include "Presentation.h"

class Level2d;
class fx_Sprite2d;

// A single item contained in the "master list"
class Item2d : public NamedHandle<fx_Sprite2d> {
public:
 bool combines;
 int type;
 Item2d() : NamedHandle() {
  type=0; combines=true;
 }
};

// Considered to be a "master list" of items
class Items2d : public LinkedList {
public:
 void Add( const char *name, fx_Sprite2d *model, int type, float weight ) {
  Item2d *i=new Item2d;
  i->p=model;
  i->name=string(name);
  Append(i);
 }
};

class InventorySlot2d : public Handle<Item2d> {
public:
 int quantity;
};

class Inventory2d : public LinkedList {
public:
 float weight,capacity;
 Inventory2d() : LinkedList() {
  weight=0.0f;
  capacity=1000.0f;
 }
 bool Add(Item2d *i);
 bool Add(Item2d *i, int qty);
 // Adds a new slot of item i, quantity 1
 void AddNewSlot( Item2d *i ) {
  InventorySlot2d *is=new InventorySlot2d;
  is->p=i;
  is->quantity=1;
  Append(is);
 }
 // Returns the total number of a specific item
 int Total( Item2d *i ) {
  if ( i->combines ) {
   InventorySlot2d *holding=findByItem(i);
   if ( holding ) return holding->quantity;
   else return 0;
  }
  int total=0;
  FOREACH(InventorySlot2d,j) if ( j->p==i ) total+=j->quantity;
  return total;
 }
 // Returns the total of all held items and their masses
 inline float TotalWeight( Item2d *i );
 // Returns true if the inventory can hold qty more of item(s) i, false if not
 inline bool CanHold( Item2d *i, int qty );
 // Increases, combining like items with the combines flag, without capacity check
 inline void Increase( Item2d *i, int qty );
 // Decreases, combining like items with the combines flag, limiting to amount available
 inline void Decrease( Item2d *i, int qty );
 // Finds first inventory slot containing item I, or null if not being held
 InventorySlot2d *findByItem( Item2d *I ) {
  FOREACH(InventorySlot2d,j) if ( j->p == I ) return j;
  return null;
 }
 // Finds first inventory slot containing item of name, or null if none found
 InventorySlot2d *findByName( string name ) {
  FOREACH(InventorySlot2d,j) if ( name==j->p->name ) return j;
  return null;
 }
 // Finds all items with a similar arbitrary type number, returns a new temporary Inventory handle list
 Inventory2d *allByType( int type ) {
  Inventory2d *results=new Inventory2d;
  FOREACH(InventorySlot2d,j) if ( j->p->type == type ) results->Add(j->p,j->quantity);
  return results;
 }
 // Finds all items similar to the name provided (uses str_prefix to match), returns a new temporary Inventory handle list
 Inventory2d *allLikeName( const char *name ) {
  Inventory2d *results=new Inventory2d;
  FOREACH(InventorySlot2d,j) {
   if ( !str_prefix( j->p->name.c_str(), name ) ) results->Add(j->p,j->quantity);
  }
  return results;
 }
 CLEARLISTRESET(InventorySlot2d);
 ~Inventory2d() { Clear(); }
};

enum MovementStates {
 _Other, _Resting, _Jumping, _Shooting, _AlternativeFiring, _Crouching, _Standing, _Flying,
 _Falling, _Kicking, _Punching, _GrabbingEnemy, _Rappelling, _Swimming,
 _Drowning, _Walking, _Running, _Dead, _Hanging, _Rushing, _Mantling,
 _Ducking, _Blocking, _Sniping, _UsingAbility, _Opening, _Throwing, _Closing,
 _Dropping, _Digging, _Dying, _BeingBorn, _Birthing,
};

/* Mega 2d-sprite base class
 * So what this thing does: maintains one entity and its animations.   Inherit from this to create a specialized entity.
 * All movement states have two virtual function options: one-off as a command-like verb, then add an -ing for a per-frame call.
 *   one-off: sets up transition, -ing: transitions to cyclic state, par example
 * This way, sprites for the player and sprites for the AI can be built with similar concepts.  Use the "Other" state for
 * custom states, or add functions of your own by overriding.  Much is up to interpretation at this level.  
 * Note: 
 */
class fx_Sprite2d : public Proce55or {
public:
 fx_Sprite2d **self;
 Level2d *level;                           // Pointer to the level we are on
 Vertex position, velocity, acceleration;  // Vectors for physics
 Cartesians hands;                         // Special points for hands and feet (and/or other things)
 Cartesians boundaries;                    // Bounding boxes for collision detection
 Inventory2d inventory;
 MovementStates state;
 PROGRAMMABLE(fx_Sprite2d,movementfuncptr,move,Move);
 fx_Sprite2d *target;                      // Pointer used as reference, usually to the player
 float money,score,health,power,strength,mass;
 float walkingSpeed;
 float directionf;
 int directioni;
 GLImage *frame;
 void Empty(){}
 virtual void Init() {
  money=score=health=power=strength=walkingSpeed=0.0f;
  directionf=0.0f;
  directioni=0;
  move=&fx_Sprite2d::Empty;
  self=null;
 }
 virtual void Setup() { Born(); }
 virtual void Customize() {}
 virtual void Born() {}
 virtual void GetTouchingItems() {}
 virtual void AI() {}
 virtual void Physics() {}
 virtual void BetweenFrames() {
  AI();
  Move();
  Physics();
  GetTouchingItems();
 }
 virtual void RenderMirror() {}
 virtual void Render() {}
 /* Device input controls, can be called this way or bound elsewhere */
 virtual void Keyboard( KeyList *keys ) {}
 virtual void Joystick( Interface *input ) {}
 virtual void Mouse( Interface *input ) {}
 /* Moving and Fighting */
 virtual void Custom() {}
 virtual void Rest() {}
 virtual void Resting() {}
 virtual void Jump() {}
 virtual void Jumping() {}
 virtual void Shoot() {}
 virtual void Shooting() {}
 virtual void AlternativeFire() {}
 virtual void AlternativeFiring() {}
 virtual void Shield() {}
 virtual void Shielding() {}
 virtual void Stand() {}
 virtual void Standing() {}
 virtual void Fly() {}
 virtual void Flying() {}
 virtual void Fall() {}
 virtual void Falling() {}
 virtual void Walk() {}
 virtual void Walking() {}
 virtual void Run() {}
 virtual void Running() {}
 virtual void Hang() {}
 virtual void Hanging() {}
 virtual void Mantle() {}
 virtual void Mantling() {}
 virtual void Crouch() {}
 virtual void Crouching() {}
 virtual void Kick() {}
 virtual void Kicking() {}
 virtual void Punch() {}
 virtual void Punching() {}
 virtual void Rush() {}
 virtual void Rushing() {}
 virtual void GrabEnemy() {}
 virtual void GrabbingEnemy() {}
 virtual void AssignGrapple() {}
 virtual void AssigningGrapple() {}
 virtual void Rappel() {}
 virtual void Rappelling() {}
 virtual void Drown() {}
 virtual void Drowning() {}
 virtual void Swim() {}
 virtual void Swimming() {}
 virtual void Throw() {}
 virtual void Throwing() {}
 virtual void Duck() {}
 virtual void Ducking() {}
 virtual void Block() {}
 virtual void Blocking() {}
 virtual void Snipe() {}
 virtual void Sniping() {}
 virtual void UseAbility() {}
 virtual void UsingAbility() {}
 virtual bool Open() {}
 virtual void Opening() {}
 virtual bool Close() {}
 virtual void Closing() {}
 virtual void Drop() {}
 virtual void Dropping() {}
 virtual void Dig() {}
 virtual void Digging() {}
 virtual void Dead() {}
 virtual void Dead_ing() {}
 virtual void Die() {}
 virtual void Dying() {}
 virtual void BeingBorn() {}
 virtual void BeingBorn_ing() {}
 virtual void Birthing() {}
 virtual void Birthing_ing() {}
 void ChangeState( MovementStates movement ) {
  switch ( state=movement ) {
                  case _Other: move=&fx_Sprite2d::Custom;            break;
                case _Resting: move=&fx_Sprite2d::Resting;           break;
                case _Jumping: move=&fx_Sprite2d::Jumping;           break;
               case _Shooting: move=&fx_Sprite2d::Shooting;          break;
      case _AlternativeFiring: move=&fx_Sprite2d::AlternativeFiring; break;
              case _Crouching: move=&fx_Sprite2d::Crouching;         break;
               case _Standing: move=&fx_Sprite2d::Standing;          break;
                 case _Flying: move=&fx_Sprite2d::Flying;            break;
                case _Falling: move=&fx_Sprite2d::Falling;           break;
                case _Kicking: move=&fx_Sprite2d::Kicking;           break;
               case _Punching: move=&fx_Sprite2d::Punching;          break;
          case _GrabbingEnemy: move=&fx_Sprite2d::GrabbingEnemy;     break;
             case _Rappelling: move=&fx_Sprite2d::Rappelling;        break;
               case _Swimming: move=&fx_Sprite2d::Swimming;          break;
               case _Drowning: move=&fx_Sprite2d::Drowning;          break;
                case _Walking: move=&fx_Sprite2d::Walking;           break;
                case _Running: move=&fx_Sprite2d::Running;           break;
                   case _Dead: move=&fx_Sprite2d::Dead_ing;          break;
                case _Hanging: move=&fx_Sprite2d::Hanging;           break;
                case _Rushing: move=&fx_Sprite2d::Rushing;           break;
               case _Mantling: move=&fx_Sprite2d::Mantling;          break;
                case _Ducking: move=&fx_Sprite2d::Ducking;           break;
               case _Blocking: move=&fx_Sprite2d::Blocking;          break;
                case _Sniping: move=&fx_Sprite2d::Sniping;           break;
           case _UsingAbility: move=&fx_Sprite2d::UsingAbility;      break;
                case _Opening: move=&fx_Sprite2d::Opening;           break;
               case _Throwing: move=&fx_Sprite2d::Throwing;          break;
                case _Closing: move=&fx_Sprite2d::Closing;           break;
               case _Dropping: move=&fx_Sprite2d::Dropping;          break;
                case _Digging: move=&fx_Sprite2d::Digging;           break;
                  case _Dying: move=&fx_Sprite2d::Dying;             break;
              case _BeingBorn: move=&fx_Sprite2d::BeingBorn_ing;     break;
               case _Birthing: move=&fx_Sprite2d::Birthing_ing;      break;
  }
 }
};

class Sprite2d : public Handle<fx_Sprite2d> {
public:
};

class Sprites2d : public Handles {
public:
 void Render() {
  FOREACH( Sprite2d, s ) s->p->Render();
 }
 void RenderMirror() {
  FOREACH( Sprite2d, s ) s->p->RenderMirror();
 }
 ADDLIST(fx_Sprite2d,p,Sprite2d);
 CLEARLISTRESET(Sprite2d);
 ~Sprites2d() { Clear(); }
};