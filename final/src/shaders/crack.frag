uniform sampler2D colormap;
uniform sampler2D normalmap;

attribute vec3 intan;

varying vec3 lightoff;
varying vec3 normal;
varying vec3 tan;
varying vec3 bitan;

void main()
{
	vec4 color_1 = texture2D(colormap,gl_TexCoord[0].st);
	vec4 color_2 = texture2D(normalmap,gl_TexCoord[0].st);

	vec3 mappednorm;
	mappednorm.xyz = color_2.rgb;

	vec3 N = normalize((tan * mappednorm.x) + (bitan * mappednorm.y) + (normal*mappednorm.z));
	
	float lightIntensity = dot(lightoff,N);
	gl_FragColor.rgb = (lightIntensity * color_1).rgb;
}

