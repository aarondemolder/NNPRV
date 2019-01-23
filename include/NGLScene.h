#ifndef NGLSCENE_H_
#define NGLSCENE_H_

#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Text.h>
#include "WindowParams.h"
#include <QEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>
#include <memory>

constexpr float ZOOM = 2.0f;
constexpr float INCREMENT = 0.01f;

class NGLScene : public QOpenGLWidget
{
Q_OBJECT
public :

  NGLScene(QWidget *_parent );

  ~NGLScene() override;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void loadHeightMap ();
    void loadDiffuse ();
    void reloadMaps ();
    void setMapScale (double _mapScale);
    void toggleWireframe( bool _mode);
    void toggleBackfaceCulling ( bool _mode);
    void triggerExport();
    void triggerArtExport();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event)override;

    ngl::Vec3 m_modelPos;
    ngl::Transformation m_transform;
    ngl::Mat4 m_proj;
    ngl::Mat4 m_camera;
    ngl::Mat4 m_world;

private :

    void reGenGridPoints();
    void genGridPoints();
    void objExporter(QString _fileName);
    void artExporter(QString _fileName);

    bool m_wireframe;
    bool m_backfaceCulling;
    QString m_heightMapPath;
    QString m_diffusePath;

    ngl::Mat4 MVP;
    ngl::Mat4 m_mouseGlobalTX;

    std::vector <ngl::Vec3> gridPoints;
    std::vector <GLuint> indices;
    GLuint m_vaoID;
    GLuint vboID;
    GLuint iboID;
    int gridSize = 100;
    int imageSize;
    unsigned int m_vertSize;

    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;
    double m_mapScale;

};

#endif
