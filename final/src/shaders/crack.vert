attribute vec3 intan;

uniform sampler2D colormap;
uniform sampler2D normalmap;



varying vec3 lightoff;
varying vec3 normal;
varying vec3 tan;
varying vec3 bitan;

void main()
{
	normal = normalize(gl_NormalMatrix * gl_Normal);
	tan = normalize(gl_NormalMatrix * intan);
	bitan = normalize(cross(tan,normal));

	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_Position = ftransform();
	lightoff = normalize(gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex );
}
