uniform sampler2D colormap;
uniform sampler2D normalmap;
 

void main()
{
	
	vec4 color_1 = texture2D(colormap,gl_TexCoord[0].st);
	vec4 color_2 = texture2D(normalmap,gl_TexCoord[0].st);
			
	gl_FragColor = color_2;
		
}
