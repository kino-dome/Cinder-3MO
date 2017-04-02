#include "../common/unpack.glsl"
#include "../common/offset.glsl"
#include "../common/material.glsl"
#include "../common/light.glsl"


#if defined( HAS_SPECULAR )
	uniform sampler2D uSamplerSpecular;
#endif

#if defined( HAS_EMISSION )
	uniform sampler2D uSamplerEmission;
#endif

#if defined( HAS_OCCLUSION )
	uniform sampler2D uSamplerOcclusion;
#endif


const float kScatter = 0.07;

uniform sampler2D	uSamplerAlbedo;
uniform sampler2D	uSamplerNormal;

flat in int vInstanceId;

layout (location = 0) out vec4 oColor;


// Stack Overflow
// vec3 toEye = normalize(vec3(0.0) - vVaryingPos);    
// vec3 lightRef = normalize(reflect(-light, normal)); 
// float spec = pow(dot(lightRef, toEye), 64.0f);  
// specularColor = vec3(1.0)*max(spec, 0.0); 

void main( void )
{
	DirectionalLight dirLight = uDirectionalLights[ vInstanceId ];

	vec2 uv				= calcTexCoordFromFrag( gl_FragCoord.xy );
	
	vec4 position 		= unpackPosition( uv );

	vec3 L 				= normalize((uActiveCamera.mViewMatrix * vec4(-dirLight.mDirection, 0.0)).xyz);

	vec4 albedo 		= texture( uSamplerAlbedo, uv );
	int materialId		= int( texture( uSamplerMaterial, uv ).r );
	ClassicMaterial material = uClassicMaterials[ materialId ];

	vec3 N 				= texture( uSamplerNormal, uv ).rgb;
	vec3 V 				= normalize( -position.xyz );
	vec3 H 				= normalize( L + V );
	float NdotL 		= max( 0.0, dot( N, L ) );
	float HdotN 		= max( 0.0, dot( H, N ) );
	float Ks		 	= pow( HdotN, material.mShininess );

	vec4 ambientColor = vec4(dirLight.mColor * albedo.rgb * vec3(dirLight.mAmbientIntensity), 0.0f);
    vec4 diffuseColor = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);
    vec4 emissionColor = vec4(0, 0, 0, 0);
    
    if (NdotL > 0) {
        diffuseColor = vec4(dirLight.mColor, 0.0f) * dirLight.mDiffuseIntensity *
            NdotL * albedo;

        if (Ks > 0){
            vec3 matSpecular;
            #if defined( HAS_SPECULAR )
			    matSpecular = texture(uSamplerSpecular, uv).rgb * material.mSpecularIntensity;
		     #else
			    matSpecular = material.mSpecularColor * material.mSpecularIntensity;
		     #endif
            specularColor = vec4(dirLight.mColor * matSpecular * Ks, 0.0f);
        }
        
        vec3 emissionMat;
        #if defined( HAS_EMISSION )
            emissionMat = texture(uSamplerEmission, uv).rgb * material.mEmissionIntensity;
        #else
            emissionMat = material.mEmissionColor * material.mEmissionIntensity;
        #endif
        emissionColor = vec4(emissionMat, 0.0);
    }

    oColor = ambientColor + diffuseColor + specularColor + emissionColor;
    //oColor = vec4(length(dirLight.mDirection));
    //oColor.rgb = L;
    if( oColor.r >0 || oColor.g>0 || oColor.b>0 ) oColor.a = 1.0;
    //oColor = vec4(direction, 1.0);

}
 