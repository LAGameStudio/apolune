#pragma once
#include "LinkedList.h"
#include "Crayon.h"
#include "TextureLibrary.h"
#include "Cartesian.h"

#define STARS 200
#define EARTH_SYSTEM 0

extern int planetsCreated;
extern int moonsCreated;
extern int starsCreated;
extern int systemsCreated;

extern int planet_catalog_counter;

extern int planetSeed;
extern int moonSeed;

const char *MKK_name_from_char( char c );

/*
# 5 Intrinsic variable stars

    * 5.1 Pulsating variable stars
          o 5.1.1 Cepheids and cepheid-like variables
                + 5.1.1.1 Delta Cepheid variables
                + 5.1.1.2 W Virginis variables
                + 5.1.1.3 RR Lyrae variables
                + 5.1.1.4 Delta Scuti variables
                + 5.1.1.5 SX Phoenicis variables
          o 5.1.2 Bluewhite variables with early spectra (O and B)
                + 5.1.2.1 Beta Cephei variables
                + 5.1.2.2 PV Telescopii variables
          o 5.1.3 Long Period and Semiregular variables
                + 5.1.3.1 Mira variables
                + 5.1.3.2 Semiregular variables
                + 5.1.3.3 Slow irregular variables
          o 5.1.4 RV Tauri variables
          o 5.1.5 Alpha Cygni variables
          o 5.1.6 Pulsating white dwarfs
          o 5.1.7 Solar-like oscillations
    * 5.2 Eruptive variable stars
          o 5.2.1 Protostars
                + 5.2.1.1 Herbig Ae/Be stars
                + 5.2.1.2 Orion variables
                + 5.2.1.3 FU Orionis variables
          o 5.2.2 Main Sequence variables
                + 5.2.2.1 Wolf-Rayet variables
                + 5.2.2.2 Flare stars
          o 5.2.3 Giants and supergiants
                + 5.2.3.1 Luminous blue variables
                + 5.2.3.2 Gamma Cassiopeiae variables
                + 5.2.3.3 R Coronae Borealis variables
          o 5.2.4 Eruptive binary stars
                + 5.2.4.1 RS Canum Venaticorum variables
    * 5.3 Cataclysmic or explosive variable stars
          o 5.3.1 Supernovae
          o 5.3.2 Novae
          o 5.3.3 Dwarf novae
          o 5.3.4 Z Andromedae variables

# 6 Extrinsic variable stars

    * 6.1 Rotating variable stars
          o 6.1.1 Non-spherical stars
                + 6.1.1.1 Ellipsoidal variables
          o 6.1.2 Stellar spots
                + 6.1.2.1 FK Comae Berenices variables
                + 6.1.2.2 BY Draconis variable stars
          o 6.1.3 Magnetic fields
                + 6.1.3.1 Alpha-2 Canum Venaticorum variables
                + 6.1.3.2 SX Arietis variables
                + 6.1.3.3 Optically variable pulsars
    * 6.2 Eclipsing binaries
          o 6.2.1 Algol variables
          o 6.2.2 Beta Lyrae variables
          o 6.2.3 W Ursae Majoris variables
    * 6.3 Planetary transits
*/

enum AnomalyTypes {
 molecularCloud=0,
 protostar=1,
 HerbigAeBe=2, // <2 solar masses, AB, infrared
 mainsequence=3,
 mainsequence3x=4,
 mainsequence10x=5,
 mainsequence50x=6,
 redgiant=7,
 carbonstar=8,
 whitedwarf=9, // contents: electron-degenerate matter
 blackdwarf=10,
 supernova=11,
 variablestar=12,
 neutronstar=13, // surviving core 1.5-3x masses
 pulsar=14, // highly magnetic neutron star with pulsed gamma radiation
 blackhole=15, // surviving core greater than 3x
 whitehole=16, // 
 MTwormholeLTW=17, // traversible wormhole
 romanRing=18, // time police"Chronology Protection Agency"
 wolf_rayet=19,
 gammacass=20,
 rscanumvena=21,
 fuorionis=22,
 zandromedae=23,
 ellipsoidal=24,
 fkcomae=25,
 algol=26,
 opticallyvariable=27,
 sxarieis=28,
 wursae=29,
 luminousblue=30,
 rcoronae=31,
 wvirginis=32,
 mira=33,
 deltascuti=34,
 rrlyrae=35,
 sxphoenicis=36,
 slowirregular=37,
 rvtauri=38,
 bydraconis=39,
 flarestar=40,
 betacephae=41,
 pulsatingwhitedwarf=42,
 pvtelescopii=43,
 ttauri=44, // x-ray photosphere < 2 solar masses, FGKM (analog of HerbigAeBe)
 ldwarf=45,
 latemdwarf=46,
 ydwarf=47,
 luminousred=48,
 novae=49,
 reddwarf=50
};

enum NebulaTypes {
 pulsarwindnebula=0, // around pulsar
 hIIregion=1, // young, hot blue stars emitting ionizing ultraviolet
 hIregion=2, // neutral atomic hydrogen cloud (non-luminous)
 magneticField=3,
 darknebula=4,
 reflection=5, // no star present
 emission=6, // cloud of ionized gas emitting light (supernova remnant or planetary nebula)
 protoplanetary=7,
 planetarynebula=8,
 supernovaremnant=9, // synchotron emission and X-ray (oscillating electrons in magnetic fields)
 herbigharo=10, // rises from poles perpendicular to accretion disc; around young stars occluding them
 wrcloud=11,
 oort=12,
 circumstellardisk=13
};

enum PlanetTypes {
 dwarf_planet=0,
 terrestrial=1,
 gas_giant=2
};

enum SolSystem {
 NotSol=-1,
 Mercury=0,
 Venus=1,
 Earth=2,
 Mars=3,
 Saturn=4,
 Jupiter=5,
 Uranus=6,
 Neptune=7,
 Pluto=8,
 Eris=9,

 // Earth
 Luna=10,
 // Mars
 Phobos=11,
 Diemos=12,
 // Jupiter
 Io=13,
 Europa=14,
 Ganymede=15,
 Callisto=16,
 // Saturn
 Mimas=17,
 Enceladus=18,
 Tethys=19,
 Dione=20,
 Rhea=21,
 Titan=22,
 Hyperion=23, // it's a troid! wow.
 Iapetus=24,
 Phoebe=25, // troid
 // Uranus
 Puck=26,  // troid
 Miranda=27,
 Ariel=28,
 Umbriel=29,
 Titania=30,
 Oberon=31,
 // Neptune
 Proteus=32,
 Triton=33,
 Nereid=34, // another troid!
 // Pluto
 Charon=35,
 // Eris
 Dysnomia=36
};

enum NeighborhoodTypes {
 supermassiveblackhole=0,
 quasar=1, // emits entire range of EM, 
 blazar=2
};

/*
Table of main sequence stellar parameters[22] Stellar
Class 	Radius 	Mass 	Luminosity 	Temperature 	Examples[23]
R/R 	M/M 	L/L 	K
O5 	18 	40 	500,000 	38,000 	Zeta Puppis
B0 	7.4 	18 	20,000 	30,000 	Phi1 Orionis
B5 	3.8 	6.5 	800 	16,400 	Pi Andromedae A
A0 	2.5 	3.2 	80 	10,800 	Alpha Coronae Borealis A
A5 	1.7 	2.1 	20 	8,620 	Beta Pictoris
F0 	1.4 	1.7 	6 	7,240 	Gamma Virginis
F5 	1.2 	1.29 	2.5 	6,540 	Eta Arietis
G0 	1.05 	1.10 	1.26 	6,000 	Beta Comae Berenices
G2 	 1.00[note 2] 	 1.00[note 2] 	 1.00[note 2] 	5,920 	Sun
G5 	0.93 	0.93 	0.79 	5,610 	Alpha Mensae
K0 	0.85 	0.78 	0.40 	5,150 	70 Ophiuchi A
K5 	0.74 	0.69 	0.16 	4,640[24] 	61 Cygni A
M0 	0.63 	0.47 	0.063 	3,920 	Gliese 185[25]
M5 	0.32 	0.21 	0.0079 	3,120 	EZ Aquarii A
M8 	0.13 	0.10 	0.0008 	— 	Van Biesbroeck's star[26]
 */


ONE(Star,{ 
 type=molecularCloud;
 nebula=oort; // most common, starts as herbig haro
 star_catalog_no=starsCreated++;
 MKspectral='M';
 corona.Pick(red255);
 luminosity=1.0f;
 radius=1.0f;
 mass=1.0f;
 temperature=1000.0f;
 period=period2=period3=1.0f;
})
 AnomalyTypes type;
 NebulaTypes nebula;
 Cartesiand position;
 Zint star_catalog_no;
 char MKspectral;
// int MKsubspectral;
 Zfloat wind; // t-tauri has high wind
 Zfloat luminosity;
 Zfloat mass, radius, period, period2, period3;
 Zfloat temperature;
 Crayon photosphere,corona;
// RadTypes radiationType;
 KEYWORDS({
  TXTWORD("name",name)
  else KEYWORD("type",{type=(AnomalyTypes)atoi(w);})
  else KEYWORD("nebula",{nebula=(NebulaTypes)atoi(w);})
  else KEYWORD("position",{position.fromString(w);})
  else KEYWORD("star_catalog_no",{star_catalog_no=w;})
  else KEYWORD("MK",{MKspectral=(*w);})
  else KEYWORD("wind",{wind=w;})
  else KEYWORD("luminosity",{luminosity=w;})
  else KEYWORD("mass",{mass=w;})
  else KEYWORD("radius",{radius=w;})
  else KEYWORD("period",{period=w;})
  else KEYWORD("period2",{period2=w;})
  else KEYWORD("period3",{period3=w;})
  else KEYWORD("temperature",{temperature=w;})
  else KEYWORD("photosphere",{photosphere.CSS(w);})
  else KEYWORD("corona",{corona.CSS(w);})
 })
 WORDKEYS({
  TXTKEY("name",name)
  WORDKEY("type",(int)type)
  WORDKEY("nebula",(int)nebula)
  SSUBKEY("position",position)
  WORDKEY("star_catalog_no",star_catalog_no)
  WORDKEY("MK",MKspectral)
  WORDKEY("wind",wind)
  WORDKEY("luminosity",luminosity)
  WORDKEY("radius",radius)
  WORDKEY("period",period)
  WORDKEY("period2",period2)
  WORDKEY("period3",period3)
  WORDKEY("temperature",temperature)
  WORDKEY("photosphere",photosphere.toCSS())
  WORDKEY("corona",corona.toCSS())
 })
 void Nova( int seed );
MANY(Star,StarHandle,StarHandles,"Star",Stars,{})
 KEYWORDSGroup("Star",Star);
 WORDKEYSGroup("Star",Star);
DONE(Star);

class Planets;
ONE(Planet,{
 planet_catalog_no=planet_catalog_counter++;
// Compound atmos,lithos,core,ocean;
 type=dwarf_planet;
 isMoon=false;
 coelescing=NotSol;
})
 SolSystem coelescing;
 Zint planet_catalog_no;
 Zbool isMoon;
 Cartesiand rotation_angle, rotation_speed;
 Zfloat mass, density, biosphere, gravity, temperature, orbital_range, radius;
// Compound atmos,lithos,core,ocean;
 Zfloat sealevel;
 Zfloat magnetism;
 Cartesiand position,rotation;
 PlanetTypes type;
 Zdis<Planets> moons;
 Zint craters;
 Zint seed,num_rings;
 void Coelesce(SolSystem coelescing);
 void Coelesce(int seed);
 void Moonify( Planet *p, int seed );
 bool isJovian() {
  return ( type == gas_giant );
 }
 void Rename() {
  char letters[14];
  FORMAT(letters,14,"ABCDGHKLQRUXYZ");
  char buf[64];
  FORMAT(buf,64,"Planet #%d-%c",
   (int) planet_catalog_no, letters[planet_catalog_no%13]
  );
  name=string(buf);
 }
 KEYWORDS({
  TXTWORD("name",name)
  else KEYWORD("coel",{coelescing=(SolSystem)atoi(w);})
  else KEYWORD("type",{type=(PlanetTypes)atoi(w);})
  else KEYWORD("planet_catalog_no",{planet_catalog_no=w;})
  else KEYTAG("moon",{isMoon=true;})
  else KEYWORD("rot_angle",{rotation_angle.fromString(w);})
  else KEYWORD("rot_speed",{rotation_speed.fromString(w);})
  else KEYWORD("position",{position.fromString(w);})
  else KEYWORD("rotation",{rotation.fromString(w);})
  else KEYWORD("mass",{mass=w;})
  else KEYWORD("density",{density=w;})
  else KEYWORD("biosphere",{biosphere=w;})
  else KEYWORD("gravity",{gravity=w;})
  else KEYWORD("temperature",{temperature=w;})
  else KEYWORD("orbital_range",{orbital_range=w;})
  else KEYWORD("radius",{radius=w;})
  else KEYWORD("sealevel",{sealevel=w;})
  else KEYWORD("magnetism",{magnetism=w;})
  else KEYWORD("craters",{craters=w;})
  else KEYWORD("seed",{seed=w;})
  else KEYWORD("num_rings",{num_rings=w;})
  else KEYWORD("moons",{LoadMoons(w);})
 })
 void LoadMoons(const char *in);
 WORDKEYS({
  TXTKEY("name",name)
  WORDKEY("type",(int)type)
  WORDKEY("coel",(int)coelescing)
  WORDKEY("planet_catalog_no",planet_catalog_no)
  TAGKEY("moon",isMoon)
  SSUBKEY("rot_angle",rotation_angle)
  SSUBKEY("rot_speed",rotation_speed)
  SSUBKEY("position",position)
  SSUBKEY("rotation",rotation)
  WORDKEY("mass",mass)
  WORDKEY("density",density)
  WORDKEY("biosphere",biosphere)
  WORDKEY("gravity",gravity)
  WORDKEY("temperature",temperature)
  WORDKEY("orbital_range",orbital_range)
  WORDKEY("radius",radius)
  WORDKEY("sealevel",sealevel)
  WORDKEY("magnetism",magnetism)
  WORDKEY("craters",craters)
  WORDKEY("seed",seed)
  WORDKEY("num_rings",num_rings)
  if ( moons ) WORDKEY("moons",SaveMoons());
 })
 string SaveMoons();
MANY(Planet,PlanetHandle,PlanetHandles,"Planet",Planets,{})
 Planet *planetByNumber( int i ) {
  FOREACH(Planet,p) if ( p->planet_catalog_no == i ) return p;
  return null;
 }
 KEYWORDSGroup("Planet",Planet);
 WORDKEYSGroup("Planet",Planet);
 SORTING(Planet,Sort,{},{ if ( A->orbital_range < B->orbital_range ) result=-1; },{})
DONE(Planet);

ONE(SolarSystem,{
 name=string("");
 system_catalog_no=systemsCreated++;
 nebula=oort;
 nebulaImage=null;
})
 NebulaTypes nebula;
 Zp<GLImage> nebulaImage;
//Chemicals residue;
 Stars stars;
 Planets planets;
 Crayon tint;
// Zfloat terminationShock; // diameter of the internal equilibrium created by interstellar wind, calculated from Stars
// Vertex heliosphere; // dimensions of the heliosphere determining the range of the heliosheath (where wind stops and friction increases) and the heliopause (edge of interstellar space, bow shock); expressed in light years
// Vertex heliocoord;  // center of the star system is
 Cartesiand position;
 Zint system_catalog_no;
 KEYWORDS({
  TXTWORD("name",name)
  else KEYWORD("nebula",{nebula=(NebulaTypes)atoi(w);})
  else KEYWORD("system_catalog_no",{system_catalog_no=w;})
  else KEYWORD("image",{nebulaImage=library.Load(w);})
  else KEYWORD("stars",{stars.fromString(w);})
  else KEYWORD("planets",{planets.fromString(w);})
 })
 WORDKEYS({
  TXTKEY("name",name)
  WORDKEY("nebula",(int)nebula)
  WORDKEY("system_catalog_no",system_catalog_no)
  if ( nebulaImage ) WORDKEY("image",nebulaImage->filename);
  SSUBKEY("stars",stars)
  SSUBKEY("planets",planets)
 })
 void Generate( int seed ) {
  if ( system_catalog_no == EARTH_SYSTEM ) {
   position.Set(0.55,0.55,0.55);

   // Generate sol system
   Star *s=new Star;
   s->Nova(0);
   s->position.Set(0,0,0);
   stars.Append(s);
   
   Planet *p=null;
   p=new Planet; p->Coelesce(Mercury); planets.Append(p);
   p=new Planet; p->Coelesce(Venus);   planets.Append(p);
   p=new Planet; p->Coelesce(Earth);   planets.Append(p);
   p=new Planet; p->Coelesce(Mars);    planets.Append(p);
   p=new Planet; p->Coelesce(Saturn);  planets.Append(p);
   p=new Planet; p->Coelesce(Jupiter); planets.Append(p);
   p=new Planet; p->Coelesce(Uranus);  planets.Append(p);
   p=new Planet; p->Coelesce(Neptune); planets.Append(p);
   p=new Planet; p->Coelesce(Pluto);   planets.Append(p);
   p=new Planet; p->Coelesce(Eris);    planets.Append(p);
   return;
  }

  Random *set=randoms.GetSet(seed);
  float perc1=randomify2(seed,STARS);
  float perc2=1/perc1;
  float perc3=(float) sin((float) seed);
  int num_stars=seed/2 % 3 ==2 ? seed/2 %4 : 1;
  int num_planets=seed != 0 ? randomify( 1, seed % (int) ( 1.0f+10.0f*(perc3<=0.1f ? 1.0f:perc3) ), seed ) : 0;
  int num_troids =CLAMP(seed != 0 ? randomify( 0, seed % 100, seed ) : 1,100);
  int i;

  if ( num_stars == 0 ) {
   switch ( seed % 10 ) {
   default: nebula=supernovaremnant; break;
    case 1: nebula=emission; break;
    case 2: nebula=reflection; break;
    case 3: nebula=darknebula; break;
    case 4: {
             Star *s=new Star;
             s->type=blackdwarf;
             s->nebula=supernovaremnant;
             s->corona.Int(64,64,64,255);
             s->photosphere.Int(64,64,64,255);
             s->mass=0.5f+1.0f*perc3;
             stars.Append(s);
            }
       break;
   }
  }
  
  position.Set(
   xyzs[(seed*3)%60000],
   xyzs[(seed+1)%60000],
   xyzs[(seed+2)%60000]
  );
  for ( i = 0; i<num_stars; i++ ) {
   Star *s=new Star;
   s->Nova(seed+i);

   if ( i==0 ) s->position.Set(0,0,0);
   stars.Append(s);
  }
  EACH(stars.first,Star,s) if ( s->nebula != oort ) nebula=s->nebula;
  for ( i = 0; i<num_planets; i++ ) {
   Planet *p=new Planet;
   p->Coelesce((seed+planetSeed)*100+i);
   planets.Append(p);
  }
  i=1;
  EACH(planets.first,Planet,p) {
   p->rotation_angle.x=360.0f*set->unique(i,seed,planetSeed);//+(float)i*30.0f;
   bool found=true;
   int checks=0;
   while ( found ) {
    found=false;
    EACH(planets.first,Planet,q) {
     if ( abs(p->rotation_angle.x-q->rotation_angle.x) < 5.0f ) { found=true; break; }
    }
    if ( found ) {
     planetSeed++;
     p->rotation_angle.x=360.0f*set->unique(i,seed,planetSeed);//+(float)i*30.0f;
     checks++;
    }
    if ( checks > 20 ) { found=false; break; }
   }
   i++;
  }
  planets.Sort();
  float w=mapRangef((xyzs[seed*3%44322]+xyzs[seed*5%22222])/2.0f,0.0f,1.0f, 0.2f,1.0f);
  tint.Double(position.x,position.y,position.z,1.0);
 }
 void Rename() {
  char letters[14];
  FORMAT(letters,14,"ABCDGHKLQRUXYZ");
  char buf[64];
  FORMAT(buf,64,"System #%d-%c", system_catalog_no, letters[system_catalog_no%13]);
  name=string(buf);
 }
MANY(SolarSystem,SolarSystemHandle,SolarSystemHandles,"SolarSystem",SolarSystems,{})
 SolarSystem *systemByNumber( int i ) {
  FOREACH(SolarSystem,ss) if ( ss->system_catalog_no == i ) return ss;
  return null;
 }
 void findNearby( SolarSystemHandles *out, SolarSystem *s, double d ) {
  FOREACH(SolarSystem,system) if ( system != s ) {
   double D=ddistance(system->position.x,system->position.y,s->position.x,s->position.y);
   if ( D<d ) out->Add(system);
  }
 }
 KEYWORDSGroup("SolarSystem",SolarSystem);
 WORDKEYSGroup("SolarSystem",SolarSystem);
DONE(SolarSystem);

class Universe {
public:
 Universe() { reseed(); }
 ~Universe() { }
 SolarSystems systems;
 Zbool apple_pie_from_scratch_ok;
 KEYWORDS({
 })
 WORDKEYS({
 })
 SolarSystem *systemByNumber( int i ) {
  return systems.systemByNumber(i);
 }
 void BigBang() {
  for ( int j=0; j<STARS; j++ ) {
   SolarSystem *system=new SolarSystem;
   system->Generate(j);
   systems.Append(system);
  }
  Rename();
 }

 // Resets all the names of the planets in the Universe.
 void Rename() {
  char buf[64];
  EACH(systems.first,SolarSystem,r) if ( r->system_catalog_no != EARTH_SYSTEM ) {
   r->Rename();
   EACH(r->planets.first,Planet,s) {
    int i=1;
    s->Rename();
    if ( s->moons ) EACH(s->moons->first,Planet,t) {
     string number=RomanNumerals(i);
     FORMAT(buf,64,"Moon %s of %s",number.c_str(),s->name.c_str());
     t->name=string(buf);
     i++;
    }
   }
  }
 }
};

extern Universe universe;

char *system_catalog_name( SolarSystem *s );
SolarSystem *find_system_by_number( int no );
Planet *find_planet_by_number( int no );
Planet *find_moon_by_number( int no );
