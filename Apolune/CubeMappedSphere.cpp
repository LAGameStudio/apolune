#include "CubeMappedSphere.h"

CubeMappedSphere3d cubemappedsphere3d;

PrecomputedCubeMappedSphereProfiles spheres;
PrecomputedCubeMappedSphere3ds sphereModels;

PrecomputedCubeMappedSphereProfiles spheresHiRes;
PrecomputedCubeMappedSphere3ds sphereModelsHiRes;

void PrecomputeSpheres() { // called as sphere.Generate() in main.cpp
 OUTPUT("Pregenerating cubed mapped sphere profiles and corresponding model format(s).\n");
 spheres.Generate();
 EACH(spheres.first,PrecomputedCubeMappedSphereProfile,sphere) sphereModels.Append(new ProfiledCubeMappedSphere3d(1.0f,sphere));
 OUTPUT("Pregenerating hi-res cube mapped sphere profiles and models.\n");
 spheresHiRes.Append(new PrecomputedCubeMappedSphereProfile(10,0.5f));
 spheresHiRes.Append(new PrecomputedCubeMappedSphereProfile(16,0.5f));
 spheresHiRes.Append(new PrecomputedCubeMappedSphereProfile(20,0.5f));
 spheresHiRes.Append(new PrecomputedCubeMappedSphereProfile(32,0.5f)); 
 EACH(spheresHiRes.first,PrecomputedCubeMappedSphereProfile,sphere) sphereModelsHiRes.Append(new ProfiledCubeMappedSphere3d(1.0f,sphere));
}