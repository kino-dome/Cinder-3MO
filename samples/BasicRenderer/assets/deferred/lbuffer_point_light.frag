#include "../common/unpack.glsl"
#include "../common/offset.glsl"
#include "../common/material.glsl"
#include "../common/light.glsl"
#include "../common/pi.glsl"


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
uniform float   uFar;

flat in int vInstanceId;

layout (location = 0) out vec4 oColor;

// void main( void )
// {
//     vec2 uv             = calcTexCoordFromFrag( gl_FragCoord.xy );

//     PointLight pointLight    = uPointLights[ vInstanceId ];
    
//     vec4 position       = unpackPosition( uv );
    
//     vec3 L              = ( uViewMatrix * vec4( pointLight.mPosition, 1.0 ) ).xyz - position.xyz;
//     float d             = length( L );
//     L                   /= d;
    
//     vec4 albedo         = texture( uSamplerAlbedo, uv );
//     int materialId      = int( texture( uSamplerMaterial, uv ).r );
//     ClassicMaterial material   = uClassicMaterials[ materialId ];

//     vec3 N              = unpackNormal( texture( uSamplerNormal, uv ).rg );
//     vec3 V              = normalize( -position.xyz );
//     vec3 H              = normalize( L + V );
//     float NdotL         = max( 0.0, dot( N, L ) );
//     float HdotN         = max( 0.0, dot( H, N ) );
//     float Ks            = pow( HdotN, material.mShininess );

//     vec4 Ia             = vec4(pointLight.mAmbientIntensity * pointLight.mColor, 1.0);
//     vec4 Id             = vec4(NdotL * pointLight.mColor * albedo.rgb * material.mDiffuseColor * material.mDiffuseIntensity, 1.0);
//     vec4 Ie;
//     #if defined( HAS_EMISSION )
//         Ie = texture(uSamplerEmission, uv).rgb;
//     #else
//         Ie = vec4(material.mEmissionColor * material.mEmissionIntensity, 1.0);
//     #endif
    
//     vec3 matSpecular;
//     #if defined( HAS_SPECULAR )
//         matSpecular = texture(uSamplerSpecular, uv).rgb;
//     #else
//         matSpecular = material.mSpecularColor * material.mSpecularIntensity;
//     #endif
//     vec4 Is             = vec4(Ks * pointLight.mColor * material.mSpecularColor * material.mSpecularIntensity, 1.0);
//     float att           = 1.0 / (pointLight.mAttenuation.mConstant + pointLight.mAttenuation.mLinear * d + pointLight.mAttenuation.mExp * d * d);

//     vec3 dir            = position.xyz;
//     float l             = length( dir );
//     dir                 /= l;
//     vec3 q              = -L;
//     float b             = dot( dir, q );
//     float c             = dot( q, q );
//     float s             = 1.0f / sqrt( c - b * b );
//     s                   = smoothstep( 0.005, 1.0, s * (atan( (s + b) * s) - atan( b * s ) ) );
    
//     oColor              = att * ( Ia * pointLight.mAmbientIntensity + Id * pointLight.mDiffuseIntensity + Is ) ;
//     oColor.rgb          *= ( vec3( 1.0 - kScatter ) + vec3( 2.5 * pointLight.mColor * vec3( s )) * kScatter );
//     oColor.a            = 1.0;
// }


void main( void )
{
	PointLight pointLight = uPointLights[ vInstanceId ];

	vec2 uv				= calcTexCoordFromFrag( gl_FragCoord.xy );
	
	vec4 position 		= unpackPosition( uv );
    vec4 lightViewPos   = uActiveCamera.mViewMatrix * vec4(pointLight.mPosition, 1.0 );
	vec3 L 				= lightViewPos.xyz - position.xyz;
	float dist          = length(L);
    if ( dist > pointLight.mRadius) {
        discard;
    }
    L                   /= dist;
	vec4 albedo 		= texture( uSamplerAlbedo, uv );
	int materialId		= int( texture( uSamplerMaterial, uv ).r );
	ClassicMaterial material = uClassicMaterials[ materialId ];

	vec3 N 				= texture( uSamplerNormal, uv ).rgb;
	vec3 V 				= normalize( -position.xyz );
	vec3 H 				= normalize( L + V );
	float NdotL 		= max( 0.0, dot( N, L ) );
	float HdotN 		= max( 0.0, dot( H, N ) );
	float Ks		 	= pow( HdotN, material.mShininess );

	
	vec4 ambientColor = vec4(pointLight.mColor * albedo.rgb * vec3(pointLight.mAmbientIntensity), 1.0f);
    vec4 diffuseColor = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);
    vec4 emissionColor = vec4(0, 0, 0, 0);

    if (NdotL > 0) {
        diffuseColor = vec4(pointLight.mColor * albedo.rgb * pointLight.mDiffuseIntensity * NdotL, 1.0f);

        if (Ks > 0){
            vec3 matSpecular;
            #if defined( HAS_SPECULAR )
                matSpecular = texture(uSamplerSpecular, uv).rgb;
             #else
                matSpecular = material.mSpecularColor * material.mSpecularIntensity;
             #endif
            specularColor = vec4(pointLight.mColor * matSpecular * Ks, 1.0f);
        }
            
        #if defined( HAS_EMISSION )
            emissionColor = texture(uSamplerEmission, uv).rgb;
        #else
            emissionColor = vec4(material.mEmissionColor * material.mEmissionIntensity, 1.0);
        #endif
    }
    float attenuation = 1.0 / (pointLight.mAttenuationConstant + pointLight.mAttenuationLinear * dist + pointLight.mAttenuationExp * dist * dist);
    //attenuation = 1.0;
    oColor = ambientColor + attenuation * (diffuseColor + specularColor) + emissionColor;
    // oColor = vec4(dist/40.0);
    oColor.a = 1.0;
}
 