#version 460 core

layout (points) in;
in float State[];

layout (triangle_strip, max_vertices = 24) out;

out float state;
out float light;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;


const vec3 square[]={
    vec3(-0.5, 0.5,-0.5),
    vec3(-0.5, 0.5, 0.5),
    vec3( 0.5, 0.5,-0.5),
    vec3( 0.5, 0.5, 0.5)
};

const mat3 faces[]={
mat3(1, 0, 0,
     0, 1, 0,
     0, 0, 1),

mat3(0, 1,0,
     0, 0,1,
     1, 0,0),

mat3(0, 0,1,
     1, 0,0,
     0, 1,0),

mat3(1, 0, 0,
     0,-1, 0,
     0, 0, 1),

mat3(0, 1, 0,
     0, 0,-1,
     1, 0, 0),

mat3(0, 0, 1,
    -1, 0, 0,
     0, 1, 0)

};

struct VtxData {
   vec3  pos;
   float state;   
};   

layout (std140, binding = 1) buffer VertexBuffer {
   VtxData verts[];
};


void main(){    
    if(State[0]>0.0){
        for(int face = 0;face<6;face++)
        {
            for(int vertex = 0;vertex<4;vertex++)
            {
                vec4 aPos = gl_in[0].gl_Position+vec4(faces[face]*square[vertex],0);
                gl_Position=projection*view*aPos;
                state=State[0];
                Normal=faces[face]*vec3(0,1,0);
                FragPos = vec3( aPos);

                light=1.0f;
                
                EmitVertex(); 
            }
            EndPrimitive();
        }
    }
}
