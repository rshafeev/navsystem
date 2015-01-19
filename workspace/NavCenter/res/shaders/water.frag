uniform sampler2D texWater;
varying vec2 water_src;

void main()
{

vec4 smesh;
vec3 ColorVec;
       ColorVec = vec3(texture2D(texWater,vec2(water_src)));   
       smesh = vec4(vec3(ColorVec),1.0); 

       if(smesh[0]>1.0) smesh[0]=1.0;
       if(smesh[1]>1.0) smesh[1]=1.0;
       if(smesh[2]>1.0) smesh[2]=1.0;
       smesh=gl_Color*smesh;

       gl_FragColor = smesh;


}