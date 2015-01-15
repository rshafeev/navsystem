uniform vec2 PictSize;
uniform vec2 Tex1Size;
uniform vec2 Tex2Size;
uniform vec2 Tex3Size;


varying vec2 vertex_src;
varying vec2 tex_src1;
varying vec2 tex_src2;
varying vec2 tex_src3;



varying float type;

void main()
{

                vec3 normal, lightDir;
		vec4 diffuse, ambient, globalAmbient;
		float NdotL;
		
		normal = normalize(gl_NormalMatrix * gl_Normal);
		lightDir = normalize(vec3(gl_LightSource[0].position));
		NdotL = max(dot(normal, lightDir), 0.0);
		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		
		ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
		globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
		
		gl_FrontColor =  NdotL * diffuse + globalAmbient + ambient;
		
		gl_Position = ftransform();
        vertex_src      = vec2(gl_Vertex.x / PictSize[0], 1.0-gl_Vertex.z /PictSize[1]);
        
      //  water_src      = vec2(gl_Vertex.x / 512.0,gl_Vertex.z /512.0);
        tex_src1       = vec2(gl_Vertex.x / Tex1Size[0],gl_Vertex.z /Tex1Size[1]);
	tex_src2       = vec2(gl_Vertex.x / Tex2Size[0],gl_Vertex.z /Tex2Size[1]);
	tex_src3       = vec2(gl_Vertex.x / Tex3Size[0],gl_Vertex.z /Tex3Size[1]);
        
        gl_TexCoord[0] = gl_MultiTexCoord0;

}
