uniform float ts;

varying vec4 diffuse, ambient, specular, ecPos;
varying vec3 normal, halfVector;

uniform sampler2D myTex;

void main()
{
   vec3 n, halfV, viewV, lightDir;
   float NdotL, NdotHV;
   vec4 color = ambientGlobal;
   float att, dist;

   /* a fragment shader can't write a verying variable, hence we need
   a new variable to store the normalized interpolated normal */
   n = normalize(gl_NormalMatrix * normal);

   // Compute the ligt direction
   lightDir = vec3(gl_LightSource[0].position - ecPos);

   if (ts)
   {
      float intensity;
      vec4 color;
      intensity = dot(lightDir, normal);

      if (intensity > 0.95)
         color = vec4(1.0, 0.5, 0.5, 1.0);
      else if (intensity > 0.5)
         color = vec4(0.6, 0.3, 0.3, 1.0);
      else if (intensity > 0.25)
         color = vec4(0.4, 0.2, 0.2, 1.0);
      else
         color = vec4(0.2, 0.1, 0.1, 1.0);
      gl_FragColor = color;

   }
   else
   {
      /* compute the distance to the light source to a varying variable*/
      dist = length(lightDir);


      /* compute the dot product between normal and ldir */
      NdotL = max(dot(n, normalize(lightDir)), 0.0);

      if (NdotL > 0.0) {

         att = 1.0 / (gl_LightSource[0].constantAttenuation +
            gl_LightSource[0].linearAttenuation * dist +
            gl_LightSource[0].quadraticAttenuation * dist * dist);
         color += (diffuse * NdotL) + ambient;


         halfV = normalize(halfVector);
         NdotHV = max(dot(n, halfV), 0.0);
         color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess);
      }

      vec4 texColor = texture2D(myTex, gl_TexCoord[0].st);

      gl_FragColor = color + texColor;
   }
}