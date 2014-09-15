#include "SkeletonDebugDrawerManager.h"

START_META("SkeletonDebugDrawerManager",SkeletonDebugDrawerManager)
	META_PROPERTY(m_objectDrawed)
END_META;

START_META("SkeletonDebugDrawerManager::info",SkeletonDebugDrawerManager::ObjectDrawedInfo)
	META_PROPERTY(m_pObj),
	META_PROPERTY(m_DrawJointNames),
	META_PROPERTY(m_PauseDrawing),
	META_PROPERTY(m_RemoveOnNextFrame),
END_META;

SkeletonDebugDrawerManager* SkeletonDebugDrawerManager::s_instance(0);

SkeletonDebugDrawerManager::SkeletonDebugDrawerManager()
{
	CML_MetadataRegistry::AddEntry("SkeletonDebugDrawerManager", this);
}

SkeletonDebugDrawerManager* SkeletonDebugDrawerManager::GetInstance()
{
	if(!s_instance)
	{
		s_instance = GEM_NEW SkeletonDebugDrawerManager();
	}
	return s_instance;
}

void SkeletonDebugDrawerManager::Draw( const DrawParams & params )
{
	for(GUInt32 index = 0; index < m_objectDrawed.size(); ++index)
	{
		if( !m_objectDrawed[index].m_PauseDrawing )
		{
			SkeletonDebugDrawer::DrawSkeleton(m_objectDrawed[index].m_pObj, m_objectDrawed[index].m_DrawJointNames);
		}
	}

	vector<ObjectDrawedInfo>::iterator iterator = m_objectDrawed.begin();
	while( iterator != m_objectDrawed.end() )
	{
		vector<ObjectDrawedInfo>::iterator iteratorCurr = iterator;
		iterator++;

		if(iteratorCurr->m_RemoveOnNextFrame)
		{
			if(m_objectDrawed.size() > 1)
			{
				m_objectDrawed.erase(iteratorCurr);
			}
			else
			{
				m_objectDrawed.clear();
				break;
			}
		}
	}
}

GBool SkeletonDebugDrawerManager::AddObject(CAnimatedModel3d* i_pObj, GUInt32 i_DrawJointNames)
{
	vector<ObjectDrawedInfo>::iterator iterator = m_objectDrawed.begin();
	vector<ObjectDrawedInfo>::iterator iteratorEnd = m_objectDrawed.end();

	GBool bIsPresentYet = false;

	while(iterator != iteratorEnd && !bIsPresentYet)
	{
		if(iterator->m_pObj == i_pObj)
		{
			bIsPresentYet = true;
		}
		iterator ++;
	}

	if( !bIsPresentYet )
	{
		ObjectDrawedInfo objInfo;
		objInfo.m_pObj = i_pObj;
		objInfo.m_DrawJointNames = i_DrawJointNames;
		objInfo.m_PauseDrawing = 0;
		objInfo.m_RemoveOnNextFrame = 0;

		m_objectDrawed.push_back(objInfo);
	}

	return !bIsPresentYet;
}

void SkeletonDebugDrawerManager::RemoveObject(CAnimatedModel3d* i_pObj)
{
	vector<ObjectDrawedInfo>::iterator iterator = m_objectDrawed.begin();
	vector<ObjectDrawedInfo>::iterator iteratorEnd = m_objectDrawed.end();

	while(iterator != iteratorEnd)
	{
		if(iterator->m_pObj == i_pObj)
		{
			m_objectDrawed.erase(iterator);
			break;
		}
		iterator ++;
	}
}