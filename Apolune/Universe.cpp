#include "Universe.h"

Universe universe;

int planetsCreated=0;
int moonsCreated=0;
int starsCreated=0;
int systemsCreated=0;

int planet_catalog_counter=0;

int planetSeed=1;
int moonSeed=1;

char *system_catalog_name( SolarSystem *s ) {
 static char buf[1024];
 int no=s->system_catalog_no;
 if ( no==EARTH_SYSTEM ) FORMAT( buf,1024, "Sol" );
 else FORMAT( buf,1024, "System %s-%d%s", 
  ( no % 3 == 0 ? "G" : ( no % 2 == 1 ? "X" : "G" ) ), 
  (int) no,
  ( no % 4 == 1 ? "A" : ( no % 3 == 1 ? "B" : ( no % 2 == 1 ? "C" : "D" ) ) )
 );
 return buf;
}

const char *MKK_name_from_char( char c ) {
 switch ( c ) {
  case 'O': return "O-type Main Sequence";
  case 'B': return "B-type Main Sequence";
  case 'A': return "A-type Main Sequence";
  case 'F': return "F-type Main Sequence";
  case 'G': return "G-type Main Sequence";
  case 'K': return "K-type Main Sequence";
  case 'M': return "M-type Main Sequence";
  case 'W': return "Wolf-Rayet";
  case 'L': return "L-dwarf";
  case 'T': return "Methane Dwarf";
  case 'Y': return "Sub-brown Dwarf";
  case 'C': return "Carbon Star";
  case 'S': return "Zirconium Oxide S-type";
  case 'D': return "White Dwarf";
  case 'P': return "Planetary Nebulae";
  case 'Q': return "Novae";
  case 'R': return "Luminous Red Novae";
  case 'H': return "Hypernova";
   default: return "Unknown";
 }
}

SolarSystem *find_system_by_number( int no ) {
 return universe.systems.systemByNumber(no);
}

Planet *find_planet_by_number( int no ) {
 EACH(universe.systems.first,SolarSystem,ss) {
  EACH(ss->planets.first,Planet,p) {
   if ( p->planet_catalog_no == no ) return p;
   if ( p->moons ) EACH(p->moons->first,Planet,m) {
    if ( m->planet_catalog_no == no ) return m;
   }
  }
 }
 return null;
}

Planet *find_moon_by_number( int no ) {
 return find_planet_by_number(no);
}


void Star::Nova( int seed ) {
 float perc1=(float) abs(cos((float) seed)+sin((float) seed))/2;
 float perc2=(float) abs(cos((float) seed));
 float perc3=(float) abs(sin((float) seed));
 int num_stars=seed/2 % 3;
 int num_planets;
 int num_troids ;
 reseed();
 if ( seed > 0 ) {
  num_planets=randomify( 1, seed % (int) ( 10*perc3+1 ), seed );
  num_troids =randomify( 0, seed % (int) ( 100*perc1+1 ), seed );
 }
 position.Set(1.0-perc1*2.0,1.0-perc2*2.0f,0.0);
//  if ( num_planets == 0 ) num_planets+=1;
  // 70% are red dwarf
  // generally have local similarity
  // most common main sequence (3,000-30,000 Degrees K) Luminosity (1/100 - 11,000) 
  // medium, common red giants (2,500-6,000 Degrees K) Luminosity (10-13,000)
  // least common white dwarves (10,000-17,000 Degrees K) Luminosity (9,500-17,000)
  // protostar temp: 15M degrees C
 if ( perc1 < 0.5f ) {
  if ( perc1 < 0.3f ) {
   MKspectral = seed % 2 == 1 ? 'M' : 'K';
   if ( MKspectral == 'K' ) {
    corona.Int( 255, (int) ((perc3*255.0f)/2.0f), (int) ((perc3*255.0f)/4.0f), 196 );
    photosphere.Int(  255, (int) ((perc3*255.0f)/2.0f), (int) ((perc1*255.0f)/4.0f), 196 );
    mass=0.45f + (0.35f * perc1);
    radius=0.7f;// + (0.26f * perc1);
    luminosity=0.1f + (0.5f * perc1);
    type= (seed % 5 == 1 ? carbonstar : redgiant);
    if ( type == carbonstar ) { photosphere.Pick(yellow); corona.Redf(0.0f); corona.Greenf(1.0f); }
    if ( type == redgiant ) { radius*=3.0f; corona.Redf(1.0f); corona.Bluef(0.25f); corona.Greenf(0.0f); }
   } else
   if ( MKspectral == 'M' ) {
    type= (seed % 5 == 1 ? carbonstar :
          (seed % 4 == 1 ? latemdwarf :
          (seed % 3 == 1 ? rvtauri : redgiant ) ) );
    if ( type == latemdwarf )  corona.Int( 255, 255, 255, 196 );
    else  corona.Int( 255, (int) ((perc3*255.0f)/4.0f), (int) ((perc3*255.0f)/4.0f), 196 );
    photosphere.Int( 255, (int) ((perc3*255.0f)/4.0f), (int) ((perc3*255.0f)/4.0f), 196 );
    if ( type == carbonstar ) { photosphere.Pick(orange); corona.Redf(0.0f); corona.Greenf(1.0f); }
    mass=0.2f + (0.45f * perc1);
    radius=0.5f + (0.7f * perc1);
    luminosity=0.1f + (0.1f * perc1);
    if ( type == redgiant ) { radius*=3.0f; corona.Redf(1.0f); corona.Bluef(0.25f); corona.Greenf(0.0f); }
    if ( type == rvtauri ) nebula=protoplanetary;
   }
   wind=0.8f;
  } else {
   int t=seed%7;
   switch ( t ) {
    case 0: MKspectral = 'T'; // methane dwarf a.k.a. brown dwarf
     photosphere.Int( 127, 0, 127, 196 );
     corona.Int( 255, (int) (perc3*255) /5, (int)(perc3*255) / 2, 196 );
     mass=0.1f + (0.5f * perc1);
     radius=0.1f + (0.3f * perc1);
     luminosity=0.1f + (0.5f * perc1);
     type=ldwarf;
    break;
    case 1: MKspectral = 'S'; // zo s-type
     photosphere.Int( 0, 160, 0, 196 );
     corona.Int( 255, (int) (perc3*255) /5, (int)(perc3*255) / 2, 196 );
     mass=0.1f + (0.5f * perc1);
     radius=0.1f + (0.3f * perc1);
     luminosity=0.1f + (0.5f * perc1);
     type=carbonstar; // sorta
//      ( type == carbonstar ) corona.Green(255);
    break;
    case 2: MKspectral = 'Y'; // Y-dwarf
     photosphere.Int( 100, 50, 20, 196 );
     corona.Int( 120, (int) (perc3*70) /5, (int)(perc3*255) / 2, 196 );
     mass=0.1f + (0.5f * perc1);
     radius=0.1f + (0.3f * perc1);
     luminosity=0.1f + (0.5f * perc1);
     type=ydwarf;
    break;
    case 3: MKspectral = 'L'; // L-dwarf
     photosphere.Int( 255, 32, 64, 196 );
     corona.Int( 255, (int) (perc3*255) /5, (int)(perc3*255) / 2, 196 );
     mass=0.1f + (0.5f * perc1);
     radius=0.1f + (0.3f * perc1);
     luminosity=0.1f + (0.5f * perc1);
     type=ldwarf;
    break;
    case 4: MKspectral = 'D'; // White dwarf
     photosphere.Int( 255, 255, 255, 196 );
     corona.Int( (int) (perc3*255), 0, (int)(perc3*255), 196 );
     mass=0.1f + (0.5f * perc1);
     radius=0.1f + (0.3f * perc1);
     luminosity=0.1f + (0.5f * perc1)*(2.0f * perc1);
     type=whitedwarf;
     nebula=circumstellardisk;
    break;
    case 5: MKspectral = 'R'; // Luminous red novae
     photosphere.Int( 255, (int) (perc3*255), (int) (perc3*255), 196 );
     corona.Int( 255, 0, 0, 196 );
     mass=0.1f + (0.5f * perc1);
     radius=0.5f + (0.3f * perc1);
     luminosity=0.1f + (0.5f * perc1);
     type=luminousred;
     nebula=planetarynebula;
    break;
    case 6: MKspectral = 'P'; // Protostar or T-Tauri
     photosphere.Int( (int) (xyzs[seed%100]*255), (int) (xyzs[seed%50]*255), (int) (perc3*255), 196 );
     corona.Int( (int) (xyzs[seed%25]*255), (int) (xyzs[seed%17]*255), (int) (perc3*255), 196 );
     mass=0.1f + (0.5f * perc1);
     radius=0.1f + (0.5f * perc1);
     luminosity=0.1f + (0.5f * perc1);
     type=( seed % 2 == 1 ? ttauri : protostar );
     if ( type == ttauri ) {
      if ( perc1 > 0.5f ) nebula=protoplanetary;
      else nebula=circumstellardisk;
     } else
     nebula=herbigharo;
    break;      
   }
  }
 } 
 else  if ( perc1 < 0.7f ) {
  MKspectral = seed % 2 == 1 ? 'G' : 'F';
  if ( MKspectral == 'G' ) {
   corona.Int( 255, (int) (perc3*255), (int) (perc3*255)/3, 196 );
   photosphere.Int( 255, (int) (perc3*255), (int) (perc3*255)/3, 196 );
   mass=0.8f + (1.05f * perc1);
   radius=0.96f + (0.30f * perc1);
   luminosity=0.6f + (0.9f * perc1); 
   type=mainsequence;
  } else
  if ( MKspectral == 'F' ) {
   corona.Int( 255, (int) (255), (int) (perc3*255)/2, 196 );
   photosphere.Int( 255, (int) (255), (int) (perc3*255)/2, 196 );
   mass=1.04f + (0.36f * perc1);
   radius=1.15f + (0.35f * perc1);
   luminosity=1.5f + (3.5f * perc1); 
   type=mainsequence3x;
  }
  wind=1.0f;
 } else 
 if ( perc3 < 0.8f ) {
  if ( seed % 2 == 1 ) { 
  int t=(seed+(int)(100.0f*perc1))%6;
  switch ( t ) {
   case 0: MKspectral='O';  // Blue/White Supergiant
           corona.Int( (int) (perc1*255)/2,(int) (perc1*255)/2,255, 196 );
           photosphere.Int( (int) (perc1*255)/2,(int) (perc1*255)/2,255, 196 );
           mass=16.0f + (16.0f * perc1);
           radius=6.6f;//6.6f;// + (6.6f * perc1);
           luminosity=30.0f;// + (30.0f * perc1); 
           wind=4.0f;
           type=(seed % 14 == 1 ? rvtauri :
                (seed % 13 == 1 ? pvtelescopii :
                mainsequence50x ) );
           if ( type == rvtauri ) {
            corona.Int( (int) (perc1*255),(int) (perc1*255),196, 196 );
            photosphere.Int( (int) (perc1*255),(int) (perc1*255),196, 196 );
            nebula=circumstellardisk;
           } else
           if ( type == pvtelescopii ) { // pulsar widely variable
            MKspectral='B';
            corona.Int( (int) (perc1*255)/2,(int) (perc1*255)/2,255, 196 );
            photosphere.Int( (int) (perc1*255)/2,(int) (perc1*255)/2,255, 196 );
            nebula=pulsarwindnebula;
           }
          break;
  case 1: MKspectral='W'; // Wolf-Rayet
           corona.Int( (int) (perc1*255),(int) (perc1*255),255, 196 );
           photosphere.Int( (int) (perc1*255)/5,(int) (perc1*255)/5,255, 196 );
           mass=20.0f+(1.0f * perc3);
           radius=2.2f - (0.5f * perc3);
           luminosity=50.0f;// + (30.0f * perc1); 
           wind=1.0f;
           type=wolf_rayet;
           nebula=wrcloud;
          break;
  case 2: MKspectral='P'; // Planetary Nebula
           corona.Int( (int) (perc1*255)/5,(int) (perc1*255)/5, 255, 196 );
           photosphere.Int( (int) (perc1*255)/5,(int) (perc1*255)/5,255, 196 );
           mass=(1.0f * perc3);
           radius=1.2f - (0.5f * perc3);
           luminosity=30.0f;// + (30.0f * perc1); 
           wind=1.0f;
           type=luminousblue;
           nebula=planetarynebula;
          break;
  case 3: MKspectral='Q'; // Novae
           corona.Int( (int) (perc1*255)/5,(int) (perc1*255)/5,255, 196 );
           photosphere.Int( (int) (perc1*255)/5,(int) (perc1*255)/5,255, 196 );
           mass=(1.0f * perc3);
           radius=1.2f - (0.5f * perc3);
           luminosity=30.0f;// + (30.0f * perc1); 
           wind=1.0f;
           type=novae;
           nebula=protoplanetary;
          break;
  case 4: MKspectral='M';  // Mira slow pulsing red
          corona.Int( 255, (int) (perc1*255)/5,(int) (perc1*255)/5, 196 );
          photosphere.Int(  255, (int) (perc1*255)/3,(int) (perc1*255)/3, 196 );
          mass=1.0f+(1.0f * perc3);
          radius=(1.0f * perc3);
          luminosity=30.0f;// + (30.0f * perc1); 
          wind=1.0f;
          type=mira;
          period+=15.0f*perc3;
          nebula=pulsarwindnebula;
         break;
  case 5: MKspectral='F';  // Delta Scuti peculiar chemicals (variable pulsar)
          corona.Int( 255, (int) (perc1*255)/5,(int) (perc1*255)/5, 196 );
          photosphere.Int(  255, (int) (perc1*255)/3,(int) (perc1*255)/3, 196 );
          mass=0.7f+(1.0f * perc3);
          radius=0.8f + (1.5f * perc3);
          luminosity=60.0f;// + (30.0f * perc1); 
          wind=3.0f;
          type=deltascuti;
          period=perc3*2.0f+0.1f;
          period2=perc3*5.0f+0.1f;
          period3=perc3*3.0f+0.1f;
          nebula=pulsarwindnebula;
         break;
   }
  } else {
   MKspectral = seed % 2 == 1 ? 'A' : 'B';
   if ( MKspectral == 'A' ) {
    corona.Int( (int) (perc1*255)/2+127, (int) (perc2*255)/2+127, 255, 196 );
    photosphere.Int( (int) (perc1*255)/2+127, (int) (perc2*255)/2+127, 255, 196 );
    mass=1.4f + (0.8f * perc1);
    radius=1.4f + (0.4f * perc1);
    luminosity=5.0f;// + (20.0f * perc1); 
    type=mainsequence;
   } else
   if ( MKspectral == 'B' ) { // Needs to be clustering..how to fix?   
    corona.Int( (int) (perc1*196), (int) (perc2*196), 255, 196 );
    photosphere.Int( (int) (perc1*255), (int) (perc2*255), 255, 196 );
    mass=2.1f + (13.9f * perc1);
    radius=1.8f;// + (4.8f * perc1);
    luminosity=25.0f;// + (5.0f * perc1); 
    type=mainsequence;
   }
   wind=2.0f;
  }
 } else  {
     MKspectral=( seed % 2 == 1 ? 'M' : 'K' );
     photosphere.Int( 0, 160, 0, 196 );
     corona.Int( 255, (int) (perc3*255) /5, (int)(perc3*255) / 2, 196 );
     mass=0.1f + (0.5f * perc1);
     radius=0.1f + (0.3f * perc1);
     luminosity=0.1f + (0.5f * perc1);
     type=reddwarf;
     nebula=oort;
     radius*=3.0f;
     //corona.Redf(1.0f); corona.Bluef(0.25f); corona.Greenf(0.0f);
 } 
  // HR diagram
  // star mass to age ratio: 1 mass = 10 billion years, bigger is less, 50x=millions
  // plantary nebula NGC6453
  // 1x small main sequence, 3x=massive main sequence, 50x=max massive giant
}

void Planet::Coelesce(SolSystem coelescing) {
 this->coelescing=coelescing;
 seed=1234567+(int)coelescing;
 if ( (int) coelescing <= 4 ) {
  position.x=iratiod((int) coelescing+1, 10);
  position.y=iratiod((int) coelescing+1, 10);
 } else {
  position.x=iratiod((int) coelescing+1, 10)+0.1f;
  position.y=iratiod((int) coelescing+1, 10)+0.1f;
 }
 switch ( coelescing ) {
 case Mercury:
    rotation_angle.x=50.0f;
    mass=0.25f;
    density=1.0f;
    biosphere=0.0f;
    gravity=0.2f;
    temperature=1000.0f;
    orbital_range=1.0f;
    radius=0.1f;
    sealevel=0.0f;
    magnetism=0.1f;
    name=string("Mercury");
    type=terrestrial;
    craters=1000;
    num_rings=0;
  break;
 case Venus:
   rotation_angle.x=150.0f;
   mass=0.8f;
   density=1.0f;
   biosphere=0.0f;
   gravity=5.0f;
   temperature=900.0f;
   orbital_range=80.0f;
   radius=0.4f;
   sealevel=10.0f;
   magnetism=0.1f;
   name=string("Venus");
   type=terrestrial;
   num_rings=0;
  break;
 case Earth:
   rotation.x=0.5;
   rotation_angle.x=250.0f;
   mass=1.0f;
   density=1.0f;
   biosphere=1.0f;
   gravity=1.0f;
   temperature=100.0f;
   orbital_range=90.0f;
   radius=0.5f;
   sealevel=1.0f;
   magnetism=1.0f;
   name=string("Earth");
   type=terrestrial;
   num_rings=0; // though there is 1
  break;
 case Mars:
   rotation_angle.x=70.0f;
   mass=1.0f;
   density=1.0f;
   biosphere=1.0f;
   gravity=1.0f;
   temperature=200.0f;
   orbital_range=400.0f;
   radius=0.45f;
   sealevel=1.0f;
   magnetism=1.0f;
   name=string("Mars");
   type=terrestrial;
   num_rings=1;
  break;
 case Saturn:
   rotation_angle.x=215.0f;
   mass=50.0f;
   density=10.0f;
   biosphere=0.0f;
   gravity=10.0f;
   temperature=500.0f;
   orbital_range=500.0f;
   radius=0.9f;
   sealevel=1.0f;
   magnetism=1.0f;
   name=string("Saturn");
   type=gas_giant;
   num_rings=14;
  break;
 case Jupiter:
   rotation_angle.x=350.0f;
   mass=50.0f;
   density=1.0f;
   biosphere=0.0f;
   gravity=10.0f;
   temperature=800.0f;
   orbital_range=720.0f;
   radius=1.0f;
   sealevel=1.0f;
   magnetism=1.0f;
   name=string("Jupiter");
   type=gas_giant;
   num_rings=14;
  break;
 case Uranus:
   rotation_angle.x=90.0f;
   mass=50.0f;
   density=2.0f;
   biosphere=0.0f;
   gravity=5.0f;
   temperature=100.0f;
   orbital_range=800.0f;
   radius=0.8f;
   sealevel=1.0f;
   magnetism=1.0f;
   name=string("Uranus");
   type=gas_giant;
   num_rings=1;
  break;
 case Neptune:
   rotation_angle.x=200.0f;
   mass=50.0f;
   density=2.0f;
   biosphere=0.0f;
   gravity=5.0f;
   temperature=0.0f;
   orbital_range=820.0f;
   radius=0.7f;
   sealevel=1.0f;
   magnetism=1.0f;
   name=string("Neptune");
   type=gas_giant;
   num_rings=0;
  break;
 case Pluto:
   rotation_angle.x=75.0f;
   mass=1.0f;
   density=1.0f;
   biosphere=0.0f;
   gravity=0.4f;
   temperature=-200.0f;
   orbital_range=880.0f;
   radius=0.2f;
   sealevel=1.0f;
   magnetism=1.0f;
   name=string("Pluto");
   type=dwarf_planet;
   num_rings=0;
  break;
 case Eris:
   rotation_angle.x=45.0f;
   mass=1.0f;
   density=1.0f;
   biosphere=0.0f;
   gravity=0.4f;
   temperature=-272.0f;
   orbital_range=970.0f;
   radius=0.1f;
   sealevel=1.0f;
   magnetism=1.0f;
   name=string("Eris");
   type=dwarf_planet;
   num_rings=0;
  break;
 }
}

void Planet::Coelesce(int seed) {
 Random *set=randoms.GetSet(seed);
 this->seed=seed;
 magnetism=xyzs[seed*9%5959];
 rotation.z=position.x=(0.1f)+((xyzs[seed*9%23456]+xyzs[seed*2%33333])/2); if ( position.x > 1.0 ) position.x+=1.0-position.x;
 rotation.x=position.y=(0.1f)+((xyzs[seed*2%54321]+xyzs[seed*3%44444])/2); if ( position.y > 1.0 ) position.y+=1.0-position.y;
 rotation.y=position.z=(0.1f)+((xyzs[seed*3%21542]+xyzs[seed*5%22222])/2); if ( position.z > 1.0 ) position.z+=1.0-position.z;
 float perc1=randomify2(seed+(++planetSeed),STARS+planetSeed);
 float perc2=1/perc1;
 float perc3=(float) abs(sin((float)(seed+(++planetSeed))));   
 int i,num_moons;
 if ( seed > 0 ) {
   num_rings=((seed+(++planetSeed))*3) % 3 == 1
            ? 1 
            : (seed % 2 == 1 ? 2 : 4);
   num_moons=((seed+(++planetSeed))*3) % 3 == 1
            ? (int) randomify( 1, seed % (int) ( 20.0f*perc3+1.0f ), seed )
            : 0;
   while ( num_moons > 4 ) num_moons/=2;
 }
 type = (PlanetTypes) (seed % 3);
 mass=10.0f*xyzs[(seed+(++planetSeed))%20001];
 density=xyzs[(seed+(++planetSeed))%20666];
 if ( mass > 4.0f ) type=gas_giant; 
 orbital_range=700.0f*set->unique(seed+planetSeed,seed+(planetSeed++),seed+planetSeed);
 radius = xyzs[(seed+(++planetSeed))%45678];
 // Moons
 for ( i=0; i<num_moons; i++ ) {
  Planet *m=new Planet;
  m->Moonify(this,seed+(planetSeed)+(++moonSeed));
  moons->Append(m);
 }
 if ( type == gas_giant || perc1 < 0.5f ) craters=0;
 else
 craters=(seed+(++planetSeed))/2 % 2 == 1 ? (int) randomify( 0, seed % (int) URANGE( 1.0f, (100.0f*perc3), 100.0f), seed ) : 0;

 switch ( type ) {
  case dwarf_planet:
      mass/=4.0f; 
      radius/=2.0f;
   case terrestrial:
     radius*=2.0f;
    break;
     case gas_giant:
      if ( perc3 > 0.5 ) {
       radius*=1.0f+5.0f*xyzs[(seed+(++planetSeed))%50013];
       mass*=1.0f+5.0f*xyzs[(seed+(++planetSeed))%50012];
      }
    break;
 }
 if ( type != gas_giant || radius > 0.60f && radius < 0.90f ) radius /= (float) (1+ (seed+(++planetSeed)) % 10);
 if ( radius > 1.0f ) radius=1.0f;
 planetsCreated++;
}

void Planet::Moonify(Planet *p,int seed) {
 moonsCreated++;
 isMoon=true;
 this->seed=seed;
 magnetism=xyzs[seed*9%5959];
 rotation.x=( seed %4 <= 1 ? 1 : -1 )*(position.x=(0.1)+((xyzs[seed*9%23456]+xyzs[seed*2%33333])/2)); if ( position.x > 1.0 ) position.x+=1.0-position.x;
 rotation.y=( seed %4 <= 1 ? 1 : -1 )*(position.y=(0.1)+((xyzs[seed*2%54321]+xyzs[seed*3%44444])/2)); if ( position.y > 1.0 ) position.y+=1.0-position.y;
 rotation.z=( seed %4 <= 1 ? 1 : -1 )*(position.z=(0.1)+((xyzs[seed*3%21542]+xyzs[seed*5%22222])/2)); if ( position.z > 1.0 ) position.z+=1.0-position.z;
 float perc1=randomify2(seed+(++planetSeed),STARS+planetSeed);
 float perc2=1/perc1;
 float perc3=(float) abs(sin((float)(seed+(++planetSeed))));   
 if ( seed > 0 ) {
   num_rings=(seed+(++planetSeed))/2 % 3 == 1
            ? 1 
            : (seed % 2 == 1 ? 2 : 4);
 }
 type = (PlanetTypes) (seed % 3);
 mass=10.0f*xyzs[(seed+(++planetSeed))%20001];
 density=xyzs[(seed+(++planetSeed))%20666];
 if ( mass > 4.0f ) type=gas_giant; 
 orbital_range=700.0f*xyzs[(seed+(++planetSeed))%20002];
 radius = xyzs[(seed+(++planetSeed))%20003]*(2.0f/3.0f);
 if ( type != gas_giant ) radius /= 2.0f;
 if ( type == gas_giant || perc1 < 0.5f ) craters=0;
 else
 craters=(seed+(++planetSeed))/2 % 2 == 1 ? (int) randomify( 0, seed % (int) URANGE( 1.0f, (100.0f*perc3), 100.0f), seed ) : 0;

 switch ( type ) {
  case dwarf_planet:
      mass/=4.0f; //radius/=2.0f;
   case terrestrial:
    break;
     case gas_giant:
      if ( perc3 > 0.5 ) {
       radius*=1.0f+10.0f*xyzs[(seed+(++planetSeed))%50013];
       mass*=1.0f+10.0f*xyzs[(seed+(++planetSeed))%50012];
      }
    break;
 }

 if ( radius > 0.75f ) radius=0.75f;
}

void Planet::LoadMoons( const char * w) {
 moons.Recycle();
 moons->fromString(w);
}

string Planet::SaveMoons() {
 if ( moons ) return moons->toString();
 else return string("");
}