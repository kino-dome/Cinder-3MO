// #include "instanceData.glsl"

in vec4 		ciPosition;
in vec3 		ciNormal;
in vec4 		ciColor;
in vec2			ciTexCoord0;

uniform mat4	ciModelViewProjection;

in mat4			uInstanceModelMatrix;
in mat3			uInstanceNormalMatrix;
in vec4			uInstanceColor;
in int			uInstanceMaterialId;
out Vertex
{
	vec4 mPosition;
	vec3 mColor;
	vec3 mNormal;
	vec2 mTexCoord;
} vertex;

flat out int vInstanceMaterialId;

void main( void )
{
	vertex.mColor		= ciColor.rgb * uInstanceColor.rgb;
	
	vertex.mTexCoord	= ciTexCoord0;

	vec3 n				= ciNormal;
	vertex.mNormal 		= normalize(uInstanceNormalMatrix * n);


	vec4 p				= ciPosition;
	p					= uInstanceModelMatrix * p;
	vertex.mPosition	= ciModelViewProjection * p;
	gl_Position			= vertex.mPosition;

	vInstanceMaterialId = uInstanceMaterialId;
}
 