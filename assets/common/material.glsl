#if !defined ( MAX_NUM_PBR_MATERIALS )
#define MAX_NUM_PBR_MATERIALS 10
#endif

#if !defined ( MAX_NUM_CLASSIC_MATERIALS )
#define MAX_NUM_CLASSIC_MATERIALS 10
#endif

//////////////////////ClassicMaterial/////////////////

struct ClassicMaterial
{
	//ALIGNMENT
	vec3			mDiffuseColor;
	float			mDiffuseIntensity;
	//ALIGNMENT
	vec3			mSpecularColor;
	float			mSpecularIntensity;
	//ALIGNMENT
	vec3			mEmissionColor;
	float			mEmissionIntensity;
	//ALIGNMENT
	float			mShininess;
	uint			mPad0;
	uint			mPad1;
	uint			mPad2;
	//ALIGNMENT
};

//////////////////////PbrMaterial///////////////////

struct PbrMaterial
{
	//ALIGNMENT
	vec4			mBaseColor;
	//ALIGNMENT
	vec3			mEmissionColor;
	float			mEmissionIntensity;
	//ALIGNMENT
	float			mMetallic;
	float			mRoughness;
	float			mSpecular;
	uint 			mPad0;
};

////////////////////UBO//////////////////////////////
#if defined( HAS_CLASSIC_MATERIALS )
	layout (std140) uniform ClassicMaterials
	{
		ClassicMaterial uClassicMaterials[ MAX_NUM_CLASSIC_MATERIALS ];
	};
#endif

#if defined( HAS_PBR_MATERIALS)
	layout (std140) uniform PbrMaterials
	{
		PbrMaterial 	uPbrMaterials[ MAX_NUM_PBR_MATERIALS ];
	};
#endif

uniform isampler2D uSamplerMaterial;


