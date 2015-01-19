uniform sampler2D texture;
varying vec2 tex_src;
void main()
{
    gl_FragColor = gl_Color*vec4(vec3(texture2D(texture,vec2(tex_src))),1.0);

}