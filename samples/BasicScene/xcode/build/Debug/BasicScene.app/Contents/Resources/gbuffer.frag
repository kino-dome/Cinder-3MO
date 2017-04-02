#include "../common/material.glsl"

#if defined( HAS_ALBEDO )
	layout(location = ALBEDO_INDEX) out vec4 oAlbedo;
	uniform sampler2D uDiffuseMap;
	uniform int uHasDiffuseMap;
#endif

#if defined( HAS_NORMAL )
	layout(location = NORMAL_INDEX) out vec4 oNormal;
	uniform sampler2D uNormalMap;
	uniform int uHasNormalMap;
#endif

#if defined( HAS_SPECULAR )
	layout(location = SPECULAR_INDEX) out vec4 oSpecular;
	uniform sampler2D uSpecularMap;
	uniform int uHasSpecularMap;
#endif

#if defined( HAS_EMISSION )
	layout(location = EMISSION_INDEX) out vec4 oEmission;
	uniform sampler2D uEmissionMap;
	uniform int uHasEmissionMap;
#endif

#if defined( HAS_OCCLUSION )
	layout(location = OCCLUSION_INDEX) out vec4 oOcclusion;
	uniform sampler2D uOcclusionMap;
	uniform int uHasOcclusionMap;
#endif

#if defined( HAS_MATERIAL_ID)
	layout(location = MATERIAL_ID_INDEX) out ivec4 oMaterialId;
#endif

#if defined( HAS_POSITION )
	layout(location = POSITION_INDEX) out vec4 oPosition;
#endif

#if defined( HAS_TEX_COORD )
	layout(location = TEX_COORD_INDEX) out vec4 oTexCoord;
#endif

#if defined( HAS_INSTANCING )
	flat in int vInstanceMaterialId;
#else
	uniform int uMaterialId;
#endif


in Vertex
{
	vec4 mPosition;
	vec4 mColor;
	vec3 mNormal;
	vec2 mTexCoord;
} vertex;

void main( void )
{
	int materialId;

	#if defined( HAS_INSTANCING )
		materialId = vInstanceMaterialId;
	#else
		materialId = uMaterialId;
	#endif

	#if defined( HAS_ALBEDO )
		oAlbedo 	= vec4(vertex.mColor);
		if (uHasDiffuseMap == 1){
			oAlbedo *= vec4(texture(uDiffuseMap, vertex.mTexCoord));
			//oAlbedo.a = 1.0;
		} 
		#if defined(HAS_CLASSIC_MATERIALS)
			oAlbedo *= vec4(uClassicMaterials[materialId].mDiffuseColor, 1.0);
		#endif
	#endif

	#if defined( HAS_NORMAL )
		oNormal = vec4( vertex.mNormal, 1.0 );
		if (uHasNormalMap == 1){
			oNormal *=  vec4(texture(uNormalMap, vertex.mTexCoord).rgb, 1.0);
		}
	#endif

	#if defined( HAS_SPECULAR )
		oSpecular = vec4( 1.0 );
		if (uHasSpecularMap == 1){
			oSpecular *=  vec4(texture(uSpecularMap, vertex.mTexCoord).rgb, 1.0);
		} 
		#if defined(HAS_CLASSIC_MATERIALS)
			oSpecular *= vec4(uClassicMaterials[materialId].mSpecularColor * uClassicMaterials[materialId].mSpecularIntensity, 1.0);
		#endif
	#endif

	#if defined( HAS_EMISSION )
		oEmission = vec4(1.0);
		if (uHasEmissionMap == 1){
			oEmission *= vec4(texture(uEmissionMap, vertex.mTexCoord).rgb, 1.0);
		}
		#if defined(HAS_CLASSIC_MATERIALS)
			oEmission *= vec4(uClassicMaterials[materialId].mEmissionColor * uClassicMaterials[materialId].mEmissionIntensity, 1.0);
		#endif 
	#endif

	#if defined( HAS_OCCLUSION )
		oOcclusion = vec4(1.0);
		if (uHasOcclusionMap == 1){
			oOcclusion *= vec4(texture(uOcclusionMap, vertex.mTexCoord).rgb, 1.0);
		} 	
	#endif

	#if defined( HAS_POSITION )
		oPosition = vertex.mPosition;
	#endif

	#if defined( HAS_TEX_COORD )
		oTexCoord = vec4(vertex.mTexCoord, 0.0, 1.0);
	#endif

	#if defined( HAS_MATERIAL_ID)
		oMaterialId = ivec4(materialId, 0, 0, 255);
	#endif
}
 