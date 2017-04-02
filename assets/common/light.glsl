#if !defined ( MAX_NUM_DIRECTIONAL_LIGHTS )
#define MAX_NUM_DIRECTIONAL_LIGHTS 10
#endif

#if !defined ( MAX_NUM_POINT_LIGHTS )
#define MAX_NUM_POINT_LIGHTS 10
#endif

#if !defined ( MAX_NUM_SPOT_LIGHTS )
#define MAX_NUM_SPOT_LIGHTS 10
#endif

 
 /////////////////////Attenuation/////////////

struct Attenuation
{
	float					mConstant;
	float					mLinear;
	float					mExp;
};


//////////////////////Light//////////////////////

struct Light
{
	//ALIGNMENT
	vec3					mColor;
	float					mAmbientIntensity;
	//ALIGNMENT
	float					mDiffuseIntensity;
};

///////////////////////DirectionalLight///////////////////

struct DirectionalLight
{
	//ALIGNMENT
	vec3					mColor;
	float					mAmbientIntensity;
	//ALIGNMENT
	vec3					mDirection;
	float					mDiffuseIntensity;
	//ALIGNMENT

};

///////////////////////PointLight///////////////////

struct PointLight
{
	//ALIGNMENT
	vec3					mColor;
	float					mAmbientIntensity;
	//ALIGNMENT
	vec3					mPosition;
	float					mDiffuseIntensity;
	//ALIGNMENT
	float 					mAttenuationConstant;
	float 					mAttenuationLinear;
	float 					mAttenuationExp;
	float 					mRadius;
	//ALIGNMENT
};

//////////////////////SpotLightData//////////////////////

struct SpotLight
{
	//ALIGNMENT
	vec3					mColor;
	float					mAmbientIntensity;
	//ALIGNMENT
	vec3					mPosition;
	float					mDiffuseIntensity;
	//ALIGNMENT
	float 					mAttenuationConstant;
	float 					mAttenuationLinear;
	float 					mAttenuationExp;
	float 					mRadius;
	//ALIGNMENT
	vec3					mDirection;
	float					mCutoff;
	//ALIGNMENT
};

////////////////////UBO//////////////////////////////


layout (std140) uniform DirectionalLights
{
	DirectionalLight 		uDirectionalLights[ MAX_NUM_DIRECTIONAL_LIGHTS ];
};

layout (std140) uniform PointLights
{
	PointLight 				uPointLights[ MAX_NUM_POINT_LIGHTS ];
};

layout (std140) uniform SpotLights
{
	SpotLight 				uSpotLights[ MAX_NUM_SPOT_LIGHTS ];
};


	
