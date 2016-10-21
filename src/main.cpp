#include "raytracer.h"

using namespace std;

 bool hitSphere(const ray &r, const sphere &s, float &t) 
 { 
   // intersection rayon/sphere 
   vecteur dist = s.pos - r.start; 
   float B = r.dir * dist;
   float D = B*B - dist * dist + s.size * s.size; 
   if (D < 0.0f) 
     return false; 
   float t0 = B - sqrtf(D); 
   float t1 = B + sqrtf(D);
   bool retvalue = false;  
   if ((t0 > 0.1f) && (t0 < t)) 
   {
     t = t0;
     retvalue = true; 
   } 
   if ((t1 > 0.1f) && (t1 < t)) 
   {
     t = t1; 
     retvalue = true; 
   }
   return retvalue; 
 }

 bool draw(char* outputName, scene &myScene) 
 {
   ofstream imageFile(outputName,ios_base::binary);
   if (!imageFile)
     return false; 
   // Ajout du header TGA
   imageFile.put(0).put(0);
   imageFile.put(2);        /* RGB non compresse */

   imageFile.put(0).put(0);
   imageFile.put(0).put(0);
   imageFile.put(0);

   imageFile.put(0).put(0); /* origine X */ 
   imageFile.put(0).put(0); /* origine Y */

   imageFile.put((myScene.sizex & 0x00FF)).put((myScene.sizex & 0xFF00) / 256);
   imageFile.put((myScene.sizey & 0x00FF)).put((myScene.sizey & 0xFF00) / 256);
   imageFile.put(24);       /* 24 bit bitmap */ 
   imageFile.put(0); 
   // fin du header TGA

   // balayage 
   for (int y = 0; y < myScene.sizey; ++y) { 
   for (int x = 0; x < myScene.sizex; ++x) {
     float red = 0, green = 0, blue = 0;
     float coef = 1.0f;
     int level = 0; 
     // lancer de rayon 
     ray viewRay = { {float(x), float(y), -10000.0f}, { 0.0f, 0.0f, 1.0f}};
     do 
     { 
       // recherche de l'intersection la plus proche
       float t = 20000.0f;
       int currentSphere= -1;

       for (unsigned int i = 0; i < myScene.sphTab.size(); ++i) 
       { 
         if (hitSphere(viewRay, myScene.sphTab[i], t)) 
         {
           currentSphere = i;
         }
       }

       if (currentSphere == -1)
         break;

       point newStart = viewRay.start + t * viewRay.dir; 
       // la normale au point d'intersection 
       vecteur n = newStart - myScene.sphTab[currentSphere].pos;
       float temp = n * n;
       if (temp == 0.0f) 
         break; 

       temp = 1.0f / sqrtf(temp); 
       n = temp * n; 
       
       material currentMat = myScene.matTab[myScene.sphTab[currentSphere].material]; 

       // calcul de la valeur d'éclairement au point 
       for (unsigned int j = 0; j < myScene.lgtTab.size(); ++j) {
         light current = myScene.lgtTab[j];
         vecteur dist = current.pos - newStart;
         if (n * dist <= 0.0f)
           continue;
         float t = sqrtf(dist * dist);
         if ( t <= 0.0f )
           continue;
         ray lightRay;
         lightRay.start = newStart;
         lightRay.dir = (1/t) * dist;
         // calcul des ombres 
         bool inShadow = false; 
         for (unsigned int i = 0; i < myScene.sphTab.size(); ++i) {
           if (hitSphere(lightRay, myScene.sphTab[i], t)) {
             inShadow = true;
             break;
           }
         }
         if (!inShadow) {
           // lambert
           float lambert = (lightRay.dir * n) * coef;
           red += lambert * current.red * currentMat.red;
           green += lambert * current.green * currentMat.green;
           blue += lambert * current.blue * currentMat.blue;
         }
       }
         
       // on itére sur la prochaine reflexion
       coef *= currentMat.reflection;
       float reflet = 2.0f * (viewRay.dir * n);
       viewRay.start = newStart;
       viewRay.dir = viewRay.dir - reflet * n;

       level++;
     } 
     while ((coef > 0.0f) && (level < 10));   

     imageFile.put((unsigned char)min(blue*255.0f,255.0f)).put((unsigned char)min(green*255.0f, 255.0f)).put((unsigned char)min(red*255.0f, 255.0f));
   }
   }
   return true;
 }

 void addMaterial(scene* s,float r,float g,float b,float reflection){
   material a;
   a.red = r;
   a.green = g;
   a.blue = b;
   a.reflection = reflection;
   s->matTab.push_back(a);
 }

 void addSphere(scene* s,float x,float y,float z,float size,int material){
   sphere sb;
   sb.pos.x = x;
   sb.pos.y = y;
   sb.pos.z = z;
   sb.size = size;
   sb.material = material;
   s->sphTab.push_back(sb);
 }

 void addLight(scene* s,float x,float y,float z,float r,float g,float b){
   light la;
   la.pos.x = x;
   la.pos.y = y;
   la.pos.z = z;
   la.red = r;
   la.green = g;
   la.blue = b;
   s->lgtTab.push_back(la);
 }

 int main(int argc, char* argv[]) {
    scene myScene;

   myScene.sizex = 512;
   myScene.sizey = 512;

   addMaterial(&myScene,1.0,1.0,0.0,0.5);
   addMaterial(&myScene,0.0,1.0,1.0,0.5);
   addMaterial(&myScene,1.0,0.0,1.0,0.5);
   
   addSphere(&myScene,233.0,290.0,0.0,100,0);
   addSphere(&myScene,407.0,290.0,0.0,100,1);
   addSphere(&myScene,320.0,140.0,0.0,100,2);

   addLight(&myScene,0.0,240.0,-100.0,1.0,1.0,1.0);
   addLight(&myScene,640.0,240.0,-10000.0,0.6,0.7,1.0);

    if (!draw("test.tga", myScene))
        return -1;
    return 0;
 }