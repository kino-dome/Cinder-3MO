precision mediump float; 

uniform int 	uMode;

in vec3 		vNormalInterp;
in vec3 		vPosition;

out vec4		oColor;



const vec3 lightPos = vec3(1.0,1.0,1.0);
const vec3 ambientColor = vec3(0.3, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main() {
    vec3 normal = normalize(normalInterp);
    vec3 lightDir = normalize(lightPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(-vertPos);

    float lambertian = max(dot(lightDir,normal), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0) {
       float specAngle = max(dot(reflectDir, viewDir), 0.0);
       specular = pow(specAngle, 4.0);
    }
    
    // all
    if(mode == 1) oColor = vec4(ambientColor + lambertian*diffuseColor + specular*specColor, 1.0);

    // only ambient
    if(mode == 2) oColor = vec4(ambientColor, 1.0);
    // only diffuse
    if(mode == 3) oColor = vec4(lambertian*diffuseColor, 1.0);
    // only specular
    if(mode == 4) oColor = vec4(specular*specColor, 1.0);

}
