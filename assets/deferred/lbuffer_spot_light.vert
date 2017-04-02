#include "../common/light.glsl"

uniform mat4	ciModelViewProjection;

in vec4			ciPosition;

flat out int	vInstanceId;

void main( void ) 
{
	vInstanceId	= gl_InstanceID;
	SpotLight spotLight = uSpotLights[ vInstanceId ];
	
	vec3 p		= ciPosition.xyz * spotLight.mRadius + spotLight.mPosition;
	gl_Position = ciModelViewProjection * vec4( p, 1.0 );
}
