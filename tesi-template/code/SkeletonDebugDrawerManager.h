#ifndef SKELETON_DEBUG_DRAWER_MANAGER_H
#define SKELETON_DEBUG_DRAWER_MANAGER_H

class SkeletonDebugDrawerManager : public DebugWorldElement
{
private:
	SkeletonDebugDrawerManager();

	static SkeletonDebugDrawerManager* s_instance;

	struct ObjectDrawedInfo
	{
		ENABLE_METADATA;

		CAnimatedModel3d*	m_pObj;
		GUInt32				m_DrawJointNames;
		GUInt32				m_PauseDrawing;
		GUInt32				m_RemoveOnNextFrame;
	};

	vector<ObjectDrawedInfo> m_objectDrawed;

public:

	ENABLE_METADATA;

	static SkeletonDebugDrawerManager* GetInstance();

	void Draw( const DrawParams & params );

	//Return true if the added object hasn't been present yet
	GBool AddObject(CAnimatedModel3d* i_pObj, GUInt32 i_DrawJointNames);

	void RemoveObject(CAnimatedModel3d* i_pObj);
};

#endif