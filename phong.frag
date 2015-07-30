/* these variables are passed from the vert shader to this frag shader for computing the color at the current pixel */
varying vec3 vertexPosition;
varying vec3 normalInterpolated;

//COMMENT WHAT THIS DOES
uniform int mode;

//define light position and 3 types of color for Phong model
vec3 lightPosition(0.0, 0.0, -.5);
vec3 ambientColor(0.1, 0.0, 0.0); //light red for ambient color
vec3 diffuseColor(0.5, 0.0, 0.0); //red color for diffuse color
vec3 specularColor(1.0, 1.0, 1.0); //white color for specular reflection

//set the shineness of our scene
float shine = 16.0;

//set the color of our specular lighting effect

void main()
{
    //let's normalize our interpolated normal vector first using OpenGL's normalize method
    vec3 normal = normalize(normalInterpolated);
    
    /* we can calculate the direction of the light by normalizing the difference between the light
     * position and the vertex position
     */
    vec3 lightDirection = normalize(lightPosition - vertexPosition);
    
    /* next we need to calculate the lambertian reflectance:
     * this number defines the diffusely reflecting surface. 
     * Note that the apparent brightness is the same regardless
     * of the observer's angle of view. We can calculate this by
     * taking the dot product of the of the lightDirection and
     * the normal vector. Further, we must clamp this dot product
     * because they may be negative, which we can do with the max() method
     */
    float lambertian = dot(lightDirection, normal); //calculate the lambertian reflectance
    lambertian = max(lambertian, 0.0); //this clamps the value if it is negative
    
    //set the default specular value to 0
    float specular = 0.0;
    
    /* a lambertian value greater than 0 implies that this fragment has some
     * reflection from our light source
     */
    if (lambertian > 0.0) {
        //calculate V, the normalized direction from the point on the surface towards the viewer
        vec3 viewingDirection = normalize(-vertPos);
        
        if(mode == 2) {
            /* first calculate R, the normalized direction to which a light ray
             * from the light source to this surface point would reflect. We can
             * use the reflect() method with both L (light direction) and N (normal vector)
             * to find R (reflection)
             */
            vec3 R = reflect(-lightDirectionn, normal);
            
            /* now we need to compute the specular angle. The dot product of R (reflection) and the
             * viewing direction. Again, this dot product must be campled because it may be negative
             */
            float specularAngle = dot(R, viewingDirection); //calculate specular angle
            specularAngle = max(specularAngle, 0.0); //this clamps the value if it is negative
            
            /* we can now use the specularAngle and our preset shine float to calculate specular
             * in the equation this is (R*V)^shine
             */
            specular = pow(specularAngle, shine); //could divide shine by 4 ***********
        }
    }
    
    // now we can determine the color using all of our calculate values for Phong modeling
    vec3 color = ambientColor + (lambertian*diffuseColor) + (specular*specularColor);
    
    //now we can set the color of the fragment --> note RGBA value so 1.0 is supplied for A
    gl_FragColor = vec4(color, 1.0);
    
}