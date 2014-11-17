#include "main.h"

//Define all key press handlers here. Declaration of extern in header
int light = 1;
int xx = 0, yy = 0, zz = 0, rot = 0;
float scale = 1.0f;
int activeTex = 0;

glm::vec3 eye, look, up;
float znear, zfar;
int pixwidth, pixheight;
int toonToggle;

std::vector<lightSources> lights;

void Keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 'l':
      if (light) light = 0;
      else light = 1;
      break;

   case 'z':
      scale += 0.1f;
      break;
   case 'x':
      scale -= 0.1f;
      break;

   case '[':
      zz++;
      break;
   case ']':
      zz--;
      break;

   case 'q':
      rot++;
      break;
   case 'w':
      rot--;
      break;

   case ',':
      (activeTex - 1 < 0) ? 0 : activeTex--;
      break;
   case '.':
      (activeTex + 1 >= scene::getScene()->texNum) ? scene::getScene()->texNum - 1 : activeTex++;
      break;
   case 't':
      if (toonToggle) toonToggle = 0;
      else toonToggle = 1;
      break;
   }
}

void SpecialInput(int key, int x, int y)
{
   switch (key)
   {
   case GLUT_KEY_UP:
      yy++;
      break;
   case GLUT_KEY_DOWN:
      yy--;
      break;
   case GLUT_KEY_LEFT:
      xx--;
      break;
   case GLUT_KEY_RIGHT:
      xx++;
      break;
   }
}

void draw()
{
   scene::getScene()->draw();
   glutSwapBuffers();
}

void redraw()
{
   glutPostRedisplay();
}

void handle_menu(int ID)
{
   glBindTexture(GL_TEXTURE_2D, scene::getScene()->textures[ID]);
}


void initialiseGLUT(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(pixwidth, pixheight);
   glutCreateWindow("\t CSC561: Rasterization");

   glShadeModel(GL_SMOOTH);
   glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   if (glewInit() != GLEW_OK)
   {
      printf("problem\n");
   }

   scene *scn = scene::getScene();
   scn->loadTextures();
   scn->setupLights();
   scn->setupShaders();

   glutCreateMenu(handle_menu);	// Setup GLUT popup menu
   int i = 0;
   for (std::vector<std::string>::iterator it = scn->texNames.begin(); it != scn->texNames.end(); it++)
   {
      glutAddMenuEntry((*it).c_str(), i);
      glBindTexture(GL_TEXTURE_2D, scn->textures[i]);
      i++;
   }
   glutAttachMenu(GLUT_RIGHT_BUTTON);

   //Initialize the model view and projection matrices
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45, (float)1, znear, zfar);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, up.x, up.y, up.z);
   //Some initial translation so cube is visible
   glTranslatef(0, 0, 5);

   glutKeyboardFunc(Keyboard);
   glutSpecialFunc(SpecialInput);
   glutDisplayFunc(draw);
   glutIdleFunc(redraw);
   glutMainLoop();
}

int main(int argc, char* argv[])
{
   //Read view and window text files for custom window
   std::ifstream fileIn;
   fileIn.open("./inputs/window.txt");
   if (!fileIn)
   {
      std::cout << "Could not find window.txt file, using assignment defaults." << std::endl;
      pixheight = 512;
      pixwidth = 512;
   }
   else
   {
      while (!fileIn.eof())
      {
         char line[50];
         fileIn.getline(line, 50);
         if (line[0] == 'p')
         {
            char *token = &line[2];
            _parseInt2(pixwidth, pixheight, token);
         }
      }
   }

   std::ifstream fileIn2;
   fileIn2.open("./inputs/view.txt");
   if (!fileIn2)
   {
      std::cout << "Could not find view.txt file, using assignment defaults." << std::endl;
      eye = glm::vec3(0, 0, -2);
      look = glm::vec3(0, 0, 1);
      up = glm::vec3(0, 1, 0);
      znear = 1;
      zfar = 10;
   }
   else
   {
      while (!fileIn2.eof())
      {
         char line[50];
         fileIn2.getline(line, 50);
         if (line[0] == 'e')
         {
            char *token = &line[2];
            _parseFloat3(eye.x, eye.y, eye.z, token);
         }
         if (line[0] == 'l')
         {
            char *token = &line[2];
            _parseFloat3(look.x, look.y, look.z, token);
         }
         if (line[0] == 'u')
         {
            char *token = &line[2];
            _parseFloat3(up.x, up.y, up.z, token);
         }
         if (line[0] == 'n')
         {
            char *token = &line[2];
            znear = _parseInt(token);
         }
         if (line[0] == 'f')
         {
            char *token = &line[2];
            zfar = _parseInt(token);
         }
      }
   }

   //Load the arbitrary lights
   std::ifstream fileIn3;
   fileIn3.open("./inputs/lights.txt");
   if (!fileIn3)
   {
      std::cout << "could not find lights.txt file, using defaults." << std::endl;
      //def light parameters
      lightSources temp;
      temp.pos = glm::vec3(0, 5, 0);
      temp.amb = glm::vec3(0.2, 0.2, 0.2);
      temp.dif = glm::vec3(0.6, 0.6, 0.6);
      temp.spec = glm::vec3(1.0, 1.0, 1.0);
      lights.push_back(temp);
   }
   else
   {
      while (!fileIn3.eof())
      {
         char line[50];
         fileIn3.getline(line, 50);
         lightSources temp;
         if (line[0] == 'p')
         {
            char *token = &line[2];
            _parseFloat3(temp.pos.x, temp.pos.y, temp.pos.z, token);

            for (int i = 0; i < 4; i++)
            {
               fileIn3.getline(line, 50);
               if (line[0] == 'a')
               {
                  char *token = &line[2];
                  _parseFloat3(temp.amb.x, temp.amb.y, temp.amb.z, token);
               }
               if (line[0] == 'd')
               {
                  char *token = &line[2];
                  _parseFloat3(temp.dif.x, temp.dif.y, temp.dif.z, token);
               }
               if (line[0] == 's')
               {
                  char *token = &line[2];
                  _parseFloat3(temp.spec.x, temp.spec.y, temp.spec.z, token);
               }
            }
            lights.push_back(temp);
         }

      }
   }

   //This class loads the obj file, and does the intersection calculations
   scene *scn = scene::getScene();

   //Get the obj file from user
   //std::string basepath = "./inputs/";
   std::string obj;
   printf("Enter obj file to load or 0 for default: ");
   std::cin >> obj;
   if (obj == "0")
      obj = "cube-textures.obj";
   //obj = basepath + obj;

   //load the obj file
   scn->loadScene(const_cast<char*>(obj.c_str()));

   initialiseGLUT(argc, argv);
}

