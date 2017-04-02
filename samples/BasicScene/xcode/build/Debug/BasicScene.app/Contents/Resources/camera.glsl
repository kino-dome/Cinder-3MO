//////////////////////Camera/////////////////

struct Camera
{
	//ALIGNMENT
	mat4			mViewMatrix;
	//ALIGNMENT
	mat4			mInversedViewMatrix;
	//ALIGNMENT
	mat4			mProjectionMatrix;
	//ALIGNMENT
	mat4			mInversedProjectionMatrix;
	//ALIGNMENT
	vec3			mPosition;
	uint			mPad0;
	//ALIGNMENT
	vec3			mViewDirection;
	uint			mPad1;
	//ALIGNMENT
	vec2			mProjectionParameters;
	float			mFov;
	float			mAspectRatio;
	//ALIGNMENT
};

////////////////////UBO//////////////////////////////
layout (std140) uniform ActiveCamera
{
	Camera 	uActiveCamera;
};

