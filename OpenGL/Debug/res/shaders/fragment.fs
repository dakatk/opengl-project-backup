#version 130
in vec2 Texcoord;
in vec3 Normal;
in vec3 WorldPos;
out vec4 FragColor;
const vec3 lightDir = vec3(0.25, 0.25, 0.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float ambIntensity = 0.40;
const float difIntensity = 0.75;
const float specIntensity = 1.0;
const float specPower = 32.0;
uniform sampler2D tex;
void main()
{
	vec4 AmbientColor = vec4(lightColor * ambIntensity, 0.0);
	float DiffuseFactor = dot(normalize(Normal), -lightDir);
	vec4 DiffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 SpecularColor = vec4(0.0, 0.0, 0.0, 1.0);
	if(DiffuseFactor > 0){
		DiffuseColor = vec4(lightColor * difIntensity * DiffuseFactor, 0.0);
		vec3 EyeVertex = normalize(WorldPos);
		vec3 LightReflect = normalize(reflect(lightDir, Normal));
		float SpecularFactor = dot(EyeVertex, LightReflect);
		if(SpecularFactor > 0){
			SpecularFactor = pow(SpecularFactor, specPower);
			SpecularColor = vec4(lightColor * specIntensity * SpecularFactor, 0.0);
		}
	}
	FragColor = texture(tex, Texcoord) * vec4((AmbientColor + DiffuseColor + SpecularColor).xyz, 1.0);
}
