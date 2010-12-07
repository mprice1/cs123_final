uniform sampler2D colormap;
uniform sampler2D normalmap;
uniform vec4 start;
uniform vec4 end;
uniform float sag;
uniform vec3 eyept;

varying float eyedist;
varying vec3 lightoff;
varying vec3 normal;
varying vec3 tan;
varying vec3 bitan;
void main()
{
	normal = normalize(gl_NormalMatrix * gl_Normal);
	tan = normalize(gl_NormalMatrix * vec3(0.0,1.0,0.0));
	bitan = normalize(cross(tan,normal));

	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	float d1 =length(gl_Vertex-start);
	float d2 = length(gl_Vertex-end);
		
	float len = length(start - end);
	d2 = abs(d2-d1);
	float dist = abs(len - d2);

	gl_Position = ftransform();
	
	gl_Position.y += sag * (cosh((d1-len/2.0)/sag) - cosh(-len/2.0/sag));

	lightoff = normalize(gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex );
	eyedist = length(vec3(gl_ModelViewMatrix * gl_Vertex) - eyept);
}

float cosh(float val)
	{
	float tmp = exp(val);
	float cosH = (tmp + 1.0 / tmp) / 2.0;
	return cosH;
	}
