#include "../../common/material.glsl"

in Vertex
{
	vec4 mPosition;
	vec3 mColor;
	vec3 mNormal;
	vec2 mTexCoord;
} vertex;

uniform sampler2D uDiffuseMap;
uniform int uHasDiffuseMap;

flat in int vInstanceMaterialId;

out vec4 oColor;

void main( void )
{
	
	 oColor = vec4(vertex.mColor * uClassicMaterials[vInstanceMaterialId].mDiffuseColor, 1.0);
	 if (uHasDiffuseMap==1) {
	 	oColor *= vec4(texture(uDiffuseMap, vertex.mTexCoord).rgb, 1.0);
	 }
	//oColor = vec4(vertex.mColor, 1.0);

}
 