#version 120

uniform float Ka;

uniform float Kd;

uniform float Ks;

varying vec3 v;

varying vec3 N;
uniform vec3 color;

//uniform sampler2D TextureUnit0;

void main(void){


        vec4 Allcolor=vec4(color,1.0);
        vec4 somecolor=vec4(1.0,0.0,0.0,1.0);
        vec3 L = normalize(gl_LightSource[0].position.xyz - v);
        vec3 E = normalize(-v);
        vec3 R = normalize(-reflect(L, N));
        vec4 ambient = gl_FrontLightProduct[0].ambient*Allcolor;
        vec4 diffuse = gl_FrontLightProduct[0].diffuse*max(dot(N, L), 0.0)*Allcolor;
        diffuse = clamp(diffuse, 0.0, 1.0);
        vec4 specular = gl_FrontLightProduct[0].specular*Allcolor*pow(max(dot(R, E), 0.0), 5.0);
        specular = clamp(specular, 0.0, 1.0);
        if (dot(N, L) <= 0){
                specular = vec4(0.0,0.0,0.0,0.0);
        }

        gl_FragColor =  Ka*ambient+Kd*diffuse+Ks*specular;
        //gl_FragColor =somecolor;
}
