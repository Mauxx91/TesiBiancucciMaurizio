#ifndef SKELETON_DEBUG_DRAWER_H
#define SKELETON_DEBUG_DRAWER_H

class SkeletonDebugDrawer
{
private:

	static GColor GetJointColor( GBool bObjectIsInSkeleton, GVector3* i_ParentJointWorldPosition);

	SkeletonDebugDrawer(void);

public:

	~SkeletonDebugDrawer(void);

	static void DrawSkeleton(const CAnimatedModel3d* i_pAnimatedModel3d, const GUInt32 i_drawJointNames);

	static void DrawEMFXSkeleton(
		GBool i_bDrawJointNames,
		const GMatrix& i_RootMatrix,
		CObject3d* i_pObj,
		const vector<GMatrix>& i_SkinPalette,
		CSkeleton* i_Skeleton,
		GVector3* i_ParentJointWorldPosition = NULL);

	inline static void DrawPoint(const GVector3& i_vPosition, const GUInt8 i_uiRed, const GUInt8 i_uiGreen, const GUInt8 i_uiBlue, const GReal i_fSize);
};

inline void SkeletonDebugDrawer::DrawPoint(const GVector3& i_vPosition, const GUInt8 i_uiRed, const GUInt8 i_uiGreen, const GUInt8 i_uiBlue, const GReal i_fSize)
{
	CML_DrawVector::DrawPoint(i_vPosition, i_uiRed, i_uiBlue, i_uiBlue, i_fSize);
}

#endif