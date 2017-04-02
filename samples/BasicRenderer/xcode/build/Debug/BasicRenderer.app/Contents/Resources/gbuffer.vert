uniform sampler2D uDisplacementMap;
uniform int 	uHasDisplacementMap;

uniform mat4	ciModelViewProjection;
uniform mat3	ciNormalMatrix;

in vec4 		ciPosition;
in vec3 		ciNormal;
in vec4 		ciColor;
in vec2			ciTexCoord0;

#if defined( HAS_INSTANCING )
	in mat4			uInstanceModelMatrix;
	in mat3			uInstanceNormalMatrix;
	in vec4			uInstanceColor;
	in int			uInstanceMaterialId;
	flat out int    vInstanceMaterialId;
#endif

#if defined( HAS_QUAD_INSTANCING )
	in vec2			uInstanceTexCoordOffset;
	in vec2			uInstanceTexCoordSize;
#endif

out Vertex
{
	vec4 mPosition;
	vec4 mColor;
	vec3 mNormal;
	vec2 mTexCoord;
} vertex;

void main( void )
{
	vertex.mColor		= ciColor;
	#if defined( HAS_INSTANCING )
		vertex.mColor *= uInstanceColor;
		vInstanceMaterialId = uInstanceMaterialId;
	#endif
	
	vertex.mTexCoord	= ciTexCoord0;

	mat3 normalMatrix;
	#if defined( HAS_INSTANCING )
		normalMatrix = uInstanceNormalMatrix;
	#else
		normalMatrix = ciNormalMatrix;
	#endif
	vec3 n				= ciNormal;
	vertex.mNormal 		= normalize(normalMatrix * n);


	vec4 p				= ciPosition;
	#if defined( HAS_INSTANCING )
		p 				= uInstanceModelMatrix * p;
	#endif
	if (uHasDisplacementMap == 1){
		float height = texture(uDisplacementMap, vertex.mTexCoord).r;
		p += vec4(vertex.mNormal * height, 1.0);
	}

	#if defined( HAS_QUAD_INSTANCING )
		vertex.mTexCoord *= uInstanceTexCoordSize;
		vertex.mTexCoord += uInstanceTexCoordOffset;
		vertex.mColor = vec4(1);
	#endif

	vertex.mPosition	= ciModelViewProjection * p;
	gl_Position			= vertex.mPosition;
}
 