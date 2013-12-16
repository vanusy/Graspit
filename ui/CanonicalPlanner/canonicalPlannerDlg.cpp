/**
 * @file canonicalPlannerDlg.cpp
 * @Authors: A. Huaman
 */


#include "canonicalPlannerDlg.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QValidator>

#include "robot.h" /**< Contains Hand */
#include "gws.h"
#include "mainWindow.h"
#include "graspitGUI.h"
#include "grasp_manager.h"
#include "grasp_tester.h"
#include "grasp_planner.h"
#include "grasp.h"
#include "world.h"
#include "ivmgr.h"
#include "quality.h"
#include "mytools.h"

#include "EGPlanner/searchState.h"

#include "simAnnPlanner.h"
#include "EGPlanner/canonPlanner.h"


/**
 * @function init
 */
void CanonicalPlannerDlg::init() {

}

/**
 * @function destroy
 * @brief Deletes the grasp_manager.
 */
void CanonicalPlannerDlg::destroy()  {

	if (masterFile.isOpen()) masterFile.close();
}


/**
 *  @function exitButton_clicked
 */
void CanonicalPlannerDlg::exitButton_clicked() {
    QDialog::accept();
}

/**
 * @function plannerStart_clicked
 * @brief Make grasps valid, close them and store them
 */
void CanonicalPlannerDlg::plannerStart_clicked() {

	mPlanner->mainLoop();
}

/**
 * @function readFile_clicked()
 */
void CanonicalPlannerDlg::readFile_clicked() {

	mPlanner = new CanonPlanner( mHand );

    QString fn( QFileDialog::getOpenFileName( this,
					      QString(),
					      "/home/ana/Research/Code/manipulation/graspTypes/data",
					      "Grasp files (*.txt)") );
    if( fn.isEmpty() ) { return; }

    mPlanner->readBaseGraspFile( fn.toStdString() );
}


/**
 * @function baseBox_valueChanged
 */
void CanonicalPlannerDlg::baseBox_valueChanged(int _i ) {

	if( mPlanner == NULL ) {
		std::cout << "No planner yet. Load base grasps first."<< std::endl;
		return;
	}

	baseBox->setRange(0, mPlanner->getNumBaseGrasps() - 1 );
	sampleBox->setRange(0, mPlanner->getNumSampleGrasps(baseBox->value()) - 1 );
	mPlanner->showBaseGrasp( baseBox->value() );
	return;
}

/**
 * @function sampleBox_valueChanged
 */
void CanonicalPlannerDlg::sampleBox_valueChanged(int _j ) {

	if( mPlanner == NULL ) {
		std::cout << "No planner yet. Load base grasps first."<< std::endl;
		return;
	}

	sampleBox->setRange(0, mPlanner->getNumSampleGrasps(baseBox->value()) - 1 );
	mPlanner->showSampleGrasp( baseBox->value(), _j );

}


/**
 * @function setMembers
 */
void CanonicalPlannerDlg::setMembers( Hand *_h, GraspableBody *_b ) {

  mPlanner = NULL;
  mHand = _h;
  mObject = _b;
  mHand->getGrasp()->setObjectNoUpdate( mObject );
  mHand->getGrasp()->setGravity( false );

  mHandObjectState = new GraspPlanningState( mHand );
  mHandObjectState->setObject( mObject );
  mHandObjectState->setPositionType( SPACE_AXIS_ANGLE );
  mHandObjectState->setRefTran(mObject->getTran());
  mHandObjectState->reset();

}


