uniform sampler2D colormap;
uniform sampler2D normalmap;

void main()
{
	
	
	gl_TexCoord[0] = gl_MultiTexCoord0;;

	gl_Position = ftransform();
}