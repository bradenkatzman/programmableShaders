varying vec3 color;

void main()
{  
    /* now we can set the color of the fragment --> note RGBA value so 1.0 is supplied for A */
    gl_FragColor = vec4(color, 1.0);
}