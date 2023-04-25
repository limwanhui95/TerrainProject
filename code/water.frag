#version 430 core
struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 fragColor;

in GS_FS_VERTEX
{
	vec2 texCoord;
	vec3 aNormal;
	vec3 fragPos;
} fragment_in;


uniform sampler2D water;
uniform vec2 movement;
uniform vec3 viewPos;
uniform Light light;
uniform int mode;

void main()
{
	if (mode == 1)
	{
		// sampling result
		vec3 sampling = texture(water,fragment_in.texCoord+movement).rgb;
		// ambient
		vec3 ambient = light.ambient * sampling;
		// diffuse
		vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(fragment_in.aNormal,lightDir),0.0);
		vec3 diffuse = light.diffuse * diff * sampling;
		// specular 
		vec3 viewDir = normalize(viewPos - fragment_in.fragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir); // blinn-phong
		float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 64);
		vec3 specular = light.specular * spec * vec3(0.3f,0.3f,0.6f);
		//vec3 specular = light.specular * spec * sampling;

		vec3 result = ambient + diffuse + specular;
		fragColor  =  vec4(result,0.45f);
	}
	else if (mode == 0)
	{
		vec3 sampling = texture(water,fragment_in.texCoord+movement).rgb;
		fragColor  =  vec4(sampling,0.45f);
	}
}
