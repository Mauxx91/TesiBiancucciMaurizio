template<typename T>
void ResourcesManagerDebug::FillObjectInfoContainer(ObjectInfoContainer& i_infoContainer, T* i_pRes)
{
	GeneralFillObjectInfoContainer(i_infoContainer, i_pRes);

	i_infoContainer.m_objectInfo.m_WMXPosition = 0.0f;
	i_infoContainer.m_objectInfo.m_WMYPosition = 0.0f;
	i_infoContainer.m_objectInfo.m_WMZPosition = 0.0f;

	i_infoContainer.m_objectInfo.m_bIsAnimated = GFALSE;
}

void ResourcesManagerDebug::FillObjectInfoContainer(ObjectInfoContainer& i_infoContainer, CObject3d* i_pRes)
{
	GeneralFillObjectInfoContainer(i_infoContainer, i_pRes);

	CObject3d* pObj = (CObject3d*)(uintptr_t)(i_infoContainer.m_objectInfo.m_uiCObjectAddres);
	i_infoContainer.m_objectInfo.m_uiCParentAddres = (GUInt64)(pObj->GetParent());

	const GVector3& objectWordPosition = pObj->GetWorldMatrix().GetTranslation();
	i_infoContainer.m_objectInfo.m_WMXPosition = objectWordPosition.x;
	i_infoContainer.m_objectInfo.m_WMYPosition = objectWordPosition.y;
	i_infoContainer.m_objectInfo.m_WMZPosition = objectWordPosition.z;

	i_infoContainer.m_objectInfo.m_bIsAnimated = pObj->GetClassId() == CLSID_ANIMATEDMODEL3D;
}

template<typename T>
void ResourcesManagerDebug::GeneralFillObjectInfoContainer(ObjectInfoContainer& i_infoContainer, T* i_pRes)
{
	sprintf_s(i_infoContainer.m_szName, 256, "%s", i_pRes->GetId().DebugGetName().c_str());
	sprintf_s(i_infoContainer.m_objectInfo.m_szName, 256, "%s", i_pRes->GetId().DebugGetName().c_str());
	i_infoContainer.m_objectInfo.m_uiHash = i_pRes->GetId().GetNameHash();
	i_infoContainer.m_objectInfo.m_uiCObjectAddres = reinterpret_cast<GUInt64>(i_pRes);
	i_infoContainer.m_objectInfo.m_uiRefCounter = i_pRes->GetRefCounter();
}

template<typename T>
void ResourcesManagerDebug::FillResourcesHelper(const TManager<T>& i_manager, vector<ResourcesManagerDebug::ObjectInfoContainer>& o_aListOfResources)
{
	size_t uiResNum = i_manager.size();

	o_aListOfResources.clear();
	o_aListOfResources.reserve(uiResNum);

	for(GUInt uiIndex = 0; uiIndex < uiResNum; ++uiIndex)
	{
		ObjectInfoContainer new_infoContainer;
		FillObjectInfoContainer(new_infoContainer, i_manager.at(uiIndex));

		o_aListOfResources.push_back(new_infoContainer);
	}
}

void ResourcesManagerDebug::FillResourcesHelperAsTree(const CObjects3dManager& i_manager, vector<ResourcesManagerDebug::ObjectInfoContainer>& o_aListOfRootContainer)
{
	const size_t uiResNum = i_manager.size();

	o_aListOfRootContainer.clear();
	o_aListOfRootContainer.reserve(uiResNum);

	vector<GBool> inserted;
	inserted.resize(uiResNum, GFALSE);

	GBool bFinished = GFALSE;

	while (!bFinished)
	{
		bFinished = GTRUE;

		for(GUInt uiIndex = 0; uiIndex < uiResNum; ++uiIndex)
		{
			if( !inserted[uiIndex] )
			{
				ObjectInfoContainer new_infoContainer;
				FillObjectInfoContainer(new_infoContainer, i_manager.at(uiIndex));

				//If it's a parent object
				if(new_infoContainer.m_objectInfo.m_uiCParentAddres == 0)
				{
					o_aListOfRootContainer.push_back(new_infoContainer);
					inserted[uiIndex] = GTRUE;
				}
				else
				{
					//Search the parent object where append this node
					for(GUInt i = 0; i < o_aListOfRootContainer.size(); ++i)
					{
						if(o_aListOfRootContainer[i].m_objectInfo.m_uiCObjectAddres == new_infoContainer.m_objectInfo.m_uiCParentAddres)
						{
							o_aListOfRootContainer[i].m_aListOfChildObjects.push_back(new_infoContainer);
							inserted[uiIndex] = GTRUE;
							bFinished = GFALSE;
							break;
						}

					}
				}

			}
		}
	}

	//Add the spare (root) elements that hasn't the parent pointer = 0
	for(GUInt uiIndex = 0; uiIndex < uiResNum; ++uiIndex)
	{
		if( !inserted[uiIndex] )
		{
			ObjectInfoContainer new_infoContainer;
			FillObjectInfoContainer(new_infoContainer, i_manager.at(uiIndex));

			GBool bIsRoot = GTRUE;
			for(GUInt i = 0; i < o_aListOfRootContainer.size(); ++i)
			{
				if(o_aListOfRootContainer[i].m_objectInfo.m_uiCObjectAddres == new_infoContainer.m_objectInfo.m_uiCParentAddres)
				{
					o_aListOfRootContainer[i].m_aListOfChildObjects.push_back(new_infoContainer);
					bIsRoot = GFALSE;
					break;
				}
			}

			if(bIsRoot)
			{
				o_aListOfRootContainer.push_back(new_infoContainer);
			}
		}
	}
}

ResourcesManagerDebug::ResourceInfo::ResourceInfo(const GString& i_szFileNameBin, const CWorld* i_pWorld, GBool i_bIsHierarchicalViewEnabled )
{
	sprintf_s(m_szFileNameBin, 256, "%s",i_szFileNameBin.c_str());
	m_uiWorldAddres = reinterpret_cast<GUInt64>(i_pWorld);
	m_uiRefCounter = int_cast<GUInt>(i_pWorld->GetRefCounter())-1;

	sprintf_s(m_managers[0].m_szManagerName, 256, "%s", "Objects3dManager");
	sprintf_s(m_managers[1].m_szManagerName, 256, "%s", "Objects3dManagerTreeView");
	sprintf_s(m_managers[2].m_szManagerName, 256, "%s", "MeshManager");
	sprintf_s(m_managers[3].m_szManagerName, 256, "%s", "MaterialManager");
	sprintf_s(m_managers[4].m_szManagerName, 256, "%s", "TextureManager");
	sprintf_s(m_managers[5].m_szManagerName, 256, "%s", "AnimationManager");
	sprintf_s(m_managers[6].m_szManagerName, 256, "%s", "AnimatorManager");

	FillResourcesHelper(i_pWorld->GetObjects3dManager(),			m_managers[0].m_aListOfObjects);
	if (i_bIsHierarchicalViewEnabled)
	{
		FillResourcesHelperAsTree(i_pWorld->GetObjects3dManager(),		m_managers[1].m_aListOfObjects);
	}
	FillResourcesHelper(i_pWorld->GetMeshManager(),					m_managers[2].m_aListOfObjects);
	FillResourcesHelper(i_pWorld->GetMaterialManager(),				m_managers[3].m_aListOfObjects);
	FillResourcesHelper(i_pWorld->GetTextureManager(),				m_managers[4].m_aListOfObjects);
	FillResourcesHelper(i_pWorld->GetAnimationManager(),			m_managers[5].m_aListOfObjects);
	FillResourcesHelper(i_pWorld->GetAnimatorManager(),				m_managers[6].m_aListOfObjects);
}

void ResourcesManagerDebug::ToggleHierarchicalView()
{
	m_bIsHierarchicalViewEnabled = !m_bIsHierarchicalViewEnabled;

	Update();
}

ResourcesManagerDebug::ObjectInfo::ObjectInfo():
	m_uiAxesScale(1.0f),
	m_fAxesRadius(0.05f),
	m_bIsAnimated(GFALSE)
{
	m_WMXPosition = 0;
	m_WMYPosition = 0;
	m_WMZPosition = 0;
}

void ResourcesManagerDebug::ObjectInfo::DrawWorldMatrix()
{
	CObject3d* pObj = (CObject3d*)(uintptr_t)(m_uiCObjectAddres);
	
	const GMatrix& matrix = pObj->GetWorldMatrix();

	DebugAxisDrawer::GetInstance()->DrawAxis(g_pEngine, matrix, m_szName, m_fAxesRadius);
}

void ResourcesManagerDebug::ObjectInfo::ToggleFlag(uint32_t flag)
{
	CObject3d* pObj = (CObject3d*)(uintptr_t)(m_uiCObjectAddres);

	if(pObj->GetFlags() & flag)
	{
		pObj->SetFlags(0, (uint32_t)~flag);
	}
	else
	{
		pObj->SetFlags(flag);
	}
}

void ResourcesManagerDebug::ObjectInfo::ToggleDrawSkeleton()
{
	RealDrawSkeleton(false);
}

void ResourcesManagerDebug::ObjectInfo::ToggleDrawSkeletonWithJointNames()
{
	RealDrawSkeleton(true);
}

void ResourcesManagerDebug::ObjectInfo::RealDrawSkeleton(GBool i_bDrawJointNames)
{
	if(m_bIsAnimated)
	{
		GBool bInserted	= SkeletonDebugDrawerManager::GetInstance()->AddObject((CAnimatedModel3d*)(uintptr_t)(m_uiCObjectAddres), i_bDrawJointNames);

		if( !bInserted )
		{
			SkeletonDebugDrawerManager::GetInstance()->RemoveObject((CAnimatedModel3d*)(uintptr_t)(m_uiCObjectAddres));
		}
	}
}
