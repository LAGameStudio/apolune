#include "Sphere.h"


Indexed<Zfloat> sinLUT;
Indexed<Zfloat> cosLUT;

void initsincosLUT() {
 sinLUT.Size(SINCOS_LENGTH);
 cosLUT.Size(SINCOS_LENGTH);

 for (int i = 0; i < SINCOS_LENGTH; i++) {
  sinLUT[i] = (float) sin(i * DEG_TO_RAD * SINCOS_PRECISION);
  cosLUT[i] = (float) cos(i * DEG_TO_RAD * SINCOS_PRECISION);
 }
}

Geosphere3d geosphere3d;




#if defined(NEVER)
 // Generate Sphere Primitives, Spherical Coordinates
 sphere3d.res=8;
 sprintf_s( sphere3d.name, "Sphere %d Spherical-Y", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geosphere-y8.txt" );
 models.Clear();

 sphere3d.res=10;
 sprintf_s( sphere3d.name, "Sphere %d Spherical-Y", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geosphere-y10.txt" );
 models.Clear();

 sphere3d.res=32;
 sprintf_s( sphere3d.name, "Sphere %d Spherical-Y", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geosphere-y32.txt" );
 models.Clear();

 sphere3d.res=64;
 sprintf_s( sphere3d.name, "Sphere %d Spherical-Y", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geosphere-y64.txt" );
 models.Clear();

 // Generate Sphere Primitives, SphereMappingZ
 sphere3d.textureCoordOptions=SphericalMappingZ;
 sphere3d.res=8;
 sprintf_s( sphere3d.name, "Sphere %d SphereZ", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geosphere-z8.txt" );
 models.Clear();

 sphere3d.res=10;
 sprintf_s( sphere3d.name, "Sphere %d SphereZ", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geosphere-z10.txt" );
 models.Clear();

 sphere3d.res=32;
 sprintf_s( sphere3d.name, "Sphere %d SphereZ", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-z32.txt" );
 models.Clear();

 sphere3d.res=64;
 sprintf_s( sphere3d.name, "Sphere %d SphereZ", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-z64.txt" );
 models.Clear();

 // Generate Sphere Primitives, PositionalSphereMapping
 sphere3d.textureCoordOptions=PositionalSphereMapping;

 sphere3d.res=8;
 sprintf_s( sphere3d.name, "Sphere %d SphericalMappingZ", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-pos8.txt" );
 models.Clear();

 sphere3d.res=10;
 sprintf_s( sphere3d.name, "Sphere %d SphericalMappingZ", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-pos10.txt" );
 models.Clear();

 sphere3d.res=32;
 sprintf_s( sphere3d.name, "Sphere %d SphericalMappingZ", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-pos32.txt" );
 models.Clear();

 sphere3d.res=64;
 sprintf_s( sphere3d.name, "Sphere %d SphericalMappingZ", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-pos64.txt" );
 models.Clear();

 // Generate Sphere Primitives, CubeMapNormal
 sphere3d.textureCoordOptions=CubeMapNormal;
 sphere3d.res=8;
 sprintf_s( sphere3d.name, "Sphere %d Positional", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-cube8.txt" );
 models.Clear();

 sphere3d.res=10;
 sprintf_s( sphere3d.name, "Sphere %d Positional", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-cube10.txt" );
 models.Clear();

 sphere3d.res=32;
 sprintf_s( sphere3d.name, "Sphere %d Positional", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-cube32.txt" );
 models.Clear();

 sphere3d.res=64;
 sprintf_s( sphere3d.name, "Sphere %d Positional", sphere3d.res );
 models.Add( &sphere3d );

 models.SaveModels( "data/models/primitives/geospheres-cube64.txt" );
 models.Clear();

 // Unit Cube
 models.Add( &cube3d );
 models.SaveModels( "data/models/primitives/unitcube.txt" );
#endif
