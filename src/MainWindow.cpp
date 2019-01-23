#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);

  m_gl=new  NGLScene(this);

  m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,2,1);

  //display options
  connect(m_ui->m_mapScaler,SIGNAL(valueChanged(double)),m_gl,SLOT(setMapScale(double)));
  connect(m_ui->m_wireframe,SIGNAL(toggled(bool)),m_gl,SLOT(toggleWireframe(bool)));
  connect(m_ui->m_backfaceCulling,SIGNAL(toggled(bool)),m_gl,SLOT(toggleBackfaceCulling(bool)));

  //reload or load other preview files
  connect(m_ui->m_reloadMaps,SIGNAL(clicked()),m_gl,SLOT(reloadMaps()));
  connect(m_ui->m_loadHeightMap,SIGNAL(clicked()),m_gl,SLOT(loadHeightMap()));
  connect(m_ui->m_loadDiffuse,SIGNAL(clicked()),m_gl,SLOT(loadDiffuse()));

  //obj exporter bad idea for the way the heightmap points are generated, this would have to be rewritten ideally - so removed it.
  connect(m_ui->m_objExport,SIGNAL(clicked()),m_gl,SLOT(triggerExport()));
  connect(m_ui->m_artExport,SIGNAL(clicked()),m_gl,SLOT(triggerArtExport()));

}

MainWindow::~MainWindow()
{
    delete m_ui;
}
