uniform sampler2D colormap;

void main()
{
	vec4 color_1 = texture2D(colormap,gl_TexCoord[0].st);
	if(color_1.r + color_1.g + color_1.b < 0.35)
	{discard;}
	gl_FragColor = color_1;


}

