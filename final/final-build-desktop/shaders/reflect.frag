uniform samplerCube CubeMap;
varying vec3 normal, lightDir, r;
varying float eyedist;
void main (void)
{
	vec4 final_color = textureCube( CubeMap, r);
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	float lambertTerm = dot(N,L);
	if(lambertTerm > 0.0)
	{
		// Specular
		final_color += textureCube( CubeMap, r);
	}
	//gl_FragColor = final_color ;

	gl_FragColor = mix(final_color,vec4(0.8,0.8,0.8,1), clamp(eyedist/10.0, 0.0, 1.0) );
	//gl_FragColor.r = eyedist/10.0;

}
