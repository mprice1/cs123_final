uniform sampler2D colormap;
uniform sampler2D normalmap;

varying vec3 normal;
varying vec3 tan;
varying vec3 bitan;

varying vec3 lightoff;
varying float eyedist;

void main()
{
	
	vec4 color_1 = texture2D(colormap,gl_TexCoord[0].st);
	vec4 color_2 = texture2D(normalmap,gl_TexCoord[0].st);

	vec3 mappednorm;
	mappednorm.xyz = color_2.rgb;

	vec3 N = normalize((tan * mappednorm.x) + (bitan * mappednorm.y) + (normal*mappednorm.z));
	
	float lightIntensity = dot(lightoff,N);
	vec3 col;
	col.rgb = (lightIntensity * color_1 * vec3(1.0,1.0,0.7)).rgb;
	gl_FragColor.rgb = mix(col,vec3(0.8,0.8,0.8), clamp(eyedist/10.0, 0.0, 1.0) );



}
