
//these values will be passed from main
uniform float rColor;
uniform float gColor;
uniform float bColor;

//shine factor
uniform float shine;

//strech factor
uniform float stretchF;

//varying means this will be passed to .frag
varying vec3 color;

// define light position
vec3 lightPosition = vec3(0.0, 0.0, -.5);

//define ambient, diffuse, and specular illuminations
vec3 ambientIll = vec3(0.1, 0.1, 0.1); //light red for ambient color
vec3 diffuseIll = vec3(rColor, gColor, bColor); //red color for diffuse color
vec3 specularIll = vec3(1.0, 1.0, 1.0); //white color for specular reflection

//define ambient, diffure, and specular colors
vec3 ambientColor = vec3(rColor, gColor, bColor); //reddish
vec3 diffuseColor = vec3(rColor, gColor, bColor); //reddish
vec3 specularColor = vec3(1.0, 1.0, 1.0); //white

vec4 stretch = vec4(1.0, stretchF, 1.0, 1.0);
vec3 L; //normalized vector from surface point to light source
vec3 V; //normalized direction from point on surface toward viewer
vec3 R; //normalized direction to which a light ray from the light source would reflect

void main()
{    
    gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex*stretch;
    
    vec3 normalInterpolated = vec3(gl_NormalMatrix*gl_Normal);

    vec3 normal = normalize(normalInterpolated);
    
/* direction of the light by normalizing the difference between the light position and the vertex position */
    L = normalize(lightPosition - gl_Vertex.xyz); 
    
    float lambertian = dot(L, normal); 
    /* calculate the lambertian reflectance*/

    lambertian = max(lambertian, 0.0); /*this clamps the value if it is negative*/
    
    /*set the default specular value to 0*/
    float specular = 0.0;
    
/* a lambertian value greater than 0 implies that this fragment has some
* reflection from our light source
*/
    if (lambertian > 0.0) {
        V = normalize(-gl_Vertex.xyz);
        
/* can use the reflect() method with both L (light direction) and N (normal vector)
* to find R (reflection) */
            R = reflect(-L, normal);
            
/* now we need to compute the specular angle. The dot product of R and the
viewing direction*/
            float specularAngle = dot(R, V);

            specularAngle = max(specularAngle, 0.0);
            
            specular = pow(specularAngle, shine);
        }
    
    /* now we can determine the color using all of our calculated values for Phong * modeling */
    color = ambientColor*ambientIll + (lambertian*(diffuseColor*diffuseIll)) + (specular*(specularColor*specularIll));
}