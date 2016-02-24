#include "Revolution.h"

#include "glsupport.h"


#include "GLRender.h"

#include "Vector3.h"

#include "GLTool.h"


using namespace std;
using namespace p3d;

Revolution::Revolution() {
  // default profile
  _profile.push_back(Vector3(-0.8,0.6,0.0));
  _profile.push_back(Vector3(-0.3,0.2,0.0));
  _profile.push_back(Vector3(-0.9,-0.8,0.0));

}


void Revolution::initSphere() {
  vector<float> p;
  vector<float> n;
  vector<float> t;

  vector<unsigned int> index;

  int nbSlice=20; // include last slice that closes sphere
  int nbStack=20;

  // *******
  //  TODO
    float angleTeta = (2*M_PI) / nbSlice;
    float angleFi = (M_PI) / nbStack;
    for(int i=0; i<=nbStack;i++) {
        for(int j=0;j<nbSlice;j++) {
            p.push_back(cos(angleTeta * j) * sin(angleFi * i));
            p.push_back(cos(angleFi * i));
            p.push_back(sin(angleTeta * j) * sin(angleFi * i));
        }
    }

    /*for(int i = 0;i<nbSlice * nbStack -2;i++){
        index.push_back(i+nbSlice);
        index.push_back(i);
        index.push_back(i+1);
        index.push_back(i+1);
        index.push_back(i+nbSlice+1);
        index.push_back(i+nbSlice);
    }*/


    for(int i=0; i<=nbStack;i++) {
        for(int j=0;j<nbSlice;j++) {
            index.push_back(nbSlice);
            index.push_back(i*j);
            index.push_back(i*j+1);
            index.push_back(nbSlice);
            index.push_back(nbSlice+1);
        }
    }




  // *******




  initVAO(index,p,n,t);

  _texture=&_earth;

}


void Revolution::initCube() {
  vector<float> p;
  vector<float> n;
  vector<float> t;

  vector<unsigned int> index;

  // *******
  //  TODO




  // *******




  initVAO(index,p,n,t);

  _texture=&_dice;
}


void Revolution::initRevolution() {
  if (_profile.size()<2) return; // more than 2 vertices in the profile

  vector<float> p;
  vector<float> n;
  vector<float> t;

  vector<unsigned int> index;

  int nbSlice=40; // include last slice that closes the object
  int nbStack=_profile.size();

  std::vector<Vector3> normalProfile; // to compute normal profile

  // *******
  //  TODO




  // *******





  initVAO(index,p,n,t);
  _texture=&_ul1;
}


void Revolution::init() {
  glGenBuffers(1,&_vbo);
  glGenBuffers(1,&_elementBuffer);
  glGenVertexArrays(1,&_vao);

  _earth.readMipmap("EarthDay512.jpg");
  _dice.readMipmap("dice.png");
  _ul1.readMipmap("UL1-IEEA.png");

}


void Revolution::draw(EShader mode) {
//  return;
  glBindVertexArray(_vao);


  p3d::material(Vector4(0,0,0,1),Vector3(0,0.8,0.2),Vector3(0,0.2,0.8),100);
  p3d::diffuseBackColor=Vector3(0.8,0,0);
  if (mode==Shader_Ambient) {
    p3d::ambientColor=Vector4(0.0,0.8,0,1);
    p3d::ambientBackColor=Vector4(0.8,0.0,0,1);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }

  _texture->bind(0);

  switch(mode) {
  case Shader_Ambient:p3d::shaderVertexAmbient();break;
  case Shader_Light:p3d::shaderLightPhong();break;
  case Shader_Texture:p3d::shaderTextureLight();break;
  }


  glDrawElements(GL_TRIANGLES,_nbElement,GL_UNSIGNED_INT,0);



  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBindVertexArray(0);

}


// Caution : position should be a (x,y,z) array, normal (x,y,z) array, texCoord (x,y) array. lengths should be coherent
void Revolution::initVAO(const std::vector<unsigned int> &index,const std::vector<float> &position,const std::vector<float> &normal,const std::vector<float> &texCoord) {

  vector<float> attrib;
  attrib.resize(position.size()*8);
  for(unsigned int i=0;i<position.size()/3;++i) {
    attrib[i*8+0]=position[i*3+0];
    attrib[i*8+1]=position[i*3+1];
    attrib[i*8+2]=position[i*3+2];

    if (!normal.empty()) {
      attrib[i*8+3]=normal[i*3+0];
      attrib[i*8+4]=normal[i*3+1];
      attrib[i*8+5]=normal[i*3+2];
    }

    if (!texCoord.empty()) {
      attrib[i*8+6]=texCoord[i*2+0];
      attrib[i*8+7]=texCoord[i*2+1];
    }
  }



  glBindBuffer(GL_ARRAY_BUFFER,_vbo);
  glBufferData(GL_ARRAY_BUFFER,attrib.size()*sizeof(float),attrib.data(),GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_elementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,index.size()*sizeof(unsigned int),index.data(),GL_STATIC_DRAW);
  _nbElement=index.size();

  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER,_vbo);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);                                   // position
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(const GLvoid *)(3*sizeof(float)));   // normal
  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(const GLvoid *)(6*sizeof(float)));   // texCoord

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_elementBuffer);

  glBindVertexArray(0);

}

void Revolution::drawProfile() {
  p3d::ambientColor=Vector4(1,0,0,1);
  p3d::shaderVertexAmbient();
  p3d::drawLineStrip(_profile);
  p3d::drawPoints(_profile);

  vector<Vector3> mirror;
  for(auto &p:_profile) {
    mirror.push_back(Vector3(-p.x(),p.y(),p.z()));
  }

  p3d::uniformAmbient(Vector4(0,0,0,1));
  p3d::drawLineStrip(mirror);
  p3d::drawPoints(mirror);

  p3d::uniformAmbient(Vector4(0,1,0,1));
  p3d::drawLines({Vector3(0,-1,0),Vector3(0,1,0)});



}



