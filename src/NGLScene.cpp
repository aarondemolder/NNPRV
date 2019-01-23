#include "NGLScene.h"
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <QFileDialog>
#include <ngl/VAOFactory.h>
#include <fstream>


NGLScene::NGLScene( QWidget *_parent ) : QOpenGLWidget( _parent )
{

	m_wireframe=false;
    m_backfaceCulling=false;
    m_mapScale=8.0;
    m_heightMapPath = "../node_noise/previews/preview_height.pre";
    m_diffusePath = "../node_noise/previews/preview_diffuse.pre";

}


void NGLScene::objExporter(QString _fileName)
{
    //little cheat so I don't have to rearrange gridPoint counter
    //Uses the colour channels to generate the vertex points for the obj writer
    //But requires we input the actual heightmap values into gridPoints where colour values would normally live
    m_diffusePath = "../node_noise/previews/preview_height.pre";
    reGenGridPoints();

    std::string utf8_text = _fileName.toUtf8().constData();

    std::ofstream myfile;
    myfile.open (utf8_text, std::ofstream::out | std::ofstream::trunc);

    myfile << "#Generated by nodenoise previewer\n";

    //write vertices
    for(int i=0; i<=gridPoints.size()-4; i+=4)
    {
        myfile <<"v "<<gridPoints[i].m_x<<" "<<(gridPoints[i+1].m_y)*m_mapScale<<" "<<gridPoints[i+2].m_z<<"\n";
    }

    //write faces
    for(int i=0; i<=gridPoints.size()-4; i++)
    {
        if (i % 512 == 0)
        {
            //do nothing, prevents writing faces where restartFlag equivalent is hit
        }
        else
        {
            myfile <<"f "<<i<<" "<<i+512<<" "<<i+1<<"\n";
            myfile <<"f "<<i+1<<" "<<i+512<<" "<<i+513<<"\n";
        }

    }

    myfile.close();

    std::cout<<"Terrain OBJ Saved\n";

    //end of cheat
    m_diffusePath = "../node_noise/previews/preview_diffuse.pre";
    reGenGridPoints();
}

void NGLScene::artExporter(QString _fileName)
{

    //little cheat so I don't have to rearrange gridPoint counter
    m_diffusePath = "../node_noise/previews/preview_height.pre";
    reGenGridPoints();

    std::string utf8_text = _fileName.toUtf8().constData();

    std::ofstream myfile;
    myfile.open (utf8_text, std::ofstream::out | std::ofstream::trunc);

    myfile << "#Generated by nodenoise previewer\n";

    for(int i=0; i<=gridPoints.size(); i+=4)
    {
        myfile <<"v "<<gridPoints[i].m_x<<" "<<(gridPoints[i+1].m_y)*m_mapScale<<" "<<gridPoints[i+2].m_z<<"\n";
    }

    for(int i=0; i<=gridPoints.size(); i++)
    {
       myfile <<"f "<<i<<" "<<i+1<<" "<<i+2<<"\n";
    }

    myfile.close();

    std::cout<<"Art File Saved\n";

    //end of cheat
    m_diffusePath = "../node_noise/previews/preview_diffuse.pre";
    reGenGridPoints();
}

void NGLScene::reGenGridPoints()
{
    indices.clear();
    gridPoints.clear();

    QImage imageTerrain(m_heightMapPath);
    QImage imageSurface(m_diffusePath);

    imageSize=imageTerrain.size().width()-1;

    //width step and depth steps
    float wStep=static_cast<float>(gridSize)/imageSize;
    float dStep=static_cast<float>(gridSize)/imageSize;

    //grid offset
    float xPos=-(gridSize/2.0f);
    float zPos=-(gridSize/2.0f);

    //generate points
    for(int z=0; z<=imageSize; ++z)
    {
      for(int x=0; x<=imageSize; ++x)
      {
        //get pixel values
        QColor c(imageTerrain.pixel(x,z));
        QColor s(imageSurface.pixel(x,z));

        //points
        gridPoints.push_back(ngl::Vec3(xPos,c.redF()*m_mapScale,zPos));
        //colours
        gridPoints.push_back(ngl::Vec3(s.redF(),s.greenF(),s.blueF()));

        //next position
        xPos+=wStep;
      }
      //next z row
      zPos+=dStep;
      //reset the xpos
      xPos=-(gridSize/2.0f);
    }

  //indicate finished row
  GLuint restartFlag=imageSize*imageSize+9999u;

  for(int z=0; z<imageSize; ++z)
  {
    for(int x=0; x<imageSize; ++x)
    {
      //Vertex in actual row
      indices.push_back(z  * (imageSize+1) + x);
      //Vertex row below
      indices.push_back((z + 1) * (imageSize+1) + x);

    }
    //signal row restart
    indices.push_back(restartFlag);

  }


  glBindVertexArray(m_vaoID);

  //bind grid point data, replace buffer data (only works if buffers are same size!)
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER,gridPoints.size()*sizeof(ngl::Vec3),&gridPoints[0].m_x,GL_DYNAMIC_DRAW);

  //bind index values, replace buffer data (only works if buffers are same size!)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint),&indices[0], GL_DYNAMIC_DRAW);

  //setup attribute pointers
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ngl::Vec3)*2,0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(ngl::Vec3)*2,((float *)NULL + (3)));
  glEnableVertexAttribArray(1);

  //now we tell gl to restart the triangle strip when restartFlag is encountered
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(restartFlag);
  glBindVertexArray(0);

  //store the new indices for re-drawing
  m_vertSize=indices.size();

}

void NGLScene::genGridPoints()
{

    QImage imageTerrain(m_heightMapPath);
    QImage imageSurface(m_diffusePath);

    imageSize=imageTerrain.size().width()-1;

    //width step and depth steps
    float wStep=static_cast<float>(gridSize)/imageSize;
    float dStep=static_cast<float>(gridSize)/imageSize;

    //now we assume that the grid is centered at 0,0,0 so we make
    //it flow from -w/2 -d/2
    float xPos=-(gridSize/2.0f);
    float zPos=-(gridSize/2.0f);

    //now loop from top left to bottom right and generate points
    for(int z=0; z<=imageSize; ++z)
    {
      for(int x=0; x<=imageSize; ++x)
      {
        //get pixel values
        QColor c(imageTerrain.pixel(x,z));
        QColor s(imageSurface.pixel(x,z));

        //this value adjusts scale factor of model, 8 is probably good
        gridPoints.push_back(ngl::Vec3(xPos,c.redF()*m_mapScale,zPos));
        //now store the colour as well
        gridPoints.push_back(ngl::Vec3(s.redF(),s.greenF(),s.blueF()));
        //calculate the new position
        xPos+=wStep;
      }
      //now increment to next z row
      zPos+=dStep;
      //reset the xpos for new row
      xPos=-(gridSize/2.0f);
    }

    //indicate finished row
    GLuint restartFlag=imageSize*imageSize+9999u;

  for(int z=0; z<imageSize; ++z)
  {
    for(int x=0; x<imageSize; ++x)
    {
      //Vertex in actual row
      indices.push_back(z  * (imageSize+1) + x);
      //Vertex row below
      indices.push_back((z + 1) * (imageSize+1) + x);

    }
    //now we have a row of tri strips signal a re-start
    indices.push_back(restartFlag);
  }

  glGenVertexArrays(1, &m_vaoID);
  glBindVertexArray(m_vaoID);

  //grid point data
  glGenBuffers(1, &vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER,gridPoints.size()*sizeof(ngl::Vec3),&gridPoints[0].m_x,GL_DYNAMIC_DRAW);

  //index values
  glGenBuffers(1, &iboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint),&indices[0], GL_DYNAMIC_DRAW);

  //setup attribute pointers
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ngl::Vec3)*2,0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(ngl::Vec3)*2,((float *)NULL + (3)));
  glEnableVertexAttribArray(1);

  //now we tell gl to restart the triangle strip when restartFlag is encountered
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(restartFlag);
  glBindVertexArray(0);

  //store the indices for re-drawing
  m_vertSize=indices.size();

}


void NGLScene::initializeGL()
{
    ngl::NGLInit::instance();
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    //create camera
    ngl::Vec3 from(0,0,100);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);

    m_world=ngl::lookAt(from,to,up);
    m_proj=ngl::perspective(45,720.0f/576.0f,0.001f,150);

    //grab an instance of shader manager
    ngl::ShaderLib* shader = ngl::ShaderLib::instance();

    //load a frag and vert shaders
    shader->createShaderProgram("ColourShader");

    shader->attachShader("ColourVertex",ngl::ShaderType::VERTEX);
    shader->attachShader("ColourFragment",ngl::ShaderType::FRAGMENT);
    shader->loadShaderSource("ColourVertex","shaders/ColourVert.glsl");
    shader->loadShaderSource("ColourFragment","shaders/ColourFrag.glsl");

    shader->compileShader("ColourVertex");
    shader->compileShader("ColourFragment");
    shader->attachShaderToProgram("ColourShader","ColourVertex");
    shader->attachShaderToProgram("ColourShader","ColourFragment");

    shader->linkProgramObject("ColourShader");
    shader->autoRegisterUniforms("ColourShader");
    (*shader)["ColourShader"]->use();

    //first run
    genGridPoints();

}


void NGLScene::resizeGL(int w, int h)
{
    m_proj=ngl::perspective( 45.0f, static_cast<float>( w ) / h, 0.05f, 350.0f );
}


void NGLScene::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(m_backfaceCulling == true)
    {
        glEnable(GL_CULL_FACE);
    }
    if(m_backfaceCulling == false)
    {
        glDisable(GL_CULL_FACE);
    }

    if(m_wireframe == true)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }

    if (m_wireframe == false)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

    //Rotation based on the mouse position for our global transform
    ngl::Mat4 rotX;
    ngl::Mat4 rotY;

    //create the rotation matrices
    rotX.rotateX(180.0f - (m_xRot / 16.0f));
    rotY.rotateY(m_yRot / 16.0f);

    //multiply the rotations
    m_mouseGlobalTX=rotY*rotX;

    //add the translations
    m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
    m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
    m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

    MVP=m_proj*m_world*m_mouseGlobalTX;

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["ColourShader"]->use();

    shader->setUniform("MVP",MVP);

    //bind & draw
    glBindVertexArray(vboID);
    glDrawElements(GL_TRIANGLE_STRIP, m_vertSize,GL_UNSIGNED_INT,0);


}

static void qNormalizeAngle(int &_angle)
{
    while (_angle < 0)
        _angle += 360 * 16;
    while (_angle > 360 * 16)
        _angle -= 360 * 16;
}

void NGLScene::setXRotation(int _angle)
{
    qNormalizeAngle(_angle);
    if (_angle != m_xRot) {
        m_xRot = _angle;
        update();
    }
}

void NGLScene::setYRotation(int _angle)
{
    qNormalizeAngle(_angle);
    if (_angle != m_yRot) {
        m_yRot = _angle;
        update();
    }
}

void NGLScene::setZRotation(int _angle)
{
    qNormalizeAngle(_angle);
    if (_angle != m_zRot) {
        m_zRot = _angle;
        update();

    }
}

void NGLScene::loadHeightMap()
{

    QString m_fileName = QFileDialog::getOpenFileName(this,tr("Open Preview file"), "", tr("Preview Files (*.pre)"));

    m_heightMapPath = m_fileName;

    reGenGridPoints();

    update();
}

void NGLScene::loadDiffuse()
{
    QString m_fileName = QFileDialog::getOpenFileName(this,tr("Open Preview file"), "", tr("Preview Files (*.pre)"));

    m_diffusePath = m_fileName;

    reGenGridPoints();

    update();
}

void NGLScene::reloadMaps()
{
    m_heightMapPath = "../node_noise/previews/preview_height.pre";
    m_diffusePath = "../node_noise/previews/preview_diffuse.pre";

    reGenGridPoints();

    update();
}


NGLScene::~NGLScene()
{
}

void NGLScene::toggleWireframe(bool _mode)
{
	m_wireframe=_mode;
	update();
}

void NGLScene::toggleBackfaceCulling(bool _mode	)
{
    m_backfaceCulling=_mode;
    update();
}

void NGLScene::setMapScale(double _mapScale)
{
    m_mapScale = _mapScale;

    reGenGridPoints();

    update();
}

void NGLScene::triggerExport()
{
    QString m_fileName = QFileDialog::getSaveFileName(this,tr("Save Geometry"), "", tr("OBJ Files (*.obj)"));

    objExporter(m_fileName);
}

void NGLScene::triggerArtExport()
{
    QString m_fileName = QFileDialog::getSaveFileName(this,tr("Save Art"), "", tr("OBJ Files (*.obj)"));

    artExporter(m_fileName);
}



void NGLScene::mousePressEvent(QMouseEvent *_event)
{
    m_lastPos = _event->pos();
    update();
}

void NGLScene::mouseMoveEvent( QMouseEvent *_event )
{

    int dx = _event->x() - m_lastPos.x();
    int dy = _event->y() - m_lastPos.y();

    if (_event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (_event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = _event->pos();
    update();

}

void NGLScene::wheelEvent( QWheelEvent *_event )
{
  if ( _event->delta() > 0 )
  {
    m_modelPos.m_z += ZOOM;
  }
  else if ( _event->delta() < 0 )
  {
    m_modelPos.m_z -= ZOOM;
  }
  update();
}