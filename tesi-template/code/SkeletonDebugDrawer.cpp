
SkeletonDebugDrawer::SkeletonDebugDrawer(void)
{
}


SkeletonDebugDrawer::~SkeletonDebugDrawer(void)
{
}

GColor SkeletonDebugDrawer::GetJointColor( GBool bObjectIsInSkeleton, GVector3* i_ParentJointWorldPosition)
{
	static const GColor FUCSIA(255, 0, 255, 255);
	static const GColor GREEN(0, 255, 0, 255);
	static const GColor RED(255, 0, 0, 255);

	GColor jointColor;

	if(bObjectIsInSkeleton)
	{
		if(i_ParentJointWorldPosition == NULL)
		{
			jointColor = GREEN;
		}
		else
		{
			jointColor = FUCSIA;
		}
	}
	else
	{
		jointColor = RED;
	}

	return jointColor;
}

void SkeletonDebugDrawer::DrawSkeleton(const CAnimatedModel3d* i_pAnimatedModel3d, const GUInt32 i_drawJointNames)
{
	const CObject3d* animatedObject3D = i_pAnimatedModel3d;

	const GMatrix& objectWordMatrix = animatedObject3D->GetWorldMatrix();

	SkinController *skinCtrl = animatedObject3D->GetControllerById<SkinController>(CLSID_SKINCONTROLLER);
	CSkeleton* skeleton = skinCtrl->GetSkeleton(1);
	if(skeleton == NULL)
	{
		skeleton = skinCtrl->GetSkeleton(0);
	}
	GEM_ASSERT(skeleton);

	BufferController* controller = animatedObject3D->GetControllerById<BufferController>(g_uiCLSID_BUFFER_CONTROLLER);
	const vector<GMatrix>* jointsMatrices = controller->GetSkinMatrix();

	CObject3d* pObj = skeleton->GetRootJoint(0);
	if (pObj && jointsMatrices)
	{
		SkeletonDebugDrawer::DrawEMFXSkeleton(i_drawJointNames, objectWordMatrix, pObj, *jointsMatrices, skeleton);
	}

	//Draw of the object position
	CML_DrawVector::DrawPoint(objectWordMatrix.GetTranslation(), GColor(255,0,0,255), 0.02);
}

void SkeletonDebugDrawer::DrawEMFXSkeleton(
	GBool i_bDrawJointNames,
	const GMatrix& i_RootMatrix,
	CObject3d* i_pObj,
	const vector<GMatrix>& i_SkinPalette,
	CSkeleton* i_Skeleton,
	GVector3* i_ParentJointWorldPosition)
{
	GEM_ASSERT(i_pObj != NULL);

	//dato l'object3d mi da l'index alla posizione nel vettore dello scheletro
	GInt iJointIndex = vector_signed_index(i_Skeleton->GetJointsList(), i_pObj);

	GBool bObjectIsInSkeleton = (iJointIndex >= 0);

	GVector3 jointWorldPosition;
	GMatrix jointWorldMatrix;
	if (bObjectIsInSkeleton)
	{
		jointWorldMatrix = RiderAnimationsBehaviour::GetJointLocalMatrix(i_Skeleton, iJointIndex, i_SkinPalette[iJointIndex]);
	}
	else
	{
		jointWorldMatrix = i_pObj->GetWorldMatrix();
	}

	jointWorldPosition = (jointWorldMatrix * i_RootMatrix).GetTranslation();
	GColor jointColor = GetJointColor(bObjectIsInSkeleton, i_ParentJointWorldPosition);

	const GReal POINT_SIZE = 0.05f;
	CML_DrawVector::DrawPoint(jointWorldPosition, jointColor, POINT_SIZE);

	if(i_bDrawJointNames)
	{
		CWorldManager worldManager = g_pEngine->GetWorldManager();

		for( CWorld*const* iter = worldManager.begin(); iter != worldManager.end(); ++iter)
		{
			const CWorld& world = **iter;

			if( (world.GetClassId() == CLSID_MENUWORLD || world.GetClassId() == CLSID_GAMEWORLD)
				&& 
				(world.GetEnabled() == eWORLD_ENABLE_RENDER || world.GetEnabled() == eWORLD_ENABLE_ALL) )
			{
				for (GUInt camIdx = 0; camIdx < world.GetNumberOfCameras(); ++camIdx)
				{
					GVector2 screenPosition;
					world.GetCurrentCamera(camIdx)->ProjectPoint(jointWorldPosition, screenPosition);

					const char* name = i_pObj->GetId().DebugGetName().c_str();
					if(name)
					{
						GraphicModule::DebugTextDrawer::GetInstance()->AppendLog(name, screenPosition.x, screenPosition.y);
					}
				}
			}
		}
	}

	if (i_ParentJointWorldPosition)
	{
		static const GColor YELLOW(255,255,0,255);
		static const GReal SEGMENT_SIZE = 0.02f;
		CML_DrawVector::DrawSegment(jointWorldPosition, *i_ParentJointWorldPosition, YELLOW, SEGMENT_SIZE);
	}
	
	CObject3d::Children::const_iterator childIt = i_pObj->GetChildren().begin();
	CObject3d::Children::const_iterator childEnd = i_pObj->GetChildren().end();

	while (childIt != childEnd)
	{
		//Recursive call
		DrawEMFXSkeleton(i_bDrawJointNames, i_RootMatrix, *childIt, i_SkinPalette, i_Skeleton, &jointWorldPosition);
		childIt++;
	}
}