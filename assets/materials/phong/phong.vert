uniform mat3	uNormalMat;

uniform mat4	ciModelView;

uniform mat4	ciModelViewProjection;

in vec4			ciPosition;

in vec3			ciNormal;

out vec3		vNormalInterp;
out vec3		vPosition;

void main( void ) 
{	
	vec4 vertPos4		= ciModelView * ciPosition;
	vPosition = vec3(vertPos4) / vertPos4.w;
	
	vNormalInterp = uNormalMat * ciNormal;
	gl_Position = ciModelViewProjection * ciPosition;
}