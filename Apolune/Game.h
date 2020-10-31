#pragma once
#include "Art2d.h"
#include "GLWindow.h"

#include "Delay.h"
#include "DJ.h"

//#include "NavController.h"
#include "Proce55or.h"
#include "Tweens.h"

#include "FastGUI.h"

extern Zint sqtdCounter;

ONE(ShipQuadTreeData, { sqtdCounter++; })
 ~ShipQuadTreeData() { sqtdCounter--; }
 Zp<Proce55or> parent;
 Zp<Cartesiand> location;
 Zdouble radius;
 bool Collides(Cartesiand &in) {
  return (ddistance(location->x, location->y, in.x, in.y) < radius);
 }
MANY(ShipQuadTreeData, ShipQuadTreeDataHandle, ShipQuadTreeDataHandles, "ShipQuadTreeData", ShipQuadTreeDatas, {})
ShipQuadTreeData *Collide(Cartesiand &in) {
 FOREACH(ShipQuadTreeData, d) if (d->Collides(in)) return d;
 return nullptr;
}
 ShipQuadTreeData *Find( Cartesiand *location ) {
  FOREACH(ShipQuadTreeData, d) if (d->location.pointer == location) return d;
  return nullptr;
 }
 ShipQuadTreeData *Add(Proce55or *p, Cartesiand *location, double radius) {
  ShipQuadTreeData *d = new ShipQuadTreeData;
  d->parent = p;
  d->location = location;
  d->radius = radius;
  Append(d);
  return d;
 }
DONE(ShipQuadTreeData);

ONE(ShipQuadTreeNode, {})
 ShipQuadTreeDatas data;
 Cartesiand extents;
 Cartesian index;
MANY(ShipQuadTreeNode, ShipQuadTreeNodeHandle, ShipQuadTreeNodeHandles, "ShipQuadTreeNode", ShipQuadTreeNodes, { Init(); })
 ZIndexed<ShipQuadTreeNode> tree;
 Cartesiand each;
 void Init() {
  tree.Size((size_t)50, (size_t)50);
  each.SetRect(0.0,0.0,512.0, 512.0);
 }
 void GetBucket(double x, double y, Cartesian &out) {
  int X = (int)abs(x/each.w);
  int Y = (int)abs(y/each.h);
  out.Set(X % (int)tree.w, Y % (int)tree.h);
 }
 void Add(Proce55or *p, Cartesiand *location, double radius, ShipQuadTreeNodeHandles *out) {
  Box area(location->x - radius, location->y - radius, location->x + radius, location->y + radius);
  Cartesian start,end;
  GetBucket(UMIN(area.x,area.x2), UMIN(area.y,area.y2), start);
  GetBucket(UMAX(area.x,area.x2), UMAX(area.y,area.y2), end);
  for (int i = start.x; i <= end.x; i++) {
   for (int j = start.y; j <= end.y; j++) {
    i %= tree.w;
    j %= tree.h;
    ShipQuadTreeNode *node = &tree(i, j);
    if (!node) { OUTPUT("ERROR! Node was null at %d,%d\n", i, j); continue; }
    if (!out->find(node)) {
     node->data.Add(p, location, radius);
     out->Add(node);
    }
   }
  }
 }
 void Remove(Cartesiand *location, ShipQuadTreeNodeHandles *in) {
  EACHN(in->first, ShipQuadTreeNodeHandle, nh,{
   in->Remove(nh);
   if (!nh->p) {
    OUTPUT("HOLEY SHIT!"); 
    delete nh;
    continue;
   }
   ShipQuadTreeData *d= nh->p->data.Find(location);
   if (d) {
    nh->p->data.Remove(d);
    delete d;
   }
   delete nh;
  });
  if (in->count != 0) {
   OUTPUT("FUCK!");
   in->Clear();
  }
 }
 ShipQuadTreeData *Colliding(Cartesiand &in) {
  Cartesian q;
  GetBucket(in.x, in.y, q);
  return tree(q.x, q.y).data.Collide(in);
 }
DONE(ShipQuadTreeNode);



extern Zbool allowCheaters, spaceGameJunkie, hunterCheat, alienPickleCheat, spazCheat;

void ACHIEVE(const char *nombre);
void ACHIEVE(having cheating_ok, const char *nombre);

int RampGeneration(int generation);

ONE(ShipImageSetNode, {})
 Zp<GLImage> image;
MANY(ShipImageSetNode, ShipImageSetNodeHandle, ShipImageSetNodeHandles, "ShipImageSetNode", ShipImageSet, {})
 void Load(const char *path) {
  Strings *files = ls(path, true, true, true);
  if (files) EACH(files->first, String, f) {
   Zstring n = f->s;
   if (n.Contains(".png")) {
    ShipImageSetNode *isn = new ShipImageSetNode;
    isn->image = library.Load(n.c_str());
    if (isn->image) Append(isn);
    else delete isn;
   }
  }
 }
void Load(const char *path, having tiling) {
 Strings *files = ls(path, true, true, true);
 if (files) EACH(files->first, String, f) {
  Zstring n = f->s;
  if (n.Contains(".png")) {
   ShipImageSetNode *isn = new ShipImageSetNode;
   isn->image = library.Load(n.c_str(), tllto_Tiling);
   if (isn->image) Append(isn);
   else delete isn;
  }
 }
}
 void Get(ShipImageSetNodeHandles &out, int quantity) {
  if (count == 0) return;
  int retries = 0;
  int found = 0;
  while (found < quantity && retries < 500) {
   retries++;
   ShipImageSetNode *isn = (ShipImageSetNode *)Element((quantity + upto(count)) % count);
   if (!isn) continue;
   if (out.Includes(isn)) continue;
   out.Add(isn);
   found++;
  }
 }
 GLImage *Get(int seed) {
  if (count == 0) return null;
  GLImage *out = null;
  int retries = 0;
  while (!out && retries < 500) {
   ShipImageSetNode *isn = (ShipImageSetNode *)Element((retries + seed + 123456) % count);
   if (isn->image) { out = isn->image; break; }
   retries++;
  }
  return out;
 }
DONE(ShipImageSetNode);

ONE(ShipImageListItem, { tint.Pick(alabaster);  })
 Zp<GLImage> image;
 Crayon tint;
MANY(ShipImageListItem, ShipImageListItemHandle, ShipImageListItemHandles, "ShipImageListIitem", ShipImageList, {})
SORTING(ShipImageListItem, SortSize, {}, { result = (A->image->width + A->image->height) < (B->image->width + B->image->height) ? 1 : -1; }, {})
 double Widest() {
  int a = 0;
  FOREACH(ShipImageListItem, ili) if (ili->image && a < ili->image->width) a = ili->image->width;
  return (double)a;
 }
 double Tallest() {
  int a = 0;
  FOREACH(ShipImageListItem, ili) if (ili->image && a < ili->image->height) a = ili->image->height;
  return (double)a;
 }
 void from(ShipImageSetNodeHandles &in, int ofs=0) {
  int t = in.count+ofs;
  EACH(in.first, ShipImageSetNodeHandle, isn) if (isn->p->image) {
   ShipImageListItem *ili = new ShipImageListItem;
   ili->image=isn->p->image;
   ili->tint.Brights(t);
   Append(ili);
   t++;
  }
  SortSize();
 }
 void Pivot(double x, double y, double angle) {
  FOREACH(ShipImageListItem, ili) {
   art.Pivot(ili->image, ili->tint, transparency, x, y, 0.0, 0.0, (double) (ili->image->height / 2), (double) (ili->image->width / 2), angle, count % 3 == 0, count % 4 == 0);
  }
 }
DONE(ShipImageListItem);


#include "SpaceShaders.h"

ONE(Starfield, { time = uniform(); Seed((int)(time*1000.0)); t = tweens.random(); })
 Cartesiand position;
 Zdouble ratio;
 Zdouble time;
 Zp<Tween> t;
 Zp<GLImage> flare;
 Crayon tint;
 void Seed( int i ) {
  position.Set(uniform()*display.wd, uniform()*display.hd, uniform() * 3.0 );
  tint.Brights(i);
  switch (i % 21) {
  default:
   case 0: flare=library.Load("data/images/flares/ball1.png"); break;
   case 1: flare=library.Load("data/images/flares/ball2.png"); break;
   case 2: flare=library.Load("data/images/flares/ball3.png"); break;
   case 3: flare=library.Load("data/images/flares/ball4.png"); break;
   case 4: flare=library.Load("data/images/flares/ball5.png"); break;
   case 5: flare=library.Load("data/images/flares/ball6.png"); break;
   case 6: flare=library.Load("data/images/flares/ball7.png"); break;
   case 7: flare=library.Load("data/images/flares/ball11.png"); break;
   case 8: flare=library.Load("data/images/flares/ball9.png"); break;
   case 9: flare=library.Load("data/images/flares/ball10.png"); break;
   case 10: flare = library.Load("data/images/flares/neoflare20.png"); break;
   case 11: flare = library.Load("data/images/flares/neoflare19.png"); break;
   case 12: flare = library.Load("data/images/flares/neoflare18.png"); break;
   case 13: flare = library.Load("data/images/flares/neoflare16.png"); break;
   case 14: flare = library.Load("data/images/flares/neoflare5.png"); break;
   case 15: flare = library.Load("data/images/flares/neoflare6.png"); break;
   case 16: flare = library.Load("data/images/flares/neoflare17.png"); break;
   case 17: flare = library.Load("data/images/flares/neoflare11.png"); break;
   case 18: flare = library.Load("data/images/flares/neoflare9.png"); break;
   case 19: flare = library.Load("data/images/flares/neoflare10.png"); break;
   case 20: flare = library.Load("data/images/flares/s_lights_all_18.png"); break;
  }
 }
 void MoveBy(double x, double y) {
  position.MoveBy(x * position.z, y * position.z); 
  if (position.x < -10.0) position.x = display.wd + 5.0;
  else if (position.x > display.wd + 10.0) position.x = -5.0;
  if (position.y < -10.0) position.y = display.hd + 5.0;
  else if (position.y > display.hd + 10.0) position.y = -5.0;
 }
 void Between() {
  time += FRAMETIME;
 }
 void Render() {
  double dia = 2.0 + (!t?0:abs(t->tweend(time/10.0, true)) * 4.0 * position.z);
  art.Pivot(flare, tint, additive, position.x, position.y, 0.0, 0.0, dia, dia, position.x + position.y);
 }
MANY(Starfield, StarfieldHandle, StarfieldHandles, "Starfield", Starfields, { Generate();  })
 CALLEACH(Starfield,Render)
 CALLEACH(Starfield,Between)
 void Generate() { for (int i = 0; i < 300; i++) Append(new Starfield); }
 void MoveBy(double x, double y) { FOREACH(Starfield, s) s->MoveBy(x, y); }
DONE(Starfield);

class GameBackground {
public:
 ShipImageSet planetary;
 Zint generation;
 Cartesiand direction;
 Cartesiand position;
 ToonTroidProfile toontroid;
 TexturedPlanetProfile textured;
 EarthlikeProfile earthlike;
 RingedJovianProfile ringed;
 RingedJovian2Profile ringed2;
 LunarPlanetProfile lunar;
 FlaringStarProfile flaring;
 ClassicPlanetProfile classic;
 Earthlike2Profile earthlike2;
 ElectroPlanetProfile electro;
 AtmosphericPlanetProfile atmospheric;
 ElectronCloudsProfile electron;
 Zp<TwoDeeElement> rrs;
 Starfields starfields;
 GameBackground() {
  planetary.Load("data/images/textures", true);
  toontroid.Randomize(4);
 }
 void Init( GLWindow *parent ) {
  rrs = art.stash.rrs_filledRect_FlipY;
  SwitchTo(generation);
 }
 void SwitchTo(int generation) {
  if ( generation > 0 ) audio.manager.CueAndPlayLimited("data/sounds/FX/hyperdrive.wav", 30, (float)(1.0 + uniform()*0.2), 1.0f);
  if (generation == 2+1+1) ACHIEVE("APOLUNE_8"); // Occurs after generation "2", skipping #2 (so is really 3), then next one after during switchto
  if (generation == 11+1 ) ACHIEVE("APOLUNE_6"); // Occurs after generation "10", skipping #2 (so adds 1), then next one after during switchto
  if (generation == 101+1) ACHIEVE("APOLUNE_7"); // Occurs after generation "100", skipping #2 (so adds 1), then next one after during switchto
  this->generation = generation;
  DJTrack *a = (DJTrack *) dJ.tracks.Element(generation%dJ.tracks.count);
  if (a) { dJ.FadeToSilence(1.0);  dJ.CutTo(a->name.c_str()); }
  double spd = URANGE(0.1, 0.1 * (double)generation,display.wd/60.0);
  switch ( (generation * 3) % 4) {
  case 0: direction.Set(spd,  0.0); position.Set(-display.w,0); break;
  case 1: direction.Set(0.0,  spd); position.Set(0,-display.h); break;
  case 2: direction.Set(-spd, 0.0); position.Set(display.w,0); break;
  case 3: direction.Set(0.0, -spd); position.Set(0,display.h); break;
  }
  electro.sampleTex = planetary.Get(generation + 1234);
  electro.surfaceTex = planetary.Get(generation);
  textured.noiseTex = planetary.Get(generation + 4321);
  textured.surfaceTex = planetary.Get(generation);
  ringed2.noiseTex = planetary.Get(generation);
  lunar.tex = planetary.Get(generation);
  classic.tex1 = planetary.Get(generation);
  classic.tex2 = planetary.Get(generation + 2);
  classic.tex3 = planetary.Get(generation + 4);
  classic.tex4 = planetary.Get(generation + 7);
  if (generation == 8) ringed2.Randomize(generation);
  if (generation > 10) {
   earthlike2.tex = planetary.Get(generation + 4);
   earthlike2.tex2 = planetary.Get(generation + 3);
   earthlike2.Randomize(generation + 3);
   earthlike.Randomize(generation + 11);
   ringed.Randomize(generation);
   textured.Randomize(generation);
   ringed2.Randomize(generation);
   lunar.Randomize(generation);
   classic.Randomize(generation);
   flaring.Randomize(generation);
  }
 }
 void Between() {
  position.MoveBy(direction.x, direction.y);
  starfields.MoveBy(
   direction.x != 0.0 ? URANGE(0.1,direction.x/2.0 * UMAX(1.0,generation),display.w/100.0) : 0.0, 
   direction.y != 0.0 ? URANGE(0.1,direction.y/2.0 * UMAX(1.0,generation),display.h/100.0) : 0.0 
  );
  starfields.Between(); 
 }
 void Render( GLWindow *parent ) {
  Blending(none);
  art.PushViewport(display.w, display.h);
  art.Rectangle(crayons.Pick(black), 0, 0, display.w, display.h);

  starfields.Render();
  Blending(transparency);

  glm::mat4 trans;
#if defined(DEBUG)
  if (input.KeyDown(DX_F1)) {
   fast.text(I2S(generation.value % 12), 64, 128+16,true);
   trans = glm::translate(trans, glm::vec3(0,0, 0.0));
   trans = glm::scale(trans, glm::vec3(128,128, 1.0));
   trans = art.viewport->ortho * trans;
  }
  else {
#endif
   trans = glm::translate(trans, glm::vec3(position.x + display.w / 2 - display.w / 4, position.y + display.h / 2 - display.w / 4, 0.0));
   trans = glm::scale(trans, glm::vec3(display.w / 2, display.w / 2, 1.0));
   trans = art.viewport->ortho * trans;
#if defined(DEBUG)
  }
#endif
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);

  switch ( generation.value % 12 ) {

  case 0:
   atmospheric.haze = 2.0;
   atmospheric.brightnessGain *= 0.1;
   atmospheric.glowCutoff = 32.0;
   atmospheric.landHeight = 8.0;
   atmospheric.cloudColor.Pick(Colors::blanchedAlmond2);
   atmospheric.time += FRAMETIME;
   atmosphericplanetShader.Set(&atmospheric);
   atmosphericplanetShader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   atmosphericplanetShader.Disable();
   break;

  case 1:
   electro.time += FRAMETIME;
   electroplanetShader.Set(&electro);
   electroplanetShader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   electroplanetShader.Disable();
   break;
   
  case 2:
   earthlike2.time += FRAMETIME;
   earthlike2Shader.Set(&earthlike2);
   earthlike2Shader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   earthlike2Shader.Disable();
   break;

   /*
  case 3:
   {
   glm::mat4 trans2;
#if defined(DEBUG)
   if (input.KeyDown(DX_F1)) trans2 = trans; else {
#endif
    trans2 = glm::translate(trans2, glm::vec3(position.x + display.w / 2 - display.w / 2, position.y + display.h / 2 - display.w / 2, 0.0));
    trans2 = glm::scale(trans2, glm::vec3(display.w, display.w, 1.0));
    trans2 = art.viewport->ortho * trans2;
#if defined(DEBUG)
   }
#endif
   toontroidShader.Set(&toontroid);
   toontroid.time += FRAMETIME;
   toontroid.rotation.x += UMIN(1.0, direction.x) / 100.0;
   toontroid.rotation.y += UMIN(1.0, direction.y) / 100.0;
   toontroidShader.UpdateUniforms(trans2);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   toontroidShader.Disable();
  }
   break;
   */
  case 4:
//   classicplanetShader.Set(&classic);
//   classicplanetShader.UpdateUniforms(trans);
//   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
//   classicplanetShader.Disable();
//   break;
  case 5:
   texturedplanetShader.Set(&textured);
   texturedplanetShader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   texturedplanetShader.Disable();
   break;
  case 6:
   earthlike.time += FRAMETIME;
   earthlikeShader.Set(&earthlike);
   earthlikeShader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   earthlikeShader.Disable();
   break;
  case 7:
//   ringed.time += FRAMETIME;
//   ringedjovianShader.Set(&ringed);
//   ringedjovianShader.UpdateUniforms(trans);
//   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
//   ringedjovianShader.Disable();
//   break;
  case 8:
   ringed2.time += FRAMETIME;
   ringedjovian2Shader.Set(&ringed2);
   ringedjovian2Shader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   ringedjovian2Shader.Disable();
   break;
  case 3:
  case 9:
   lunar.time += FRAMETIME;
   lunarplanetShader.Set(&lunar);
   lunarplanetShader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   lunarplanetShader.Disable();
   break;
  case 10:
   flaring.scaleStar = 2.0;
   flaring.time += FRAMETIME;
   flaringstarShader.Set(&flaring);
   flaringstarShader.UpdateUniforms(trans);
   glDrawElements(GL_TRIANGLES, (GLsizei)rrs->elements.value, GL_UNSIGNED_INT, 0);
   flaringstarShader.Disable();
   break;

  default: break;
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }
};

class SpaceBullet : public Proce55or {
public:
 Cartesiand location;
 void Init() {

 }
 void Between() {

 }
 void Render() {

 }
};

class EnemyShip;
class PlayerShip;
class EnemyFleet {
public:
 Zp<PlayerShip> ps;
 Zp<Proce55ors> friendlyBullets;
 ShipImageSet images;
 Proce55ors enemies;
 Proce55ors enemyBullets;
 EnemyFleet() {
  images.Load("data/images/ships/");
 }
 int EnemyLimit();
 EnemyShip *Manufacture(int generation, int counter, PlayerShip *ps, int *enemyCounter, bool ignoreLimit=false);
 void Between() {
  enemyBullets.Between();
  enemyBullets.DropDone();
  enemies.Between();
  enemies.DropDone();
 }
 void Render() {
  enemyBullets.Render();
  enemies.Render();
 }
};


class SpinParticle : public Proce55or {
public:
 Zp<GLImage> image;
 Crayon tint;
 Cartesiand location, speed, size;
 SpinParticle(double px, double py, Colors c) : Proce55or() {
  image = library.Load("data/images/flares/s_redblueshot_5.png");
  tint.Pick(c);
  duration = 1.0;
  x = (int)px + (upto(6) - 3);
  y = (int)py + (upto(6) - 3);
  location.Set(x, y);
  speed.Set((double)upto(12) / 6.0 - 1.0, upto(12) / 6.0 - 1.0);
  size.Set(8.0, 8.0);
 }
 void Between() {
  expired += FRAMETIME;
  location.x += speed.x;
  location.y += speed.y;
  if (expired > duration) { this->dropMe = true; }
 }
 void Render() {
  double ratio = expired / duration;
  art.Pivot(image, tint, additive, location.x, location.y, 0.0, 0.0, size.x*(1.0 - ratio), size.y*(1.0 - ratio), expired*360.0);
 }
};

class EnemyShip : public Proce55or {
public:
 ShipQuadTreeNodeHandles qt;
 Zp<GLImage> gas;
 Cartesiands wounds;
 Zp<Cartesiand> woundRender;
 Zp<PlayerShip> ps;
 ShipImageList hull;
 Cartesiand location,speed;
 Zdouble angle,desiredAngle,radius;
 Delay shooting;
 Zint generation, gQ, hits, launches;
 Zbool fighter;
 EnemyShip(int gen, int q, bool fighter) : Proce55or() {
//  location.Set(upto(1024) - 512, upto(1024) - 512);
  location.Set(upto(8192) - 4096, upto(8192) - 4096);
  if (location.x > 0 && location.x < display.w) location.x = display.w * 2;
  if (location.y > 0 && location.y < display.h) location.y = - display.h * 2;
  angle = (double)upto(360);
  shooting.Set(0.3);
  generation = gen;
  gQ = q;
  this->fighter = fighter;
  gas = library.Load("data/images/flares/smoke9.png");
 }
 void Think();
 void Shoot( double dist );
 void Between() {
  Think();
  location.x += speed.x;
  location.y += speed.y;
  shooting.Between();
  if (!dropMe) {
   RemoveFromQuadTree();
   AddToQuadTree();
  }
 }
 void Render() {
  hull.Pivot(location.x, location.y, angle+180.0);
#if defined(DEBUG)
  fast.text(I2S(qt.count), (int)location.x, (int)location.y, true, crayons.Pick(green255), none);
#endif
 }
 void AddToQuadTree();
 void RemoveFromQuadTree();
 void OnDrop() { RemoveFromQuadTree(); }
};


class EnemyBullet : public Proce55or {
public:
 Zp<PlayerShip> ps;
 Zp<GLImage> image;
 Cartesiand speed, location, fired;
 Zdouble damage;
 Crayon tint;
 Zbool deflected;
 EnemyBullet(double px, double py, double angle, double velocity, double dmg) {
  speed.Set(velocity, 0.0);
  speed.Rotate(angle);
  location.Set(px, py);
  fired.Set(px, py);
  x = (int)px;
  y = (int)py;
  damage = dmg;
  duration = 8.0;
  image = library.Load("data/images/flares/ball12.png");
  tint.Brights(x + y);
 }
 void Between() {
  expired += FRAMETIME;
  location.x += speed.x;
  location.y += speed.y;
  if (expired > duration) { this->dropMe = true; }
 }
 void Render() {
  double ratio = expired / duration;
  art.Pivot(image, tint, additive, location.x, location.y, 0.0, 0.0, 8.0*(1.0 - ratio), 8.0*(1.0 - ratio), expired*360.0);
 }
 void CheckCollide();
};

class PlayerBullet : public Proce55or {
public:
 Zp<GLImage> image;
 Cartesiand speed, location;
 Zdouble damage,size;
 Crayon tint;
 Zbool player2;
 PlayerBullet(double px, double py, double angle, double velocity, double dmg) {
  size = 1.0;
  speed.Set(velocity, 0.0);
  speed.Rotate(angle);
  location.Set(px, py);
  x = (int)px;
  y = (int)py;
  damage = dmg;
  duration = 10.0;
  image = library.Load("data/images/flares/ball12.png");
  switch (upto(3)) {
  case 0: tint.Double(0.0, 0.5*uniform() + 0.5, 0.0, 1.0); break;
  case 1: tint.Pick(green255); break;
  case 2: default: tint.Pick(alabaster); break;
  }
 }
 void Between() {
  expired += FRAMETIME;
  location.x += speed.x;
  location.y += speed.y;
  if (expired > duration) { this->dropMe = true; }
  else CheckCollide();
 }
 void Render() {
  double ratio = expired / duration;
  art.Pivot(image, tint, additive, location.x, location.y, 0.0, 0.0, 10.0*(1.0 - ratio)* size, 10.0*(1.0 - ratio)* size, expired*360.0);
 }
 void CheckCollide(); // Uses QuadTree
 void CheckCollide(EnemyShip *e); // Old Method
};

class PlayerShip : public Proce55or {
public:
 Zbool dead;
 Zp<EnemyFleet> enemies;
 Proce55ors fx;
 ShipImageSet images;
 ShipImageList hull;
 Cartesiand location;
 Zdouble angle,time,deflectionTime;
 Zp<Tween> blinker,flashing;
 Crayon lamphue,flashhue;
 Zp<GLImage> lamp,lightsource,deflector;
 Zint hits;
 Zdouble radius, stamina;
 PlayerShip() {
  images.Load("data/images/ships/");
  Mutate(1);
  location.Set(display.w / 2, display.h / 2);
  angle = -90.0;
  blinker = tweens.find("Humanized Triple");
  flashing = tweens.find("Humanized Triple");
  lamp = library.Load("data/images/flares/flare.png");
  lightsource = library.Load("data/images/icon/lightbulb_t.png");
  deflector = library.Load("data/images/flares/s_lights_all_81.png");
  lamphue.Pick(alabaster);
  flashhue.Pick(alabaster);
 }
 ShipImageSetNodeHandles parts;
 int ShipVariant() {
  ShipImageSetNodeHandle *a = (ShipImageSetNodeHandle *)parts.first;
  return a ? images.IndexOf(a->p) : 1;
 }
 void Mutate(int generation) {
  parts.Clear();
  images.Get(parts, 1);
  hull.from(parts);
  radius = UMAX(10.0, (double)UMIN(hull.Widest(), hull.Tallest()) / 2.0);
 }
 void TurnLeft() {
  angle -= 1.95 * Generation();
 }
 void TurnRight() {
  angle += 1.95 * Generation();
 }
 void RebuildStamina();
 bool CanDodge();
 void Dodge(int key);
 double Generation();
 Zdouble lastDownW, lastDownA, lastDownS, lastDownD;
 Zbool mustW, mustA, mustS, mustD;
 void Between() {
  RebuildStamina();
  int oldSecond = (int)time;
  time += FRAMETIME;
  if ((int)time != oldSecond) flashhue.Brights(oldSecond);
  if ( input.KeyDown(DX_LEFT)  || input.KeyDown(DX_NUMPAD4) || input.KeyDown(DX_COMMA) || input.KeyDown(DX_LBRACKET) ) TurnLeft();
  if ( input.KeyDown(DX_RIGHT) || input.KeyDown(DX_NUMPAD6) || input.KeyDown(DX_PERIOD) || input.KeyDown(DX_RBRACKET) ) TurnRight();
  bool _w = input.KeyDown(DX_W) || input.KeyDown(DX_UP)    || input.KeyDown(DX_NUMPAD8);
  bool _a = input.KeyDown(DX_A);
  bool _s = input.KeyDown(DX_S) || input.KeyDown(DX_DOWN)  || input.KeyDown(DX_NUMPAD2);
  bool _d = input.KeyDown(DX_D);
  if (!mustW) if ( _w ) {
   double deltaDown = (time - lastDownW);
   if (CanDodge() && deltaDown > FRAMETIMEd * 2.0 && deltaDown < 0.2) { Dodge(DX_W); lastDownW = time; mustW = true; }
   else { lastDownW = time;  location.y -= 2.34 * Generation(); if (location.y < -100.0) location.y = -100.0; ThrustDust(270.0); }
  }
  if (!mustA) if ( _a ) {
   double deltaDown = (time - lastDownA);
   if (CanDodge() && deltaDown > FRAMETIMEd * 2.0 && deltaDown < 0.2) { Dodge(DX_A); lastDownA = time; mustA = true; }
   else { lastDownA = time; location.x -= 2.34 * Generation(); if (location.x < -100.0) location.x = -100.0; ThrustDust(180.0); }
  }
  if (!mustS) if ( _s ) {
   double deltaDown = (time - lastDownS);
   if (CanDodge() && deltaDown > FRAMETIMEd * 2.0 && deltaDown < 0.2) { Dodge(DX_S); lastDownS = time; mustS = true; }
   else { lastDownS = time;  location.y += 2.34 * Generation(); if (location.y > display.hd + 100.0) location.y = display.hd + 100.0; ThrustDust(90.0); } 
  }
  if (!mustD) if ( _d ) {
   double deltaDown = (time - lastDownD);
   if (CanDodge() && deltaDown > FRAMETIMEd * 2 && deltaDown < 0.2) { Dodge(DX_D); lastDownD = time; mustD = true; }
   else { lastDownD = time; location.x += 2.34 * Generation(); if (location.x > display.wd + 100.0) location.x = display.wd + 100.0; ThrustDust(0.0); }
  }
  if (!_w) mustW = false;
  if (!_a) mustA = false;
  if (!_s) mustS = false;
  if (!_d) mustD = false;
  fx.Between();
  fx.DropDone();
 }
 void ThrustDust( double moveAngle );
 void Render() {
  fx.Render();
  hull.Pivot(location.x, location.y, angle+180.0);
  if (time < 1.0) { // initial flash
   double blinking = blinker->tweenf((float)time, true);
   double dia = (double)lamp->width * blinking;
   art.Stretch(lamp, lamphue.ScaleByAlpha(blinking*255.0), additive, location.x - dia / 2.0, location.y - dia / 2.0, dia, dia);
  } else { // pulsing
   art.Pivot(lightsource, crayons.Brights(0), transparency, location.x, location.y, 0.0, 0.0, 2.5, 2.5, angle);
   double blinking = flashing->tweend(time/10.0,true);
   double dia = (double)lamp->width / 16.0 * blinking;
   art.Pivot(lamp, flashhue, additive, location.x, location.y, 0.0,0.0, dia,dia, location.x);
  }
  if (deflectionTime > 0.0) {
   Crayon color(alabaster);
   color.ScaleByAlphaf(deflectionTime);
   art.Stretch(deflector, color, additive, location.x - radius * 2.0, location.y - radius * 2.0, radius*4.0, radius*4.0);
   deflectionTime -= FRAMETIME;
  }
 }
};

#include "Box2D.h"

class Tether : public TouchableBox2dWorld {
public:
 Zp<b2Body> head;
 Zp<b2Body> tail;
 Zint links;
 void Inflation()
 {
  links = 20;
  {
   b2PolygonShape shape;
   shape.SetAsBox(0.06f, 0.0125f);

   b2FixtureDef fd;
   fd.shape = &shape;
   fd.density = 40.0f;
   fd.friction = 2.5f;

   b2RevoluteJointDef jd;
   jd.collideConnected = false;
   jd.enableLimit = true;
   jd.lowerAngle = -90.0f;
   jd.upperAngle = 90.0f;
   jd.maxMotorTorque = 1.0;


   const float y = 65.0f;
   b2Body *prevBody = nullptr;
   for (int i = 0; i < links; ++i) {
    b2BodyDef bd;
    bd.angularDamping = 0.01;
    bd.linearDamping = 0.01;
//    bd.linearDamping = 0.01f;   // recommended
//    bd.angularDamping = 0.03f; // between 0.0f - 0.1f recommended
    bd.allowSleep = true;
    bd.type = b2_dynamicBody;
    bd.position.Set(0.5 + (double)i, y);
    b2Body* b = m_world->CreateBody(&bd);

    if (i == 0) head = b;
    else if (i == links - 2) {
     jd.collideConnected = true;
     fd.density = 60.0f;
    } else if (i == links - 1) { // last
     jd.collideConnected = true;
//     jd.enableLimit = false;
     fd.density = 40.0f;
     tail = b;
    }
    if (!prevBody) { prevBody = b; continue; }
    b->CreateFixture(&fd);
    jd.Initialize(prevBody, b, b2Vec2((float)(i), y));
    m_world->CreateJoint(&jd);
    prevBody = b;
   }
   /*
   b2MassData tail_mass;
   tail->GetMassData(&tail_mass);
   tail_mass.mass=50.0f; //kg
   tail->SetMassData(&tail_mass);
   */
  }
 }

 
 static TouchableBox2dWorld* Create()
 {
  return new Tether;
 }
};

class Player2 {
public:
 Zdouble angle;
 Zp<GLImage> image,hoseSegment;
 Zbool active;
 Tether p;
 Zp<TouchableBox2dWorld> simulated;
 Zp<b2Body> tail,head;
 Zint shots;
 Cartesiand location;
 void Location(Cartesiand &out) {
  out.Set(&location);
 }
 Player2() {
  Init();
  tail = p.tail;
  head = p.head;
 }
 bool AllowForce(double dx, double dy);
 void Jolt(int key, double amount = 10.0) {
  switch (key) {
  case DX_T: if (AllowForce(0, -1)) { b2Vec2 f = tail->GetWorldVector(b2Vec2(0.0f, (float)-amount)); tail->ApplyLinearImpulse(f, tail->GetPosition()); } break;
  case DX_F: if (AllowForce(-1, 0)) { b2Vec2 f = tail->GetWorldVector(b2Vec2((float)-amount, 0.0f));  tail->ApplyLinearImpulse(f, tail->GetPosition()); } break;
  case DX_G: if (AllowForce(0, 1))  { b2Vec2 f = tail->GetWorldVector(b2Vec2(0.0f, (float)amount));  tail->ApplyLinearImpulse(f, tail->GetPosition()); } break;
  case DX_H: if (AllowForce(1, 0))  { b2Vec2 f = tail->GetWorldVector(b2Vec2((float) amount,0.0f));   tail->ApplyLinearImpulse(f, tail->GetPosition()); } break;
  default:break;
  }
 }
 void Force(int key, double amount = 10.0) {
  switch (key) {
  case DX_T: if ( AllowForce(0,-1) ) { b2Vec2 f = tail->GetWorldVector(b2Vec2(0.0f, (float)-amount)); tail->ApplyForce(f, tail->GetPosition()); } break;
  case DX_F: if ( AllowForce(-1,0) ) { b2Vec2 f = tail->GetWorldVector(b2Vec2((float)-amount, 0.0f)); tail->ApplyForce(f, tail->GetPosition()); } break;
  case DX_G: if ( AllowForce(0,1)  ) { b2Vec2 f = tail->GetWorldVector(b2Vec2(0.0f, (float)amount));  tail->ApplyForce(f, tail->GetPosition()); } break;
  case DX_H: if ( AllowForce(1,0)  ) { b2Vec2 f = tail->GetWorldVector(b2Vec2((float)amount, 0.0f));  tail->ApplyForce(f, tail->GetPosition()); } break;
  default:break;
  }
 }
 void Controls() {
  if (input.KeyDown(DX_L)) angle += 1.93;
  if (input.KeyDown(DX_J)) angle -= 1.93;
  if (input.KeyDown(DX_T) || input.KeyDown(DX_I)) Force(DX_T);
  if (input.KeyDown(DX_F)) Force(DX_F);
  if (input.KeyDown(DX_G) || input.KeyDown(DX_K)) Force(DX_G);
  if (input.KeyDown(DX_H)) Force(DX_H);
  // Player 1.
  if (input.KeyDown(DX_W)) Force(DX_G,5.0);
  if (input.KeyDown(DX_A)) Force(DX_F,5.0);
  if (input.KeyDown(DX_S)) Force(DX_T,5.0);
//  if (input.KeyDown(DX_D)) Force(DX_H,5.0);
 }
 Zbool recentlyActivated;
 void Between() {
  if (!active) {
   CheckForActivation();
   return;
  }
  Controls();
  if (recentlyActivated) {
   Jolt(DX_G,1.0);
   recentlyActivated = false;
  }
  p.Step();
 }
 void Render() {
  if (!active) return;
  DrawRelative();
 }
 void CheckForActivation();
 void DrawRelative();
 void Init() {
  image = library.Load("data/images/player2.png");
  hoseSegment = library.Load("data/images/tube.png");
  p.hz = FPS;
  p.enableContinuous = false;
  p.gravity.Set(0.0f,0.1f);
  simulated = &p;
  p.BigBang();
 }
 void Hold( Cartesian &action ) {
  b2MouseJointDef md;
  md.bodyA = p.m_groundBody;
  md.bodyB = tail;
  md.target = p.m_mouseWorld;
  md.maxForce = 100.0;
  p.m_mouseJoint = (b2MouseJoint*)p.m_world->CreateJoint(&md);
  tail->SetAwake(true);
  p.view.ConvertScreenToWorld(action.x,action.y);
  p.MouseDown();
  p.view.ConvertScreenToWorld(action.x2, action.y2);
  p.MouseMove();
 }
 void Release() { p.MouseUp(); }
};

class GameStats {
public:
 Zdouble survived;
 Zint kills, hits, damage, shots, fuel, enemies, fighters, last, greatest, next, accumulated;
 void operator =(GameStats &in) {
  survived = in.survived;
  kills = in.kills;
  hits = in.hits;
  damage = in.damage;
  shots = in.shots;
  fuel = in.fuel;
  enemies = in.enemies;
  fighters = in.fighters;
  last = in.last;
  greatest = in.greatest;
  next = in.next;
  accumulated = in.accumulated;
 }
 int FinalScore() {
  return (int)(survived + (double)kills * ( (enemies > 0 ? 1.0+iratiod(fighters, enemies) : 1) + (shots > 0 ? 1.0 + iratiod(hits, shots) : 1) ) / 2.0 + kills + accumulated) * 10;
 }
};

class Game;
extern Zp<Game> game;

class Game : public GLWindow {
public:
 ShipQuadTreeNodes qtEnemy;
 GameStats stats,was;
 GameBackground bg;
 EnemyFleet enemies;
 Proce55ors friendlyBullets, friendlyBullets2;
 PlayerShip player;
 Player2 player2;
 Delay generate;
 Zint generateCounter;
 Delay shootDelay,healDelay,shootDelay2;
 Zdouble deathAnim,deathTime,mineDelay;
 Zp<GLImage> dethImage,damageFlare;
 void OnLoad() {
  game = this;
  enemies.ps = &player;
  player.enemies = &enemies;
  Fullscreen();
  background.Pick(clear);
  bg.Init(this);
  generate.Set(1.0);
  generateCounter++;
  Generate();
  input->hideCursor = true;
  stats.next = 30;
  shootDelay.Set(FRAMETIME * 3);
  shootDelay.Reset();
  shootDelay2.Set(FRAMETIME * 2);
  shootDelay2.Reset();
  healDelay.Set(UMAX(1.0,60.0 - (double)bg.generation));
  healDelay.Reset();
  deathAnim = 1.0;
  dethImage = library.Load("data/images/deth.png");
  damageFlare = library.Load("data/images/flares/flare.png");
  stats.next = GetNext(0);
 }
 int GetNext( int gen ) {
  switch (gen) {
  case 0: return 30;
  case 1: return 50;
  case 2: return 75; // skipped
  case 3: return 75;
  case 4: return 100;
  case 5: return 120;
  case 6: return 160;
  case 7: return 200;
  case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: return stats.next + (20 + bg.generation * 5);
  default: return stats.next + (20 + absi(bg.generation - 3) * 10);
  }
 }
 void AdvanceGeneration() {
  audio.Stop();
  audio.Clear();
  audio.manager.sounds.Clear();
  stats.last = bg.generation;
  if ( bg.generation == 1 ) bg.SwitchTo(3);
  else bg.SwitchTo(bg.generation + 1);
  stats.next = GetNext(bg.generation);
  if (player.hits > UMAX(1, abs(50 - RampGeneration(game->bg.generation)))*2) {
   player.hits = 0;
   audio.manager.CueAndPlay("data/sounds/FX/quest_complete.wav", 1.0, 1.0);
   for ( int i=0; i<50; i++ ) {
    SpinParticle *s = new SpinParticle(player.location.x, player.location.y, green255);
    s->speed.Set(1.2, 0);
    s->speed.Rotate(uniform()*360.0);
    s->size.Set(20.0,20.0);
    player.fx.Add(s);
   }
  }
  player.time = 0.0;
  player.lamphue.Brights(bg.generation);
  if ( bg.generation > 10 ) player.blinker = tweens.randomNot1();
 }
 void CheckGenerationAdvance() {
  if (stats.kills.value > 0 && was.kills.value < stats.next && stats.kills.value >= stats.next) AdvanceGeneration();
 }
 Zdouble lastShot;
 void Between() {
  lastShot += FRAMETIME;
  was = stats;
  bg.Between();
  friendlyBullets.Between();
  friendlyBullets2.Between();
  //EACH(friendlyBullets.first, PlayerBullet, pb) EACH(enemies.enemies.first, EnemyShip, e) pb->CheckCollide(e);
  EACH(enemies.enemyBullets.first, EnemyBullet, eb) eb->CheckCollide();
  friendlyBullets.DropDone();
  friendlyBullets2.DropDone();
  CheckGenerationAdvance();
  enemies.Between();
  generate.Between();
  if (!generate) {
   Generate();
   generate.Set(UMAX(1.0, 15.0 - (double)upto(RampGeneration(bg.generation))));
   generate.Reset();
  }
  if (player.dead) return;
  player.Between();
  player2.Between();
  if (was.kills < 30 && stats.kills == 30) ACHIEVE("APOLUNE_2");
  if (was.kills < 50 && stats.kills == 50) ACHIEVE("APOLUNE_3");
  if (was.kills < 150 && stats.kills == 150) {
   audio.manager.CueAndPlay("data/sounds/UI/pad_confirm.wav", 1.0, 1.0);
   while (!(player.blinker = tweens.randomNot1()));
  }
  if (was.kills < 420 && stats.kills == 420) {
   ACHIEVE("APOLUNE_12");
   audio.manager.CueAndPlay("data/sounds/UI/pad_confirm.wav", 1.0, 1.0);
   while (!(player.blinker = tweens.randomNot1()));
  }
  if (was.kills < 2000 && stats.kills == 2000) ACHIEVE("APOLUNE_10");
  if (was.kills < 1000 && stats.kills == 1000) {
   ACHIEVE("APOLUNE_11");
   audio.manager.CueAndPlay("data/sounds/UI/pad_confirm.wav", 1.0, 1.0);
   while (!(player.blinker = tweens.randomNot1()));
  }
  if (was.kills < 500 && stats.kills == 500) {
   ACHIEVE("APOLUNE_9");
   audio.manager.CueAndPlay("data/sounds/UI/pad_confirm.wav", 1.0, 1.0);
   while (!(player.blinker = tweens.randomNot1()));
  }
  if (was.kills < 300 && stats.kills == 300) {
   ACHIEVE("APOLUNE_5");
   audio.manager.CueAndPlay("data/sounds/UI/pad_confirm.wav", 1.0, 1.0);
   while ( !(player.blinker = tweens.randomNot1()) );
  }
  if (was.kills < 200 && stats.kills == 200) {
   ACHIEVE("APOLUNE_4");
   audio.manager.CueAndPlay("data/sounds/UI/pad_confirm.wav", 1.0, 1.0);
   while ( !(player.blinker = tweens.randomNot1()) );
  }
  if (stats.kills < 200) {
   if (input->KeyHit(DX_LCONTROL) || input->KeyHit(DX_RCONTROL)) {
    lastShot = 0.0;  PlayerShoot();
   }
   if (input->KeyHit(DX_Y) || input->KeyHit(DX_U)) {
    lastShot = 0.0;  PlayerShoot(true);
   }
  } else {
   if (mineDelay > 0.0) { mineDelay -= FRAMETIME; if (mineDelay < 0.0) mineDelay = 0.0; }
   if (mineDelay <= 0.0) if (input->KeyDown(DX_RCONTROL)) PlayerMine();
   shootDelay.Between();
   if (!shootDelay && (input->KeyDown(DX_LCONTROL)) ) {
    lastShot = 0.0;
    PlayerShoot();
    shootDelay.Reset();
   }   
   shootDelay2.Between();
   if (!shootDelay2 && (input->KeyDown(DX_Y) || input->KeyDown(DX_U))) {
    lastShot = 0.0;
    PlayerShoot(true);
    shootDelay2.Reset();
   }

  }
  stats.survived += FRAMETIME;
 }
 void RenderAfter() {
  bg.Render(this);
  enemies.Render();
  friendlyBullets.Render();
  if (player.dead) {
   Death();
  } else {
   if (player.radius > 50.0) {
    player.Render();
    friendlyBullets2.Render();
    player2.Render();
   } else {
    friendlyBullets2.Render();
    player2.Render();
    player.Render();
   }
   HUD();
  }
 }
 Zdouble killText;
 Zint sgjCheatCount;
 void HUD() {
  int totalHits = UMAX(1, abs(50 - RampGeneration(game->bg.generation)));
  double perc = iratiod(player.hits,totalHits);
  if (perc > 0.0) {
   Crayon heat;
   heat.Pick(orangeRed);
   heat.ScaleByAlpha(255.0*perc);
   art.Pivot(damageFlare, heat, additive, player.location.x, player.location.y, 0.0, 0.0, player.radius*3.0, player.radius*3.0, player.location.x + player.location.y);
  }
  if (stats.kills > 0) {
   if (was.kills != stats.kills) {
    killText = 0.2;
   }
   if (killText > 0.0) {
    killText -= FRAMETIME;
    double ratio = killText / 1.0;
    fast.SaveState();
    double dia = ratio * 128.0;
    int dia2 = (int)(dia / 2.0);
    fast.tfont.w += (int)dia;
    fast.tfont.h += (int)dia;
    fast.text(digicaps,I2S(stats.kills), 64 - dia2, 64 - dia2, crayons.Pick(hotPink), additive);
    fast.RecallState();
   }
   fast.text(digicaps, I2S(stats.kills), 64, 64, crayons.Pick(red255), additive);
  }
  if (player.CanDodge() && player.stamina > 0.0) {
   double t = player.stamina;
   art.BevelRect(crayons.Pick(skyBlue), none, 8.0, display->w2d - t * display->w2d, 0, t*display->wd, 16.0);
  }
  double hitWidth=iratiod(display->w2,UMAX(1, abs(50 - RampGeneration(game->bg.generation))));
  if (player.hits > 0) {
   art.BevelRect(crayons.jami(0,42,0,0,255), none, 8.0, display->w2d - perc * display->w2d - hitWidth, display->h - 16, perc*display->wd + 2.0*hitWidth, 16.0);
   art.BevelRect(crayons.Pick(red255), none, 8.0, display->w2d - perc * display->w2d, display->h - 16, perc*display->wd, 16.0);
   healDelay.Between();
   if (!healDelay) {
    player.hits--;
    healDelay.Set(UMAX(1.0, 60.0 - (double)bg.generation));
    healDelay.Reset();
   }
  }
  if (allowCheaters) {
   if (spaceGameJunkie) {
    if (input->KeyHit(DX_NUMPADPLUS)) { AdvanceGeneration(); stats.survived += 100.0; sgjCheatCount++; if ( sgjCheatCount == 100+1 ) ACHIEVE(true,"APOLUNE_21"); }
   }
   if (hunterCheat) {
    if (input->KeyHit(DX_NUMPADMINUS))  { stats.kills = 199; ACHIEVE(true,"APOLUNE_20"); }
    if (input->KeyHit(DX_NUMPADPERIOD)) { stats.kills = 299; ACHIEVE(true,"APOLUNE_20"); }
    if (input->KeyHit(DX_NUMPADSTAR))   { stats.kills = 499; ACHIEVE(true,"APOLUNE_20"); }
    if (input->KeyHit(DX_NUMPADSLASH)) { stats.kills++; ACHIEVE(true, "APOLUNE_20"); }
   }
  }
  if (input->KeyHit(DX_ESCAPE) || input->KeyHit(DX_BACKTICK) || input->KeyHit(DX_BACKSPACE)) {
   ACHIEVE("APOLUNE_13");
   audio.manager.CueAndPlay("data/sounds/Legacy/sfx_Specops_selfdestruct2.wav", 1.0, 0.5);
   deathReason = "CHICKENED OUT";
   player.dead = true;
  }
 }
 void Generate() {
  if (enemies.enemies.count > 300) return;
  enemies.Manufacture(bg.generation, generateCounter,&player,&stats.enemies.value);
  generateCounter++;
 }
 void Player2Shoot() {
  Cartesiand location;
  player2.Location(location);
  PlayerBullet *b = nullptr;
  if (stats.kills >= 300) { // Triple Threat
   friendlyBullets2.Add(b = new PlayerBullet(location.x, location.y, player2.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 8.0;
   b->tint.Pick(alabaster);
   b->image = library.Load("data/images/flares/orange.png");
   b->speed.Rotate(-5.0);
   b->player2 = true;
   friendlyBullets2.Add(b = new PlayerBullet(location.x, location.y, player2.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 8.0;
   b->tint.Pick(alabaster);
   b->speed.Rotate(5.0);
   b->player2 = true;
   b->image = library.Load("data/images/flares/orange.png");
  }
  friendlyBullets2.Add(b = new PlayerBullet(location.x, location.y, player2.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
  b->tint.Pick(alabaster);
  b->player2 = true;
  b->image = library.Load("data/images/flares/orange.png");
  player2.shots++;
  audio.manager.CueAndPlay("data/sounds/Legacy/sfx_fighter_shoot.wav", (float)((1.4 + uniform()*0.2) - 0.25), 
   stats.kills < 300 ? 1.0f : 0.5f);
 }
 void PlayerShoot( bool p2=false ) {
  if (p2 && game->player2.active) { Player2Shoot(); return; }
  if (friendlyBullets.count > 1000) { if ( lastShot > 0.5 ) audio.manager.CueAndPlay("data/sounds/UI/locked.wav", 1.0, 0.5); return; }
  stats.shots++;
  audio.manager.CueAndPlay("data/sounds/Legacy/sfx_fighter_shoot.wav", (float)((1.0 + uniform()*0.2) - 0.25), stats.kills < 300 ? 1.0f : 0.5f);
  PlayerBullet *b = nullptr;
  if (stats.kills >= 2000) {
   Cartesiand offset;
   offset.Set(10.0, 0.0);
   offset.Rotate(player.angle + 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle - 10.0 - 180.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(10.0, 0.0);
   offset.Rotate(player.angle - 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle + 10.0 - 180.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(10.0, 0.0);
   offset.Rotate(player.angle + 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle - 8.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(10.0, 0.0);
   offset.Rotate(player.angle - 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle + 8.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(5.0, 0.0);
   offset.Rotate(player.angle + 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle + 5.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(5.0, 0.0);
   offset.Rotate(player.angle - 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle + 5.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 5.0;
   friendlyBullets.Add(b=new PlayerBullet(player.location.x, player.location.y, player.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 5.0;
  } else
  if (stats.kills >= 1000) {
   Cartesiand offset;
   offset.Set(10.0, 0.0);
   offset.Rotate(player.angle + 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle-10.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(10.0, 0.0);
   offset.Rotate(player.angle - 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle+10.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(10.0, 0.0);
   offset.Rotate(player.angle + 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle-8.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(10.0, 0.0);
   offset.Rotate(player.angle - 90);
   friendlyBullets.Add(b=new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle+8.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(5.0, 0.0);
   offset.Rotate(player.angle + 90);
   friendlyBullets.Add(b = new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle+5.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   offset.Set(5.0, 0.0);
   offset.Rotate(player.angle - 90);
   friendlyBullets.Add(b = new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle+5.0, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
   friendlyBullets.Add(b = new PlayerBullet(player.location.x, player.location.y, player.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
   b->duration = 4.0;
  } else {
   if (stats.kills >= 500) { // 5-shot
    Cartesiand offset;
    offset.Set(10.0, 0.0);
    offset.Rotate(player.angle + 90);
    friendlyBullets.Add(b = new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
    b->duration = 5.0;
    offset.Set(10.0, 0.0);
    offset.Rotate(player.angle - 90);
    friendlyBullets.Add(b = new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
    b->duration = 5.0;
   }
   if (stats.kills >= 300) { // Triple Threat
    Cartesiand offset;
    offset.Set(5.0, 0.0);
    offset.Rotate(player.angle + 90);
    friendlyBullets.Add(b = new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
    b->duration = 8.0;
    offset.Set(5.0, 0.0);
    offset.Rotate(player.angle - 90);
    friendlyBullets.Add(b = new PlayerBullet(player.location.x + offset.x, player.location.y + offset.y, player.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
    b->duration = 8.0;
   }
   friendlyBullets.Add(b = new PlayerBullet(player.location.x, player.location.y, player.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation));
  }
 }
 void PlayerMine() {
  mineDelay = FRAMETIMEd * 5.0 + (double)UMAX(1, 100 - bg.generation) / 50.0;
  double staminaRequired = 0.1+0.1 * (1.0 - iratiod(UMIN(100, bg.generation), 100));
  if (player.stamina < staminaRequired) { audio.manager.CueAndPlay("data/sounds/UI/locked.wav", 1.0, 1.0); return; }
  if (friendlyBullets.count > 1000) { if (lastShot > 0.5) audio.manager.CueAndPlay("data/sounds/UI/locked.wav", 1.0, 0.5); return; }
  stats.shots++;
  player.stamina -= staminaRequired;
  audio.manager.CueAndPlay("data/sounds/Legacy/sfx_energymines.wav", (float)((1.0 + uniform()*0.2) - 0.25),  1.0f);
  PlayerBullet *b = nullptr;
  b = new PlayerBullet(player.location.x, player.location.y, player.angle, UMAX(4.0, UMIN(1.0*(double)bg.generation, 20.0)), 2.0*(double)bg.generation);
  b->duration = 30.0 + 30.0 * ((double)UMAX(1, 100 - bg.generation) / 50.0);
  b->size = 3.0;
  b->speed.Set(0.0, 0.0);
  b->tint.Pick(macNcheese);
  b->image = library.Load("data/images/flares/ball9.png");
  friendlyBullets.Add(b);
 }
 Zstring deathReason;
 Zint finalScore;
 void Death() {
  input->hideCursor = false;
  if (deathTime == 0.0) {
   finalScore = stats.FinalScore();
  }
  if (deathAnim > 0.0) {

  }
  deathTime += FRAMETIME;
  deathAnim -= FRAMETIME;
  Cartesiand area;
  area.SetRect(display->wd / 2.0 - (double)(dethImage->width/2), display->hd / 2.0 - (double)(dethImage->height/2), (double)dethImage->width, (double)dethImage->height);
  art.Rectangle(crayons.jami(0, 0, 0, 0, 196), transparency, area.x - 16, area.y - 16, area.w + 32, area.h + 32);
  art.Stretch(dethImage, crayons.jami(0, 64, 64, 64, 255), additive, area.x, area.y, area.w, area.h);
  fast.SaveState();
  if (area.x < 0.0) area.x = 16.0;
  if (area.x2 > display->wd) area.x2 = display->wd-16.0;
  if (area.y < 0.0) area.y = 32.0;
  if (area.y2 > display->hd) area.y2 = display->hd-32.0;
  area.SetCorners(area.x, area.y, area.x2, area.y2);
  fast.tfont.w = 24;
  fast.tfont.h = 24;
  fast.text(digicaps,
   FMT("%s\n\n"
    "Survived ..... %1.1f minutes\n"
    "Kills ........ %d\n"
    "Enemies ...... %d\n"
    "Fighters ..... %d %1.1f%%\n"
    "Hits Enemy ... %d\n"
    "%s %d%s\n"
    "Wasted ....... %d\n"
    "Accuracy ..... %1.1f%%\n"
    "Your Ship .... Variant %d\n"
    "Difficulty ... %1.1f%%\n"
    "Damaged ...... %d times\n"
    "Generation ... %d\n"
    "Last Killed .. Level %d\n"
    "Epigenetics .. %d\n"
    "Next Level ... %d kills\n",
    deathReason.c_str(),
    (double)(stats.survived / 60.0),
    (int)stats.kills.value,
    (int)stats.enemies.value,
    (int)stats.fighters.value, (double)(stats.enemies == 0 ? 0.0 : 100.0*iratiod(stats.fighters, stats.enemies)),
    (int)stats.hits.value,
    player2.active ? "Player 1 Shots" : "Shots Fired ..",
    (int)stats.shots.value,
    player2.active ? FMT("\nPlayer 2 Shots %d",(int)player2.shots).c_str() : "",
    (int)((stats.shots.value+player2.shots) - stats.hits.value),
    (double)((player2.shots+stats.shots) == 0 ? 0.0 : 100.0 * ((double)stats.hits / (double)(stats.shots+player2.shots))),
    (int)player.ShipVariant(),
    (double)(100.0*player.radius / 20.0),
    (int)stats.damage.value,
    (int)bg.generation.value,
    (int)stats.last.value,
    (int)stats.accumulated.value,
    (int)stats.next.value
   ).c_str(), 
   (int)display->w2d, (int)area.y+32, crayons.Pick(orangeRed), additive
  );
  fast.RecallState();
  fast.text(digicaps, "FINAL SCORE", (int)(area.x + area.w / 2.0), (int)area.y2 - 128 - 64 - 4 + (display->h <= 720 ? 64 : 0), 24, 24, 0, 3, crayons.Pick(orangeRed), additive);
  fast.text(digicaps, FMT("%d",(int)finalScore).c_str(), (int)(area.x + area.w / 2.0), (int)area.y2 - 128 + (display->h <= 720 ? 64 : 0), 64, 64, 0, 3, crayons.Pick(orangeRed), additive);
  if (fast.button(digicaps, "AGAIN", (int)area.x, (int)area.y2 - 64, 180, 64) || (deathTime > 5.0 && (input->KeyHit(DX_ENTER) || input->KeyHit(DX_LCTRL) || input->KeyHit(DX_RCTRL)))) RestartGame();
  if (allowCheaters) if (fast.button(digicaps, "CONTINUE", (int)(area.x + area.w/2.0) - (128+16), (int)area.y2 - 64, 256+32, 64)) ContinueGame();
  if (fast.button(digicaps, "EXIT", (int)area.x2 - 180, (int)area.y2 - 64, 180, 64)) ReturnToGameMenu();
 }
 void ContinueGame() {
  player.hits = 0;
  player.dead = false;
  deathTime = 0.0;
  deathAnim = 1.0;
 }
 void RestartGame();
 void ReturnToGameMenu();
};

