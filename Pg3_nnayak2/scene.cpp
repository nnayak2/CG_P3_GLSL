#include "scene.h"
#include <math.h>

scene* scene::sceneinstance = NULL;

scene* scene::getScene()
{
	if (sceneinstance == NULL)
		sceneinstance = new scene;
	return sceneinstance;
}

void scene::loadScene(char *filename)
{
   std::string err = tinyobj::LoadObj(shapes, materials, filename);

   if (!err.empty()) 
   {
      std::cerr << err << std::endl;
      exit(1);
   }
}

void scene::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //Toggle the lights
	if (light) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);

	//Don't apply the transformations to light source
	glPushMatrix();

	glScalef(scale, scale, scale);

	glTranslatef(xx, 0.0f, 0.0f);
	glTranslatef(0.0f, yy, 0.0f);
	glTranslatef(0.0f, 0.0f, zz);

	glRotatef(rot, 1.0f, 1.0f, 1.0f);

	for (std::vector<tinyobj::shape_t>::iterator iter = shapes.begin();
		iter != shapes.end(); iter++)
	{
		for (std::vector<unsigned int>::iterator ind = iter->mesh.indices.begin();
			ind != iter->mesh.indices.end(); ind++)
		{
         if (iter->mesh.material_ids.size() > 0)
         {
            int index = iter->mesh.material_ids[0];
            if (index >= 0)
            {
               GLfloat tri_ambient[] = { (materials[index]).ambient[0], (materials[index]).ambient[1], (materials[index]).ambient[2] };
               GLfloat tri_diffuse[] = { (materials[index]).diffuse[0], (materials[index]).diffuse[1], (materials[index]).diffuse[2] };
               GLfloat tri_specular[] = { (materials[index]).specular[0], (materials[index]).specular[1], (materials[index]).specular[2] };

               glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tri_ambient);
               glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tri_diffuse);
               glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tri_specular);
            }
         }

         bool normalsPresent = 1, texPresent = 1;
         if ((iter->mesh.normals).size() == 0)
            normalsPresent = 0;
         if ((iter->mesh.texcoords).size() == 0 || texNum <= 0)
            texPresent = 0;

         glm::vec3 v1, v2, v3;
         glm::vec3 n1, n2, n3;
         glm::vec2 t1, t2, t3;

			//Read and store individual vertices, normals and tex coords of the triangle
			v1 = glm::vec3((iter->mesh.positions)[*ind * 3], (iter->mesh.positions)[*ind * 3 + 1], (iter->mesh.positions)[*ind * 3 + 2]);
         if (normalsPresent) n1 = glm::vec3((iter->mesh.normals)[*ind * 3], (iter->mesh.normals)[*ind * 3 + 1], (iter->mesh.normals)[*ind * 3 + 2]);
         if (texPresent) t1 = glm::vec2((iter->mesh.texcoords)[*ind * 2], (iter->mesh.texcoords)[*ind * 2 + 1]);
			ind++;
         v2 = glm::vec3((iter->mesh.positions)[*ind * 3], (iter->mesh.positions)[*ind * 3 + 1], (iter->mesh.positions)[*ind * 3 + 2]);
         if (normalsPresent) n2 = glm::vec3((iter->mesh.normals)[*ind * 3], (iter->mesh.normals)[*ind * 3 + 1], (iter->mesh.normals)[*ind * 3 + 2]);
         if (texPresent) t2 = glm::vec2((iter->mesh.texcoords)[*ind * 2], (iter->mesh.texcoords)[*ind * 2 + 1]);
			ind++;
         v3 = glm::vec3((iter->mesh.positions)[*ind * 3], (iter->mesh.positions)[*ind * 3 + 1], (iter->mesh.positions)[*ind * 3 + 2]);
         if (normalsPresent) n3 = glm::vec3((iter->mesh.normals)[*ind * 3], (iter->mesh.normals)[*ind * 3 + 1], (iter->mesh.normals)[*ind * 3 + 2]);
         if (texPresent) t3 = glm::vec2((iter->mesh.texcoords)[*ind * 2], (iter->mesh.texcoords)[*ind * 2 + 1]);

         //try some default values for normals and textures since it wasn't present in obj
			if(!normalsPresent) 
            n1 = n2 = n3 = -glm::normalize(glm::cross((v3 - v1), (v2 - v1)));
         if (!texPresent)
         {
            t1 = glm::vec2(0, 0);
            t2 = glm::vec2(0, 1);
            t3 = glm::vec2(1, 0);
         }

			glBegin(GL_TRIANGLES);

			glNormal3f(n1.x,n1.y,n1.z);
			glTexCoord2f(t1.x,t1.y);
			glVertex3f(v1.x,v1.y,v1.z);

			glNormal3f(n2.x,n2.y,n2.z);
			glTexCoord2f(t2.x,t2.y);
			glVertex3f(v2.x,v2.y,v2.z);

			glNormal3f(n3.x,n3.y,n3.z);
			glTexCoord2f(t3.x,t3.y);
			glVertex3f(v3.x,v3.y,v3.z);

			glEnd();
		}
	}

	//Dont apply transformations to lights
	glPopMatrix();
}

void scene::loadTextures()
{
   glEnable(GL_TEXTURE_2D);
   std::string basepath = "./inputs/";
   printf("How many textures do you want to load ?\n");
   scanf("%d", &texNum);
   for (int i = 0; i < texNum; i++)
   {
      std::string texName;
      printf("Enter texture %d name: ", i + 1);
      std::cin >> texName;
      texNames.push_back(texName);
      texName = basepath + texName;
      GLuint tex2d = SOIL_load_OGL_texture(texName.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
      if (tex2d == 0)
         printf("Loading texture failed \n");
      textures.push_back(tex2d);
   }
   if (texNum > 0)
      glBindTexture(GL_TEXTURE_2D, textures[0]);
}

void scene::setupLights()
{
   //enable light
   glEnable(GL_LIGHTING);
   //Base pointer for first light in GL
   float light = GL_LIGHT0;
   for (std::vector<lightSources>::iterator it = lights.begin(); it != lights.end(); it++)
   {
      GLfloat LightAmbient[] = { it->amb.x, it->amb.y, it->amb.z, 1.0f };
      GLfloat LightDiffuse[] = { it->dif.x, it->dif.y, it->dif.z, 1.0f };
      GLfloat LightSpecular[] = { it->spec.x, it->spec.y, it->spec.z, 1.0f };
      GLfloat LightPosition[] = { it->pos.x, it->pos.y, it->pos.z, 1.0f };

      glLightfv(light, GL_AMBIENT, LightAmbient);
      glLightfv(light, GL_DIFFUSE, LightDiffuse);
      glLightfv(light, GL_SPECULAR, LightSpecular);
      glLightfv(light, GL_POSITION, LightPosition);
      glEnable(light);
      light++;
   }
}