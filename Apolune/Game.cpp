#include "Game.h"

Zp<Game> game;

Zbool allowCheaters,spaceGameJunkie, hunterCheat, alienPickleCheat, spazCheat;

Zint sqtdCounter;

#include "Steamworks.h"
#include "GLSetup.h"

void ACHIEVE(const char *nombre) {
//#if !defined(DEBUG)
//Alert("ACHIEVE", nombre);
#if defined(USE_STEAMWORKS)
 if (!allowCheaters && !spaceGameJunkie && !hunterCheat && !alienPickleCheat && !spazCheat) 
  if (!steamworks.SetAchievement(nombre)) {}// Alert("Apolune: STEAMWORKS FAIL", FMT("Apolune says: COULD NOT SET ACHIEVEMENT %s, RETURNED FALSE.  Game will continue, but who knows what will be achieved... :(", nombre).c_str()); }
#endif
//#endif
}

void ACHIEVE(having cheating_ok, const char *nombre) {
//#if !defined(DEBUG)
// Alert("ACHIEVE CHEAT", nombre);
#if defined(USE_STEAMWORKS)
 if (!steamworks.SetAchievement(nombre)) {}// Alert("Apolune: STEAMWORKS FAIL", FMT("Apolune says: COULD NOT SET ACHIEVEMENT (cheat) %s, RETURNED FALSE.  Game will continue, but who knows what will be achieved... :(", nombre).c_str()); }
#endif
//#endif
}

int RampGeneration(int generation) {
 int gen = 1;
 switch (generation) {
 case 0:
 case 1:gen =1; break;
 case 2:gen =3; break;
 case 3:gen =4; break;
 case 4:gen =5; break;
 case 5:gen =6; break;
 case 6:gen =6; break;
 case 7:gen =6; break;
 case 8:gen =7; break;
 case 9:gen =7; break;
 case 10:gen=8; break;
 case 11:gen=8; break;
 case 12:gen=9; break;
 case 13:gen=9; break;
 case 14:gen=10; break;
 case 15:gen=10; break;
 case 16:gen = 11; break;
 case 17:gen = 12; break;
 case 18:gen = 12; break;
 case 19:gen = 12; break;
 case 20:gen = 12; break;
 default: int tGen = generation; gen = 13; while (tGen > 20) { gen += RampGeneration(tGen % 20);  tGen -= 10; } break;
 }
 if (game->stats.kills < 200 && gen > 10) gen /= 2;
 return gen;
}

int EnemyFleet::EnemyLimit() {
 return (game->player.dead || alienPickleCheat ? 200 : ((game->bg.generation == 1 && game->stats.survived < 300.0) ? (1) : (URANGE(2, (int)((double)RampGeneration(game->bg.generation)*2.54321) + 1, 200))));
}

EnemyShip *EnemyFleet::Manufacture(int generation, int counter, PlayerShip *ps, int *enemyCounter, bool ignoreLimit) {
 if (!ignoreLimit && enemies.count > EnemyLimit()) return nullptr;
 EnemyShip *enemy = new EnemyShip(UMAX(1, upto(generation + 1)), counter, ignoreLimit);
 enemy->ps = ps;
 if (enemy->fighter) {
  int retries = 0;
  while (retries < 1 || (retries < 50 && enemy->hull.Widest() > 30 && enemy->hull.Tallest() > 30) ) {
   enemy->hull.Clear();
   ShipImageSetNodeHandles parts;
   images.Get(parts, URANGE(1, game ? 1 + game->stats.last.value + 1 : 1 + generation, 20));
   enemy->hull.from(parts, enemies.count + 1);
   retries++;
  }
 } else {
  ShipImageSetNodeHandles parts;
  images.Get(parts, URANGE(1, game ? 1 + game->stats.last.value + 1 : 1 + generation, 20));
  enemy->hull.from(parts, enemies.count + 1);
 }
 enemy->radius = UMAX(10.0, (double)UMIN(enemy->hull.Widest(), enemy->hull.Tallest()) / 2.0);
 if (!ignoreLimit && enemy->radius > 50) enemy->launches = upto(generation);
 if (upto(5) < 3) enemies.AddPrepend(enemy); else enemies.Add(enemy);
 *enemyCounter=(*enemyCounter)+1;
 return enemy;
}

void EnemyShip::Shoot(double dist) {
 if (dist < ps->radius) return;
 EnemyBullet *b = new EnemyBullet(location.x, location.y, angle, 4.0 + generation, 1.0 + generation);
 b->ps = ps;
 if (!allowCheaters) {
  b->image = library.Load("data/images/flares/red.png");
  b->tint.Pick(alabaster);
 } else {
  b->image = library.Load("data/images/flares/red.png");
  b->tint.Pick(hotPink);
 }
 ps->enemies->enemyBullets.Add(b);
 if (dist < 200) shooting.Set(0.1);
 else
 if ( uniform() < UMIN(0.27, 0.57 -(double)generation / 10.0) ) shooting.Set(UMAX(2.0, 10.0 - generation));
 else if (upto(5) < 2 ) shooting.Set(0.15);
 else shooting.Set(0.3);
 shooting.Reset();
}

void EnemyShip::Think() {
 if (wounds.first) {
  if (woundRender) {
   SpinParticle *s = new SpinParticle(woundRender->x, woundRender->y, alabaster);
   s->location.Rotate(angle);
   s->location.x += location.x;
   s->location.y += location.y;
   s->x = (int)s->location.x + (upto(6) - 3);
   s->y = (int)s->location.y + (upto(6) - 3);
   s->image = gas;
   s->duration = 0.1;
   if (upto(20) < 18) {
    s->tint.Brights(generation);
    double sz = UMAX(4.0, radius / 20.0);
    s->size.Set(sz, sz);
    s->speed.Set(1.0, 0.0);
   }
   else {
    double sz = UMAX(4.0, radius / 20.0) * 2.0;
    s->duration *= 5;
    s->size.Set(sz, sz);
    s->speed.Set(2.0, 0.0);
   }
   s->speed.Rotate(angle + 180.0 + (double)(upto(60)) - 30.0);
   ps->fx.Add(s);
   woundRender = (Cartesiand *)wounds.Element(upto(wounds.count));
  }
 }
 Cartesiand line;
 if (ps->dead) {
  line.SetCorners(location.x, location.y, display.wd / 2.0, display.hd / 2.0);
  if (ddistance(line.x, line.y, line.x2, line.y2) > display.wd) {
   angle = rad2deg(line.LineAngle());
   speed.Set(1.0, 0.0);
   speed.Rotate(angle);
  }
  return;
 }
 double toPlayer = ddistance(ps->location.x, ps->location.y, location.x, location.y);
 if (toPlayer > display.wd) {
  line.SetCorners(location.x, location.y,display.wd / 2.0, display.hd / 2.0);
  angle = rad2deg(line.LineAngle());
  speed.Set(10.0, 0.0);
  speed.Rotate(angle);
  return;
 }
 line.SetCorners(location.x, location.y, ps->location.x, ps->location.y);
 desiredAngle = rad2deg(line.LineAngle());
 double oldAngle = angle;
 TOWARDS(angle, desiredAngle, 1.0 + ((double)generation/10.0)); 
 if ( ((game->stats.kills > 20 && toPlayer < 200.0) || NEARBY(oldAngle,angle,10.0)) && toPlayer < display.h/2 ) { if ( !shooting && !ps->dead ) Shoot(toPlayer); }
 speed.Set(URANGE(1.0,1.0*(double)generation,19.0), 0.0);
 speed.Rotate(angle);
 if (upto(10) == 0) {
  if (generation % 5 > 1) {
   Cartesiand diff(radius, 0.0);
   diff.Rotate(180.0);
   SpinParticle *s = new SpinParticle(diff.x+location.x, diff.y+location.y, magenta);
   s->duration = 0.2;
   s->speed.Set(UMAX(0.5, speed.x), 0.0);
   s->speed.Rotate(angle + 180.0);
   ps->fx.Add(s);
  }
  else {
   SpinParticle *s = new SpinParticle(location.x, location.y, magenta);
   s->duration = 0.5;
   s->speed.Set(UMAX(0.5, speed.x), 0.0);
   s->speed.Rotate(angle + 180.0);
   ps->fx.Add(s);
  }
 }
 if (game->enemies.enemies.count < 500 && toPlayer < display.h2d && game->stats.kills > 100 && launches > 0) {
  for (int i = 0; i < launches; i++) {
   EnemyShip *e = game->enemies.Manufacture(generation + 1, 0, ps, &game->stats.enemies.value, true);
   if (!e) continue;
   Cartesiand ofs;
   ofs.Set(30.0, 0.0);
   ofs.Rotate((360.0 / ((double)launches))*(double)(i + 1));
   e->location.Set(location.x+ofs.x, location.y+ofs.y);
   e->angle = angle;
   SpinParticle *s = new SpinParticle(e->location.x, e->location.y, blue255);
   s->duration = 2.0;
   s->size.Set(e->radius*2,e->radius*2);
   s->speed.Set(0.0,0.0);
   s->speed.Rotate(angle + 180.0);
   ps->fx.Add(s);
  }
  launches = 0;
  audio.manager.CueAndPlay("data/sounds/FX/radionoise1.wav", 1.0, 1.0);
 }
}

void EnemyShip::AddToQuadTree() {
 game->qtEnemy.Add(this, &location, radius, &qt);
}

void EnemyShip::RemoveFromQuadTree() {
 game->qtEnemy.Remove(&location, &qt);
 qt.Clear();
}

void EnemyBullet::CheckCollide() {
 if (this->deflected) return;
 if (this->dropMe) return;
 double d = ddistance(ps->location.x, ps->location.y, location.x, location.y);
 if (d < ps->radius) {
  if (game->stats.kills > 150) {
   if (ps->stamina > 0.1) {
    audio.manager.CueAndPlay("data/sounds/Legacy/MetalRicochet.wav", 0.5 + uniform(), 0.25 + uniform()*0.25);
    this->speed.Set(uniform()*20.0 + 5.0, 0.0);
    this->speed.Rotate(uniform()*360.0);
    this->deflected = true;
    ps->deflectionTime = 0.5;
    ps->stamina -= 0.05+0.2*iratiod(100-UMIN(100, RampGeneration(game->bg.generation)), 100);
    if (ps->stamina < 0.0) ps->stamina = 0.0;
    return;
   }
  }
  this->dropMe = true;
  ps->hits++;
  game->stats.damage++;
  if ( ps->hits > UMAX(1, abs(50 - RampGeneration(game->bg.generation))) ) {
   ps->dead = true;
   game->deathReason = allowCheaters ? "CHEATING DEATH" : game->stats.kills > 1000 ? "LEGENDARY DEATH" : game->stats.kills > 100 ? "DIED IN GLORY" : "SHIP DESTROYED";
   if (game->stats.kills > 1000) ACHIEVE("APOLUNE_14");
   if (game->stats.kills > 3000) ACHIEVE("APOLUNE_15");
   if (game->stats.kills > 10000) ACHIEVE("APOLUNE_16");
#if !defined(DEBUG) && defined(USE_STEAMWORKS)
   if (!allowCheaters) {
    if (game->player2.active) {
     steamworks.SetLeaderboard("Apolune 2 Player High Scores", game->finalScore);
     steamworks.SetLeaderboard("Apolune 2 Player Kill Counts", game->stats.kills);
    } else {
     steamworks.SetLeaderboard("Apolune High Scores", game->finalScore);
     steamworks.SetLeaderboard("Apolune Kill Counts", game->stats.kills);
    }
   }
#endif
  }
  else {
   switch (upto(3)) {
   default:
   case 0: audio.manager.CueAndPlay("data/sounds/FX/impact1.wav", 1.0 + uniform()*0.2 *UMAX(0.1, 1.0 - ddistance(fired.x, fired.y, ps->location.x, ps->location.y) / display.wd), 1.0); break;
   case 1: audio.manager.CueAndPlay("data/sounds/FX/impact2.wav", 1.0 + uniform()*0.2 *UMAX(0.1, 1.0 - ddistance(fired.x, fired.y, ps->location.x, ps->location.y) / display.wd), 1.0); break;
   case 2: audio.manager.CueAndPlay("data/sounds/FX/impact3.wav", 1.0 + uniform()*0.2 *UMAX(0.1, 1.0 - ddistance(fired.x, fired.y, ps->location.x, ps->location.y) / display.wd), 1.0); break;
   }
   for (int i = 0; i < 10; i++) ps->fx.Add(new SpinParticle(location.x, location.y, orangeRed));
  }
 }
}

void PlayerBullet::CheckCollide() {
 ShipQuadTreeData *d = game->qtEnemy.Colliding(location);
 if (d) CheckCollide((EnemyShip *)d->parent.pointer);
}

void PlayerBullet::CheckCollide(EnemyShip *e) {
 if (e->dropMe) return;
 double d = ddistance(e->location.x, e->location.y, location.x, location.y);
 bool actuallyHit= d < 10.0;
 if (!actuallyHit && d < e->radius) {
  actuallyHit = number_range(0, 10000) < URANGE(1000,(int)((double)12345 * (double)(game->bg.generation/100.0)),9000);
  /*
  bool flippedX = e->hull.count % 3 == 0;
  bool flippedY = e->hull.count % 4 == 0;
  Cartesiand relative(location.x - e->location.x, location.y - e->location.y);
  relative.Rotate(e->angle);
  int probeOfsX = (int)relative.x;
  int probeOfsY = (int)relative.y;
  ShipImageListItem *i =(ShipImageListItem *)e->hull.first;
  if (i && i->image) {
   GLTexImage tex(i->image->texture);
   Crayon result = tex.Probe(probeOfsX, probeOfsY, flippedX, flippedY);
   if (result.ad * (double) result.Luminance() > 0.2) {
    actuallyHit = true;
   }
  }
  */
 }
 if ( actuallyHit ) {
  this->dropMe = true;
  e->hits++;
  game->stats.hits++;
  if (e->hits > e->generation + (int)e->radius/11) {
   audio.manager.CueAndPlay("data/sounds/Legacy/sfx_boom_echo.wav", 1.0 + uniform()*0.2, 0.55);
   e->dropMe = true;
   e->RemoveFromQuadTree();
   int capacity = (int)UMAX(e->hull.Widest(), e->hull.Tallest());
   for (int i = 0; i < capacity; i++) if ( upto(5) < 3 ) {
    SpinParticle *sp = new SpinParticle(e->location.x + upto(capacity) - capacity / 2, e->location.y + upto(capacity) - capacity / 2, orangeRed);
    if (capacity > 20 && upto(capacity) < capacity / 4) {
     while ( sp->duration > 10.0 ) sp->duration /= 2.0;
     if (sp->duration > 10.0) sp->duration = 10.0;
     int wh = upto(capacity/2) + capacity / 2;
     sp->size.Set(wh / 2, wh / 2);
    }    
    game->player.fx.Add(sp);
   }
   if (e->fighter) game->stats.fighters++;
   if (game->stats.kills > 3000 || (allowCheaters && spazCheat) ) if (game->player.hits > 0) game->player.hits--;
   game->stats.kills++;
   game->stats.last = e->generation;
   game->stats.accumulated += e->generation;
   if (e->generation > game->stats.greatest) game->stats.greatest = e->generation;
   double toPlayer = ddistance(e->ps->location.x, e->ps->location.y, location.x, location.y);
   if (game->enemies.enemies.count < 500 && toPlayer < display.h2d && game->stats.kills > 100 && e->launches > 0) {
    for (int i = 0; i < e->launches; i++) {
     EnemyShip *es = game->enemies.Manufacture(e->generation + 1, 0, e->ps, &game->stats.enemies.value, true);
     if (!es) continue;
     Cartesiand ofs;
     ofs.Set(30.0, 0.0);
     ofs.Rotate((360.0 / ((double)e->launches))*(double)(i + 1));
     es->location.Set(location.x + ofs.x, location.y + ofs.y);
     es->angle = e->angle;
     es->hits = number_range(1,es->generation);
     // Initial wound damage due to exploding ship.
     for (int i = 0; i < es->hits; i++) {
      Cartesiand *wound = new Cartesiand;
      wound->Set(es->radius*(uniform()*0.8 + 0.2), 0.0);
      wound->Rotate(uniform()*360.0);
      es->wounds.Append(es->woundRender=wound);
     }
     SpinParticle *s = new SpinParticle(es->location.x, es->location.y, blue255);
     s->duration = 2.0;
     s->size.Set(e->radius * 2, e->radius * 2);
     s->speed.Set(0.0, 0.0);
     s->speed.Rotate(e->angle + 180.0);
     e->ps->fx.Add(s);
    }
    e->launches = 0;
    audio.manager.CueAndPlay("data/sounds/FX/radionoise1.wav", 1.0, 1.0);
   }
  }
  else {
   switch (upto(3)) {
   default:
   case 0: audio.manager.CueAndPlay("data/sounds/FX/impact1.wav", 1.0 + uniform()*0.2*UMAX(0.1, 1.0 - ddistance(location.x, location.y, game->player.location.x, game->player.location.y) / display.wd), 1.0); break;
   case 1: audio.manager.CueAndPlay("data/sounds/FX/impact2.wav", 1.0 + uniform()*0.2*UMAX(0.1, 1.0 - ddistance(location.x, location.y, game->player.location.x, game->player.location.y) / display.wd), 1.0); break;
   case 2: audio.manager.CueAndPlay("data/sounds/FX/impact3.wav", 1.0 + uniform()*0.2*UMAX(0.1, 1.0 - ddistance(location.x, location.y, game->player.location.x, game->player.location.y) / display.wd), 1.0); break;
   }
   for (int i = 0; i < 10; i++) game->player.fx.Add(new SpinParticle(location.x, location.y, green255));
   e->wounds.Append(e->woundRender = new Cartesiand(location.x-e->location.x, location.y-e->location.y));
   e->woundRender->Rotate(-e->angle);
  }
 }
}

void PlayerShip::ThrustDust( double moveAngle ) {
 if (game->bg.generation < 5) {
  if (upto(3) == 0) {
   SpinParticle *s = new SpinParticle(location.x, location.y, skyBlue);
   s->speed.Set(0.5, 0.0);
   s->speed.Rotate(moveAngle + 180.0 + (double)(upto(40)) - 20.0);
   s->duration *= 2.0;
   fx.Add(s);
  }
 } else if (upto(3) == 0) fx.Add(new SpinParticle(location.x, location.y, skyBlue));
}

double PlayerShip::Generation() {
 return UMAX(0.25,(double)(game->bg.generation) / 5.0);
}

bool PlayerShip::CanDodge() {
#if defined(DEBUG)
 return true;
#else
 return game->stats.kills > 150;
#endif
}

void PlayerShip::Dodge(int key) {
 double staminaRequired = 0.1 + 0.6 * (1.0 - iratiod(UMIN(100, game->bg.generation), 100));
 if (stamina < staminaRequired) { audio.manager.CueAndPlay("data/sounds/UI/locked.wav", 1.0, 1.0); return; }
 stamina -= staminaRequired;
 Cartesiand jump;
 double jumpDistance = (radius + UMIN(display.h2d, 2.0 * radius * ((double)game->bg.generation / 5.0)) );
 jump.Set(jumpDistance, 0.0);
 switch (key) {
 default: return; break;
  case DX_W: if ( game->player2.active ) game->player2.Force(DX_G,100.0); jump.Rotate(angle);  break;
  case DX_A: if ( game->player2.active ) game->player2.Force(DX_H,100.0); jump.Rotate(angle - 90.0);  break;
  case DX_S: if ( game->player2.active ) game->player2.Force(DX_F,100.0); jump.Rotate(angle +180.0);  break;
  case DX_D: if ( game->player2.active ) game->player2.Force(DX_T,100.0); jump.Rotate(angle + 90.0);  break;
 }
 Line line(location.x, location.y, location.x+jump.x, location.y+jump.y);
 location.Add(jump.x, jump.y, jump.z);
 Vertexd out;
 int d = (int)jumpDistance;
 for (int i = 0; i < d; i += 4 ) {
  double t = (double)i / jumpDistance;
  line.Point((double)t, &out);
  if (game->bg.generation > 7) {
   PlayerBullet *pb = new PlayerBullet(out.x, out.y, 0.0, 0.0, 2.0*(double)game->bg.generation);
   pb->duration = 0.2;
   pb->tint.Pick(clear);
   game->friendlyBullets.Add(pb);
  }
  SpinParticle *s = new SpinParticle(out.x, out.y, alabaster);
  s->tint.Double(t, t, 1.0, 1.0);
  s->duration += t * 2.0;
  s->speed.Set(0.0, 0.0);
  s->size.Set(radius,radius);
  fx.Add(s);
 }
 audio.manager.CueAndPlay("data/sounds/UI/levelup.wav",1.0,1.0);
}

void PlayerShip::RebuildStamina() {
 if ( CanDodge() ) if ( stamina < 1.0 ) stamina = stamina + 1.0/1200.0 + (((double)game->bg.generation * 0.01)) * (FRAMETIMEd);
}

bool Player2::AllowForce(double dx, double dy) {
 Cartesiand line2;
 line2.SetCorners(game->player.location.x, game->player.location.y, game->player2.location.x, game->player2.location.y);
 double tetherlength = ddistance(line2.x, line2.y, line2.x2, line2.y2);
 if (tetherlength > 300.0) return false;
 if (tetherlength < 200.0) return true;
 Cartesiand line1(dx, dy);
 double angleA = rad2deg(line1.LineAngle());
 double angleB = rad2deg(line2.LineAngle());
 if (!NEARBY(angleA, angleB, 45.0)) return true;
 return false;
}

void Player2::CheckForActivation() {
 if (active) return;
 if (game->bg.generation > 1) return;
 if (game->stats.kills > 30) return;
 if (input.KeyHit(DX_Y) || input.KeyHit(DX_U)) {
  active = true;
  audio.manager.CueAndPlay("data/sounds/Legacy/yo.wav", 1.0, 1.0);
  recentlyActivated = true;
 }
}

void Player2::DrawRelative() {
 b2Body *d = nullptr;
 Blending(none);
 Cartesiand line;  
 b2Body *bodies = simulated->m_world->GetBodyList();
 b2Vec2 hp = p.head->GetPosition();
 for (b2Body *c = bodies; c; c = c->GetNext()) {
  if (!c->GetNext()) break;
  if (c == head) continue;
//  if (c == tail) continue;
  if (!d) { d = c; continue; }
  b2Vec2 C = c->GetPosition() - hp;
  b2Vec2 D = d->GetPosition() - hp;
  line.SetCorners(D.x*10.0, D.y * 10, C.x*10.0, C.y*10.0);
  double linearangle = rad2deg(line.LineAngle());
  line.MoveBy(game->player.location.x, game->player.location.y);
  double len = ddistance(line.x, line.y, line.x2, line.y2);
  if (len == 0.0) continue;
  Cartesiand center((line.x2 + line.x) / 2.0, (line.y2 + line.y) / 2.0);
  art.Pivot(hoseSegment,crayons.Pick(alabaster),transparency,center.x,center.y,0.0,0.0,1.0,len/2.0, linearangle);
  d = c;
 }
 b2Vec2 position = p.tail->GetPosition() - hp;
 location.Set(game->player.location.x + (double)(position.x)*10.0, game->player.location.y + (double)(position.y)*10.0);
 art.Pivot(image, crayons.Pick(alabaster), transparency,
  location.x, 
  location.y, 0.0, 0.0,
  (double)(image->width / 2), (double)(image->height / 2), angle
 );
}

void Game::RestartGame() {
 ACHIEVE("APOLUNE_17");
 windows.DestroyAll();
 windows.Add(new Game);
}

#include "GameMenu.h"
void Game::ReturnToGameMenu() {
 input->hideCursor = false;
 windows.DestroyAll();
 windows.Add(new GameMenu);
}