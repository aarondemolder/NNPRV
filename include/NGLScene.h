#ifndef NGLSCENE_H_
#define NGLSCENE_H_

#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Text.h>
#include <QEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>
#include <memory>

class NGLScene : public QOpenGLWidget
{
Q_OBJECT

public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for GLWindow
  /// @param [in] _parent the parent window to create the GL context in
  //----------------------------------------------------------------------------------------------------------------------
  NGLScene(QWidget *_parent);

  /// @brief dtor
  ~NGLScene() override;

public slots:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the X rotation value
  /// @parm[in] _angle the value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setXRotation(int _angle);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the Y rotation value
  /// @parm[in] _angle the value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setYRotation(int _angle);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the Z rotation value
  /// @parm[in] _angle the value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setZRotation(int _angle);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to load a heightmap
  //----------------------------------------------------------------------------------------------------------------------
  void loadHeightMap ();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to load a diffuse map
  //----------------------------------------------------------------------------------------------------------------------
  void loadDiffuse ();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to reload current maps
  //----------------------------------------------------------------------------------------------------------------------
  void reloadMaps ();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the map scale value
  /// @parm[in] _mapScale the value to set
  //----------------------------------------------------------------------------------------------------------------------
  void setMapScale (double _mapScale);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to toggle wireframe mode
  /// @param[in] _mode the mode passed from the togglebutton
  //----------------------------------------------------------------------------------------------------------------------
  void toggleWireframe( bool _mode);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to toggle backface culling mode
  /// @param[in] _mode the mode passed from the togglebutton
  //----------------------------------------------------------------------------------------------------------------------
  void toggleBackfaceCulling ( bool _mode);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to trigger OBJ exporting
  //----------------------------------------------------------------------------------------------------------------------
  void triggerExport();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a slot to trigger art OBJ exporting
  //----------------------------------------------------------------------------------------------------------------------
  void triggerArtExport();

protected:

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called when the window is created
  //----------------------------------------------------------------------------------------------------------------------
  void initializeGL() override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is the main gl drawing routine which is called whenever the window needs to be re-drawn
  //----------------------------------------------------------------------------------------------------------------------
  void paintGL() override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  //----------------------------------------------------------------------------------------------------------------------
  void resizeGL(int _w, int _h) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mousePressEvent(QMouseEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseMoveEvent(QMouseEvent *_event) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time mouse wheel is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void wheelEvent(QWheelEvent *_event)override;
  //----------------------------------------------------------------------------------------------------------------------

  /// @brief our model position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_modelPos = {0.0,0.0,0.0};
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our transform for objects
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Transformation m_transform;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our camera
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_proj;
  ngl::Mat4 m_view;

private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called when we want to regenerate the terrain
  //----------------------------------------------------------------------------------------------------------------------
  void reGenGridPoints();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called when we initially generate the terrain
  //----------------------------------------------------------------------------------------------------------------------
  void genGridPoints();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called when we export a terrain OBJ
  /// @param[in] _fileName the path of the file to export
  //----------------------------------------------------------------------------------------------------------------------
  void objExporter(QString _fileName);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called when we export an art terrain OBJ
  /// @param[in] _fileName the path of the file to export
  //----------------------------------------------------------------------------------------------------------------------
  void artExporter(QString _fileName);
  //----------------------------------------------------------------------------------------------------------------------

  /// @brief wireframe mode
  //----------------------------------------------------------------------------------------------------------------------
  bool m_wireframe;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief backfaceCulling mode
  //----------------------------------------------------------------------------------------------------------------------
  bool m_backfaceCulling;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief filepath for the heightmap image file
  //----------------------------------------------------------------------------------------------------------------------
  QString m_heightMapPath;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief filepath for the diffuse image file
  //----------------------------------------------------------------------------------------------------------------------
  QString m_diffusePath;
  //----------------------------------------------------------------------------------------------------------------------

  /// @brief our Multi View Projection Matrix
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 MVP;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the global mouse transforms
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_mouseGlobalTX;
  //----------------------------------------------------------------------------------------------------------------------

  /// @brief Vector containing the X,Y,Z positions of the terrain gridpoints
  /// as well as the R,G,B colour values
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <ngl::Vec3> gridPoints;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector containing the index values
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <GLuint> indices;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vao created from the image
  //----------------------------------------------------------------------------------------------------------------------
  GLuint m_vaoID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vbo for the grid point data
  //----------------------------------------------------------------------------------------------------------------------
  GLuint vboID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief index buffer object
  //----------------------------------------------------------------------------------------------------------------------
  GLuint iboID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the physical width size of our grid
  //----------------------------------------------------------------------------------------------------------------------
  int gridSize;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief width of our image
  //----------------------------------------------------------------------------------------------------------------------
  int imageSize;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief number of verts to draw
  //----------------------------------------------------------------------------------------------------------------------
  unsigned int m_vertSize;
  //----------------------------------------------------------------------------------------------------------------------

  /// @brief used to store the x rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_xRot;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the y rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_yRot;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the z rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_zRot;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the last mouse click position
  //----------------------------------------------------------------------------------------------------------------------
  QPoint m_lastPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the map scale value
  //----------------------------------------------------------------------------------------------------------------------
  double m_mapScale;

};
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
constexpr float ZOOM = 2.0f;

#endif
