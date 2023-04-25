#version 430 core
in GS_FS_VERTEX
{
	vec2 texCoord;
	vec3 normal;
	flat bool isGrass;
	vec4 aPos;
} fragment_in;

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform vec3 viewPos;
uniform Light light;

out vec4 fragColor;

uniform sampler2D terrain;
uniform sampler2D detail;
uniform sampler2D grassAlpha;
uniform sampler2D grass;
uniform int mode;

void main()
{
	if (fragment_in.isGrass == false)
	{
		vec3 sampling = mix(texture(terrain,fragment_in.texCoord),texture(detail,fragment_in.texCoord*5.0),0.3).rgb;
		if (mode == 1)
		{
			// ambient
			vec3 ambient = light.ambient * sampling;
			// diffuse
			vec3 lightDir = normalize(-light.direction);
			float diff = max(dot(fragment_in.normal,lightDir),0.0);
			vec3 diffuse = light.diffuse * diff * sampling;
			// specular
			vec3 viewDir = normalize(viewPos - fragment_in.aPos.xyz);
			vec3 halfwayDir = normalize(lightDir + viewDir); // blinn-phong
			float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 64);
			vec3 specular = light.specular * spec * vec3(0.3f,0.3f,0.3f);

			vec3 result = ambient + diffuse + specular;
			fragColor = vec4(result,1.0f);
			if (fragment_in.aPos.y <= -9.0f)
			{
				discard;
			}
		}
		else
			fragColor = vec4(sampling,1.0f);
			if (fragment_in.aPos.y <= -9.0f)
			{
				discard;
			}
	}
	
	else if (fragment_in.isGrass)
	{
		vec4 grassColor = texture(grass,fragment_in.texCoord);
		vec4 alphaMask = texture(grassAlpha,fragment_in.texCoord);
		fragColor = vec4(grassColor.rgb,alphaMask.r);
		if (fragColor.a < 0.1)
			discard;
		else
			fragColor.a = 1.0;
	}
}