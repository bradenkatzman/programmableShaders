/*3 component vector for x, y, z coordinates of vertex position and normal
 * attribute denotes parameters passed to this shader which will be provided PER vertex i.e. many times during a rendering call
 */
attribute vec3 position;
attribute vec3 normal;

/*4x4 matrices for projection, modelview, and normal
 * uniform denotes parameters that are passed to this shader program. these will not change from one execution of this shader to the next within the same rendering call
 */
uniform mat4 projection, modelview, normalMatrix;

/* global variables to be defined and passed to frag shader
* varying denotes parameters that provide an interface between vert and frag shader i.e. vert shader passes these variables to frag shader */
varying vec3 vertexPosition
varying vec3 normalInterpolated;

void main()
{
    // homogenize vertex position using vec4 constructor with w=1.0
    vec4 inputPositionH(position, 1.0);
    
    //position vector with model view -- homogenized
    vec4 vertexPositionH = modelview*inputPositionH;
    
    // calculate position with projection, modelview and homogenized coordinate for vector
    gl_Position = projection*modelview*inputPositionH;
    
    /*calculate interpolated normal vector
    * dehomogenize the vector from construction vec3 from the normal matrix with a w=0.0 value
    */
    vec4 normalDH(normal*, 0.0);
    normalInterpolated = vec3(normalMatrix*normalDH);
}
