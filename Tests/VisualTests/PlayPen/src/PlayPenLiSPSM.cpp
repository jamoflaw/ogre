/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/
Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "PlayPenTests.h"

PlayPen_LiSPSM::PlayPen_LiSPSM()
{
	mInfo["Title"] = "PlayPen_LiSPSM";
	mInfo["Description"] = "Tests.";
	addScreenshotFrame(250);
}
//----------------------------------------------------------------------------

void PlayPen_LiSPSM::cleanupContent()
{
	clearDebugTextureOverlays();
}
//----------------------------------------------------------------------------

void PlayPen_LiSPSM::setupContent()
{
	SceneNode* mTestNode[10];

	mSceneMgr->setShadowTextureSize(1024);
	mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	
	//FocusedShadowCameraSetup* lispsmSetup = new FocusedShadowCameraSetup();
	LiSPSMShadowCameraSetup* lispsmSetup = new LiSPSMShadowCameraSetup();
	lispsmSetup->setOptimalAdjustFactor(2);
	mSceneMgr->setShadowCameraSetup(ShadowCameraSetupPtr(lispsmSetup));
	
	mSceneMgr->setShadowFarDistance(3000);
	mSceneMgr->setShadowColour(ColourValue(0.35, 0.35, 0.35));
	mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
	
	Light* mLight = mSceneMgr->createLight("MainLight");
	mLight->setType(Light::LT_DIRECTIONAL);
	Vector3 vec(-1,-1,0);
	vec.normalise();
	mLight->setDirection(vec);
	
	mTestNode[1] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	
	
	Entity* pEnt;
	pEnt = mSceneMgr->createEntity( "1", "robot.mesh" );
	//pEnt->setRenderingDistance(100);
	AnimationState* mAnimState = pEnt->getAnimationState("Walk");
	mAnimState->setEnabled(true);
	mAnimStateList.push_back(mAnimState);
	//pEnt->setMaterialName("2 - Default");
	mTestNode[1]->attachObject( pEnt );
	mTestNode[1]->translate(0,-100,0);
	
	pEnt = mSceneMgr->createEntity( "3", "knot.mesh" );
	mTestNode[2] = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(-200, 0, -200));
	mTestNode[2]->attachObject( pEnt );
	
	createRandomEntityClones(pEnt, 20, Vector3(-1000,0,-1000), Vector3(1000,0,1000), mSceneMgr);
	
	// Transparent object (can force cast shadows)
	pEnt = mSceneMgr->createEntity( "3.5", "knot.mesh" );
	MaterialPtr tmat = MaterialManager::getSingleton().create("TestAlphaTransparency", 
	TRANSIENT_RESOURCE_GROUP);
	tmat->setTransparencyCastsShadows(true);
	Pass* tpass = tmat->getTechnique(0)->getPass(0);
	tpass->setAlphaRejectSettings(CMPF_GREATER, 150);
	tpass->setSceneBlending(SBT_TRANSPARENT_ALPHA);
	tpass->createTextureUnitState("gras_02.png");
	tpass->setCullingMode(CULL_NONE);
	
	pEnt->setMaterialName("TestAlphaTransparency");
	mTestNode[3] = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(350, 0, -200));
	mTestNode[3]->attachObject( pEnt );
	
	MeshPtr msh = MeshManager::getSingleton().load("knot.mesh",
	TRANSIENT_RESOURCE_GROUP);
	msh->buildTangentVectors(VES_TANGENT, 0, 0);
	pEnt = mSceneMgr->createEntity( "4", "knot.mesh" );
	//pEnt->setMaterialName("Examples/BumpMapping/MultiLightSpecular");
	mTestNode[2] = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(100, 0, 200));
	mTestNode[2]->attachObject( pEnt );
	
	mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");
	
	MovablePlane movablePlane = MovablePlane(Vector3::UNIT_Y, 100);
	MeshManager::getSingleton().createPlane("Myplane",
	TRANSIENT_RESOURCE_GROUP, movablePlane,
	2500,2500,10,10,true,1,5,5,Vector3::UNIT_Z);
	Entity* pPlaneEnt;
	pPlaneEnt = mSceneMgr->createEntity( "plane", "Myplane" );
	pPlaneEnt->setMaterialName("2 - Default");
	pPlaneEnt->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pPlaneEnt);
	
	addTextureShadowDebugOverlay(1, mSceneMgr);
	
	
	mCamera->setPosition(0, 1000, 500);
	mCamera->lookAt(0,0,0);
	mCamera->setFarClipDistance(10000);
	
	
}
