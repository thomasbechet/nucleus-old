#define MAX_RAYMARCH_STEP  512
#define MAX_DISTANCE       300.0
#define MIN_HIT_DISTANCE   0.0001
#define MAX_SHAPE_COUNT    128
#define MAX_MATERIAL_COUNT 64
#define MAX_INSTANCE_COUNT 128
#define EPSILON            0.005

float map(in vec3 p) {
    return length(p) - 1.0;
}

layout(set = 0, binding = 1) uniform TypeArraysHeaderUBO {
	uint instanceIndices0Count;
	uint instanceIndices0[128];
};

struct InstanceData0 {
	float radius;
};
struct Instance0 {
	mat4 invMatrix;
	InstanceData0 data;
};
layout(set = 0, binding = 2) uniform TypeArraysUBO {
	Instance0 instances0[128];
};

for (uint i = 0; i < instanceIndices0Count; i++) {
	instances0[instancesIndices0[i]]
}

vec3 normalScene(in vec3 p, in float sd) {
#if 1
    const vec2 e = vec2(EPSILON, 0);
    return normalize(vec3(
        sd - map(vec3(p - e.xyy)),
        sd - map(vec3(p - e.yxy)),
        sd - map(vec3(p - e.yyx))
    ));
#else
    vec3 n = vec3(0.0);
    for(int i = 0; i < 4; i++) {
        vec3 e = 0.5773 * (2.0 * vec3((((i + 3) >> 1) & 1),((i >> 1) & 1),(i & 1)) - 1.0);
        n += e * map(p + 0.0005 * e).x;
    }
    return normalize(n);
#endif
}

struct HitInfo {
	vec3 position;   // Relevant if intersect
	int materialID;  // Relevant if intersect
	vec3 normal;     // Relevant if intersect
	int shapeID;	 // Relevant if intersect
	float depth;     // Relevant if intersect or maxDepth instead
	bool intersect;
};

HitInfo intersectScene(in vec3 p, in vec3 dir, in float maxDepth) {
	HitInfo hit;
	
	hit.intersect = false;
	hit.depth     = EPSILON;
	
	float sd;
	for (int step = 0; step < MAX_RAYMARCH_STEP; step++) {
		hit.position = p + hit.depth * dir;
		sd = map(hit.position);
		if (sd < MIN_HIT_DISTANCE) {
			hit.intersect = true;
			break;
		}
		hit.depth += sd;
		if (hit.depth > maxDepth) {
			hit.depth = maxDepth;
			break;
		}
	}

	if (hit.intersect) {
		hit.normal = normalScene(hit.position, sd);
	}
	
	return hit;
}