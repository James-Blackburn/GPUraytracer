// DEVICE STRUCTS --------------------------------------------------------

typedef struct {
	float3 pixel00Center;
	float3 pixelDeltaU;
	float3 pixelDeltaV;
	float3 position;
} Camera;

typedef struct {
	float3 colour;
	float fuzz;
	uint type;
} Material;

typedef struct {
	Material material;
	float4 position;
} Sphere;

typedef struct {
	float3 position;
	float3 normal;
	__constant Material* material;
	int frontFace;
	float t;
} RayHit;

typedef struct {
	float3 origin;
	float3 direction;
} Ray;

// DEVICE ENUMS --------------------------------------------------------

enum MaterialTypes {
	DIFFUSE,
	DIFFUSE_CHEQUERED,
	METALLIC,
	LIGHT_SOURCE
};

// AUXILIARY FUNCTIONS --------------------------------------------------------

inline float3 ray_at(Ray* ray, float t) {
	return ray->origin + t * ray->direction;
}

inline float rand(uint* seed)
{
	*seed = (long)(*seed * 16807u + 1039423u) & 2147483646u;
	return (float)*seed / 2147483646.0f;
}

inline float3 randomUnitVec(uint* seed) {
	return cbrt(rand(seed)) * fast_normalize((float3)(
		rand(seed) * 2.0f - 1.0f,
		rand(seed) * 2.0f - 1.0f, 
		rand(seed) * 2.0f - 1.0f
	));
}

int sphere_hit(__constant Sphere* sphere, Ray* ray, RayHit* rayHit, float tMin, float tMax) {
	// calculate discriminant to work out if ray intersects circle
	const float3 center = sphere->position.xyz - ray->origin;
	const float a = pow(fast_length(ray->direction), 2);
	const float h = dot(ray->direction, center);
	const float c = pow(fast_length(center), 2) - sphere->position.w * sphere->position.w;
	const float discriminant = h * h - a * c;

	// ray does not intersect with circle
	if (discriminant < 0.0f) {
		return 0;
	}

	// check if t is within specified range 
	const float sqrtDiscriminant = half_sqrt(discriminant);
	float t = (h - sqrtDiscriminant) / a;
	if (t < tMin || t > tMax) {
		t = (h + sqrtDiscriminant) / a;
		if (t < tMin || t > tMax) {
			return 0;
		}
	}

	// register the hit
	rayHit->t = t;
	rayHit->material = &sphere->material;
	rayHit->position = ray_at(ray, t);
	rayHit->normal = (rayHit->position - sphere->position.xyz) / sphere->position.w;
	rayHit->frontFace = dot(ray->direction, rayHit->normal) < 0.0f;

	if (!rayHit->frontFace) {
		rayHit->normal = -rayHit->normal;
	}

	return 1;
}

float3 traceRay(__constant Sphere* spheres,
				uint nSpheres, uint maxDepth, 
				Ray* ray, uint* seed) {
	
	float3 rayColour = (float3)(1.0f, 1.0f, 1.0f);

	// trace ray and child rays until depth limit exceeded
	for (int depth = 0; depth < maxDepth; depth++) {
		float closest = FLT_MAX;
		int rayCollision = 0;
		RayHit hit;

		// check for ray hitting objects in scene
		for (int i = 0; i < nSpheres; i++) {
			if (sphere_hit(&spheres[i], ray, &hit, 0.001f, closest)) {
				closest = hit.t;
				rayCollision = 1;
			}
		}

		// no objects hit
		if (!rayCollision)
			return (float3)(0.0f, 0.0f, 0.0f);

		// reflect ray from closest hit
		float3 rDir;
		float3 hitColour = hit.material->colour;
		
		switch (hit.material->type) {
		case DIFFUSE: // diffuse sphere
			rDir = fast_normalize(hit.normal + randomUnitVec(seed));
			rDir = (dot(hit.normal, rDir) < 0.0f) ? -rDir : rDir;
			break;
		case DIFFUSE_CHEQUERED: // chequered sphere
			rDir = fast_normalize(hit.normal + randomUnitVec(seed));
			rDir = (dot(hit.normal, rDir) < 0.0f) ? -rDir : rDir;
			if ((int)(floor(hit.position.x) + floor(hit.position.y) + floor(hit.position.z)) % 2 == 0)
				hitColour = (float3)(0.025f, 0.025f, 0.025f);
			break;
		case METALLIC: // metallic sphere
			rDir = fast_normalize(randomUnitVec(seed) * hit.material->fuzz +
				ray->direction - 2.0f * dot(ray->direction, hit.normal) * hit.normal);
			break;
		case LIGHT_SOURCE: // light source
			return rayColour * hit.material->colour;
		default: // invalid material type
			return (float3)(0.0f, 0.0f, 0.0f);
		}

		ray->origin = hit.position;
		ray->direction = rDir;
		rayColour *= hitColour;
	}
	
	return (float3)(0.0f, 0.0f, 0.0f);
}

// KERNELS --------------------------------------------------------

__kernel void traceSamples(__constant Camera* camera, 
						   __constant Sphere* spheres,
						   __global float3* render,
						   uint nSpheres, uint maxDepth,
						   uint nSamples, uint seed) {

	const uint gid = get_global_id(1) * get_global_size(0) + get_global_id(0);
	const float x = get_global_id(0);
	const float y = get_global_id(1);
	uint threadSeed = (seed + gid) % UINT_MAX;
	
	Ray ray;
	float3 pixelColour = (float3)(0.0f, 0.0f, 0.0f);

	// trace the rays, according to no. of samples, to get pixel colour
	for (int i = 0; i < nSamples; i++) {
		// construct a ray from the camera position to the current pixel + random offset
		ray.origin = camera->position;
		ray.direction = fast_normalize(camera->pixel00Center
			+ (x + rand(&threadSeed) - 0.5f) * camera->pixelDeltaU
			+ (y + rand(&threadSeed) - 0.5f) * camera->pixelDeltaV
			- camera->position);
		
		// trace the path of the ray throughout the scene
		pixelColour += traceRay(spheres, nSpheres, maxDepth, &ray, &threadSeed);
	}

	// get final pixel colour from averaging samples, sqrt() for gamma correcting
	render[gid] += pixelColour;
}

__kernel void averageSamples(__global float3* render, uint nSamples) {
	// get final pixel colour from averaging samples, sqrt() for gamma correcting
	const uint gid = get_global_id(1) * get_global_size(0) + get_global_id(0);
	render[gid] = half_sqrt(render[gid] / (float)nSamples);
}