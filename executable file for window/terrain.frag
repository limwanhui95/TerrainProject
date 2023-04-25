#version 430 core
in vec2 texCoord;
in vec4 aPos;
in vec3 aNormal;
out vec4 fragColor;

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform vec3 viewPos;
uniform Light light;

uniform sampler2D terrain;
uniform sampler2D detail;

uniform float flag;
uniform int mode;


void main()
{
	// sampling result
	vec3 sampling = mix(texture(terrain,texCoord),texture(detail,texCoord*5.0),0.3).rgb;
	if (mode == 1)
	{
		// ambient
		vec3 ambient = light.ambient * sampling;
		// diffuse
		vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(aNormal,lightDir),0.0);
		vec3 diffuse = light.diffuse * diff * sampling;
		// specular
		vec3 viewDir = normalize(viewPos - aPos.xyz);
		vec3 halfwayDir = normalize(lightDir + viewDir); // blinn-phong
		float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 64);
		vec3 specular = light.specular * spec * vec3(0.3f,0.3f,0.3f);

		vec3 result = ambient + diffuse + specular;
		fragColor = vec4(result,1.0f);
	}
	else
		fragColor = vec4(sampling,1.0f);
	if (flag > 0.0)
	{
		if(aPos.y <= -9.0f)
		{
			discard;
		}
	}
	else if (flag < 0.0)
	{
		if(aPos.y >= -9.0f)
		{
			discard;
		}
	}
}