#include "shaderSrc.h"

const char* phongVertex = STRINGIFY(
#version 330\n
in vec3 vVertex;
in vec3 vNormal;
in vec2 vTexCoords;
uniform mat4 mv;
uniform mat4 p;
uniform vec3 vLightPosition;
smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;
smooth out vec2 vVaryingTexCoords;
void main(void)
{
	vVaryingNormal = normalize((mv * vec4(vNormal, 1.0) - mv * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
	vec4 vPosition = mv * vec4(vVertex, 1.0);
	vec3 vPosition3 = vPosition.xyz / vPosition.w;
	vVaryingLightDir = vLightPosition - vPosition3;
	vVaryingTexCoords = vTexCoords;
	gl_Position = p * vPosition;
}
);

const char* phongFragment = STRINGIFY(
#version 330\n
	out vec4 vFragColor;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform sampler2D colorMap;
in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 vVaryingTexCoords;
void main(void)
{
	float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));

	vFragColor = diff * texture(colorMap, vVaryingTexCoords);
	vFragColor += ambientColor;
	vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir), normalize(vVaryingNormal)));
	float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
	if (diff != 0)
	{
		float fSpec = pow(spec, 128.0);
		vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
	}
}
);

//const char* bumpVertex = STRINGIFY(
//#version 330\n
//	in vec3 vVertex;
//in vec3 vNormal;
//in vec2 vTexCoords;
//in vec3 t;
//in vec3 b;
//in vec3 n;
//uniform mat4 mv;
//uniform mat4 p;
//uniform vec3 vLightPosition;
//smooth out vec3 vVaryingNormal;
//smooth out vec3 vVaryingLightDir;
//smooth out vec2 vVaryingTexCoords;
//smooth out vec3 vVaryingLightDirO;
//void main(void)
//{
//	vVaryingNormal = normalize((mv * vec4(vNormal, 1.0) - mv * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
//	vec4 vPosition = mv * vec4(vVertex, 1.0);
//	vec3 vPosition3 = vPosition.xyz / vPosition.w;
//	vVaryingLightDirO = vLightPosition - vPosition3;
//
//	mat3 tbnInverse = (mat3(t, b, n));
//	//mat3 tbnInverse = mat3(n,n,n);
//	vec3 tbnx = vec3(t.x, b.x, n.x);
//	vec3 tbny = vec3(t.y, b.y, n.y);
//	vec3 tbnz = vec3(t.z, b.z, n.z);
//
//	vec3 objLightDir = vec3(1.0, 0.0, 1.0);
//	//vVaryingLightDir = normalize(tbnInverse * objLightDir);
//	vVaryingLightDir = normalize(vec3(dot(tbnx,objLightDir), dot(tbny,objLightDir), dot(tbnz,objLightDir)));
//	//vVaryingLightDir = normalize(vec3(dot(t,objLightDir), dot(b,objLightDir), dot(n,objLightDir)));
//	vVaryingTexCoords = vTexCoords;
//
//	gl_Position = p * vPosition;
//
//	//vVaryingLightDir = (normalize(n) + vec3(1.0, 1.0, 1.0))/2.0;
//	//vVaryingNormal = normalize((mv * vec4(vNormal, 1.0) - mv * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
//	//vec4 vPosition = mv * vec4(vVertex, 1.0);
//	//vec3 vPosition3 = vPosition.xyz / vPosition.w;
//	//vVaryingLightDir = vLightPosition - vPosition3;
//	//vVaryingTexCoords = vTexCoords;
//	//gl_Position = p * vPosition;
//}
//);
//
//const char* bumpFragment = STRINGIFY(
//#version 330\n
//	out vec4 vFragColor;
//uniform vec4 ambientColor;
//uniform vec4 diffuseColor;
//uniform vec4 specularColor;
//uniform sampler2D colorMap;
//uniform sampler2D heightMap;
//in vec3 vVaryingNormal;
//in vec3 vVaryingLightDir;
//in vec2 vVaryingTexCoords;
//in vec3 vVaryingLightDirO;
//void main(void)
//{
//	vec2 du = vec2(1.0/800.0, 0.0);
//	vec2 dv = vec2(0.0f, 1.0/800.0);
//	float hight = (texture(heightMap, vVaryingTexCoords)).r;
//	//if (hight > 0.0)
//	{
//		float hghr = (texture(heightMap, vVaryingTexCoords)).r
//			- (texture(heightMap, vVaryingTexCoords+1.0 * du)).r;
//		float hgha = (texture(heightMap, vVaryingTexCoords)).r
//			- (texture(heightMap, vVaryingTexCoords+1.0 * dv)).r;
//		vec3 tbnN;
//		if (hgha < 0.0)
//		{
//			tbnN = normalize(vec3(-hgha, -hghr, 0.01));
//		}
//		else
//			tbnN = normalize(vec3(hgha, hghr, 0.01));
//		tbnN = normalize(vec3(0.0, 0.0, 0.01));
//		float diff = max(0.0, dot(normalize(tbnN), (vVaryingLightDir)));
//
//		vFragColor = diff * texture(colorMap, vVaryingTexCoords);
//		vFragColor += ambientColor;
//		vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir), normalize(vVaryingNormal)));
//		float spec = max(0.0, dot(normalize(tbnN), vReflection));
//		if (diff != 0)
//		{
//			float fSpec = pow(spec, 128.0);
//			vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
//		}
//	}
//	//else
//	//{
//	//	float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDirO)));
//
//	//	vFragColor = diff * texture(colorMap, vVaryingTexCoords);
//	//	vFragColor += ambientColor;
//	//}
//
//	//vFragColor.rgb = (normalize(vVaryingLightDir) + vec3(1.0, 1.0, 1.0))/2.0;
//}
//);
const char* bumpVertex = STRINGIFY(
#version 330\n
in vec3 vVertex;
in vec3 vNormal;
in vec2 vTexCoords;
uniform mat4 mv;
uniform mat4 p;
uniform vec3 vLightPosition;
smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;
smooth out vec2 vVaryingTexCoords;
void main(void)
{
	vVaryingNormal = normalize((mv * vec4(vNormal, 1.0) - mv * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
	vec4 vPosition = mv * vec4(vVertex, 1.0);
	vec4 objLightPosition = mv * vec4(vLightPosition, 1.0);
	vec3 vObjLightPosition = objLightPosition.xyz ;
	vec3 vPosition3 = vPosition.xyz / vPosition.w;
	vVaryingLightDir = vLightPosition - vPosition3;
	//vVaryingLightDir = (- vObjLightPosition + vPosition3);
	vVaryingTexCoords = vTexCoords;
	gl_Position = p * vPosition;
}
);

const char* bumpFragment = STRINGIFY(
#version 330\n
out vec4 vFragColor;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform sampler2D colorMap;
uniform sampler2D heightMap;
uniform sampler2D verticeMap;
in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 vVaryingTexCoords;
void main(void)
{
	float d1 = 1.0/1024.0;
	float d2 = 1.0/540.0;
	vec2 du = vec2(d1, 0.0);
	vec2 dv = vec2(0.0, d2);
	vec3 u = ((texture(verticeMap, vVaryingTexCoords+du)).rgb - (texture(verticeMap, vVaryingTexCoords-du)).rgb) / (2.0*d1);
	vec3 v = ((texture(verticeMap, vVaryingTexCoords+dv)).rgb - (texture(verticeMap, vVaryingTexCoords-dv)).rgb) / (2.0*d2);
	vec3 n = cross(u, v);
	if (dot(n,vVaryingNormal) < 0.0)
	{
		n = cross(v, u);
	}
	//n = normalize(vVaryingNormal);
	//v = normalize(cross(u, n));
	
	u = normalize(u);
	//v = normalize(cross(u, n));
	v = normalize(v);
	n = normalize(n);
	mat3 tbn = mat3
		( u.x, u.y, u.z,
		v.x, v.y, v.z,
		n.x, n.y, n.z
		);
	mat3 worldtouv = inverse(tbn);

	float hight = (texture(heightMap, vVaryingTexCoords)).r;
	if (hight > 0.0)
	{
		float hghr = (texture(heightMap, vVaryingTexCoords+du)).r - (texture(heightMap, vVaryingTexCoords-1.0 * du)).r;
		float hgha = (texture(heightMap, vVaryingTexCoords+dv)).r - (texture(heightMap, vVaryingTexCoords-1.0 * dv)).r;
		
		//1.translate light to uv
		//vec3 tbnN;
		//tbnN = normalize(vec3(hgha, hghr, 1.0));
		//vec3 uvLightDir = normalize(worldtouv * vVaryingLightDir);
		//float diff = max(0.0, dot(tbnN, uvLightDir));

		//vFragColor = diff * texture(colorMap, vVaryingTexCoords);

		//vFragColor = diff * vec4(0.5, 0.5, 0.3, 1.0);
		//vFragColor += ambientColor;
		//vec3 vReflection = normalize(reflect(-uvLightDir, tbnN));
		//float spec = max(0.0, abs(dot(tbnN, vReflection)));
		//if (diff != 0)
		//{
		//	float fSpec = pow(spec, 128.0);
		//	vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
		//}

		//2.difference normal in xyz
		vec3 newNormal = vVaryingNormal;
		//vec3 newNormal = n;
		newNormal -= 2.0 * (hghr * u + hgha * v);
		float diff = max(0.0, dot(normalize(vVaryingLightDir), normalize(newNormal)));
		vFragColor = diff * texture(colorMap, vVaryingTexCoords);
		//vFragColor.r = 1.0;
		vFragColor += vec4(0.01, 0.01, 0.01, 0.0);
		vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir), normalize(newNormal)));
		float spec = max(0.0, dot(normalize(newNormal), vReflection));
		if (diff != 0)
		{
			float fSpec = pow(spec, 128.0);
			vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
		}
	}
	else
	{
		float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));

		vFragColor = diff * texture(colorMap, vVaryingTexCoords);
		vFragColor += ambientColor;
	//	vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir), normalize(vVaryingNormal)));
	//	float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
	//	if (diff != 0)
	//	{
	//		float fSpec = pow(spec, 128.0);
	//		vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
	//	}
	}
}
);

const char* uvModelInfoVertex = STRINGIFY(
#version 120\n
in vec3 vVertex;
in vec3 vNormal;
in vec2 vTexCoords;
uniform mat4 mv;
uniform mat4 p;
varying vec3 vVaryingNormal;
varying vec3 vVaryingXyzs;
void main(void)
{
	vec4 vPosition = mv * vec4(vTexCoords, 0.0, 1.0);
	vVaryingNormal = vNormal;
	vVaryingXyzs = vVertex;
	gl_Position = p * vPosition;
}
);

const char* uvModelInfoFragment = STRINGIFY(
#version 120\n
varying vec3 vVaryingNormal;
varying vec3 vVaryingXyzs;
void main(void)
{
	vec3 testColor = (atan(vVaryingNormal)/3.1416 + vec3(0.5, 0.5, 0.5));
	gl_FragData[0] = vec4(vVaryingNormal, 1.0);
	gl_FragData[1] = vec4(vVaryingXyzs, 1.0);

	//gl_FragData[1] = vec4(testColor, 1.0);
}
);

const char* pressTextureVertex = STRINGIFY(
#version 330\n
in vec2 vVertex;
in vec2 vTexCoords;
uniform mat4 mv;
uniform mat4 p;
smooth out vec2 vVaryingTexCoords;
void main(void)
{
	vec4 vPosition = mv * vec4(vVertex, 0.0, 1.0);
	vVaryingTexCoords = vTexCoords;
	gl_Position = p * vPosition;
}
);

const char* pressTextureFragment = STRINGIFY(
#version 330\n
uniform vec3 gravity;
uniform sampler2D normalMap;
out vec4 vFragColor;
in vec2 vVaryingTexCoords;
void main(void)
{
	vec3 normal = (texture(normalMap, vVaryingTexCoords)).xyz;
	normal = normalize(normal);
	float press = -dot(gravity, normal);
	//float test = atan(press) / 3.1416 + 0.5;
	vFragColor = vec4(press, press, press, 1.0);
	//vFragColor = vec4(test, test, test, 1.0);
}
);

const char* gravityTextureVertex = STRINGIFY(
#version 330\n
	in vec2 vVertex;
in vec2 vTexCoords;
uniform mat4 mv;
uniform mat4 p;
smooth out vec2 vVaryingTexCoords;
void main(void)
{
	vec4 vPosition = mv * vec4(vVertex, 0.0, 1.0);
	vVaryingTexCoords = vTexCoords;
	gl_Position = p * vPosition;
}
);

const char* gravityTextureFragment = STRINGIFY(
#version 330\n
uniform vec3 gravity;
uniform sampler2D verticeMap;
uniform sampler2D pressMap;
uniform sampler2D normalMap;
out vec4 vFragColor;
in vec2 vVaryingTexCoords;
void main(void)
{
	float d = 1.0/1024.0;
	vec2 du = vec2(d, 0.0);
	vec2 dv = vec2(0.0, d);
	vec3 u = ((texture(verticeMap, vVaryingTexCoords+du)).rgb - (texture(verticeMap, vVaryingTexCoords-du)).rgb) / (2.0*d);
	vec3 v = ((texture(verticeMap, vVaryingTexCoords+dv)).rgb - (texture(verticeMap, vVaryingTexCoords-dv)).rgb) / (2.0*d);
	vec3 n = cross(u, v);
	vec3 normal = (texture(normalMap, vVaryingTexCoords)).rgb;
	//if (dot(n,normal) < 0.0)
	//{
		//n = cross(v, u);
		//n = length(n) * normalize(normal);

		//v = cross(u, n);
	//}
	//else
	//{
		//n = length(n) * normalize(normal);

		//v = cross(n, u);
	//}

	mat3 tbn = mat3
		( u.x, u.y, u.z,
		v.x, v.y, v.z,
		n.x, n.y, n.z
		);
	mat3 worldtouv = inverse(tbn);

	vec3 gra = gravity + (texture(pressMap, vVaryingTexCoords)).rgb;
	vec3 g = worldtouv * gra;

	vFragColor = vec4(g, 1.0);

	//vec3 testColor = (vec3(atan(g.r)/3.1416, atan(-g.g)/3.1416, atan(g.b)/3.1416) + vec3(0.5, 0.5, 0.5));
	//
	//vFragColor = vec4(testColor.r, testColor.r, testColor.r, 1.0);
	//vFragColor = vec4(testColor.g, testColor.g, testColor.g, 1.0);
	//vFragColor = vec4(testColor.b, testColor.b, testColor.b, 1.0);
}
);

const char* genHeightTextureVertex = STRINGIFY(
#version 330\n
in vec2 vVertex;
in float vMass;
uniform mat4 p;
uniform mat4 mv;
smooth out float mass;
void main()
{
	vec4 vPosition = mv * vec4(vVertex, 0.0, 1.0);

	gl_Position = p * vPosition;

	gl_PointSize = vMass * 10.0f;
	mass = vMass;
}
);

const char *genHeightTextureFragment = STRINGIFY(
	#version 330\n
in float mass;
uniform float poly;
void main()
{
	vec2 p = gl_PointCoord * 2.0 - vec2(1.0, 1.0);
	float mag = dot(p, p);
	if (mag > 1.0) discard;   // kill pixels outside circle

	float r = length(p);

	float c = mass/100.0;
	float w = 0.0;
	if (r < 1.0)
	{
		w = poly * pow(1.0 - r*r, 3.0);
	}

	gl_FragColor = vec4(c, c, c, w);
}
);

const char* updatePvVertex = STRINGIFY(
#version 330\n
in vec2 vPosition;
in vec2 vVelocity;
in float fSurface;
in vec2 vAinside;
uniform sampler2D pressTex;
uniform sampler2D gravityTex;
uniform float timeStep;
uniform float miu;
uniform float constfn;
out vec2 vPositionFeedback;
out vec2 vVelocityFeedback;
out int inOutFeedback;
void main()
{
	vec2 g = (texture(gravityTex, vPosition)).rg;
	float fn = (texture(pressTex, vPosition)).r;
	float an = fn + constfn + fSurface;
	vPositionFeedback = vPosition;
	vVelocityFeedback = vVelocity;
	if (an < 0)
	{
		inOutFeedback = 0;
	}
	else
	{
		vec2 af = vec2(0.0, 0.0);
		if (an > 7.0)
		{
			an = 7.0;
		}
		if (length(vVelocity) > 0.0)
		{
			af = -miu * an * normalize(vVelocity);
		}
		
		vec2 a = g + af + vAinside;
		//a -= vAinside;
		vVelocityFeedback += timeStep * a;
		vPositionFeedback += timeStep * vVelocity + 0.5 * timeStep * timeStep * a;
		if (vPositionFeedback.x < 1.0 && vPositionFeedback.x > 0.0 
			&& vPositionFeedback.y < 1.0 && vPositionFeedback.y > 0.0)
		{
			inOutFeedback = 1;
		}
		else
			inOutFeedback = 0;
	}
}
);

GLuint compileProgram(const char* vertexSrc, const char* fragmentSrc)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexSrc, 0);
	glShaderSource(fragmentShader, 1, &fragmentSrc, 0);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		char temp[256];
		glGetProgramInfoLog(program, 256, 0, temp);
		printf("Fail to link program:\n%s\n", temp);
		glDeleteProgram(program);
		program = 0;
	}

	return program;
}
