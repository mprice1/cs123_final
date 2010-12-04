uniform sampler2D colormap;
uniform sampler2D normalmap;

void main()
{
	gl_Position = ftransform();
}