#include "DebugAxisDrawer.h"

DebugAxisDrawer* DebugAxisDrawer::s_pInstance(NULL);

DebugAxisDrawer::DebugAxisDrawer():
	m_matrix(GMatrix::IDENTITY)
{
}

DebugAxisDrawer* DebugAxisDrawer::GetInstance()
{
	if(s_pInstance != NULL)
	{
		s_pInstance = new DebugAxisDrawer();
	}

	return s_pInstance;
}

void DebugAxisDrawer::DrawAxis(CEngine* i_pEngine, const GMatrix& i_matrixPos, const GString& i_text, GUInt32 i_radius, GUInt32 i_scale)
{
	bool isMatrixToStore = &i_matrixPos == &m_matrix && i_scale != 1;

	//Resolve the flickering of the Word origin name
	if(isMatrixToStore)
	{
		m_matrixCollection.push_back(const_cast<GMatrix&>(i_matrixPos));
		m_matrixCollection.back().ApplyScale(GVector3(i_scale,i_scale,i_scale));
	}

	static const GColor RED(255,0,0,255);
	static const GColor GREEN(0,255,0,255);
	static const GColor BLUE(0,0,255,255);

	//Draw the axis
	CML_DrawVector::DrawVector(i_matrixPos.GetXAxis(), i_matrixPos.GetTranslation(), RED, i_radius);
	CML_DrawVector::DrawVector(i_matrixPos.GetYAxis(), i_matrixPos.GetTranslation(), GREEN, i_radius);
	CML_DrawVector::DrawVector(i_matrixPos.GetZAxis(), i_matrixPos.GetTranslation(), BLUE, i_radius);

	//Draw the object name near the axis
	CWorldManager& worldManager = i_pEngine->GetWorldManager();
	for( CWorld*const* iter = worldManager.begin(); iter != worldManager.end(); ++iter)
	{
		const CWorld& world = **iter;

		if( (world.GetClassId() == CLSID_MENUWORLD || world.GetClassId() == CLSID_GAMEWORLD)
			&& 
			(world.GetEnabled() == eWORLD_ENABLE_RENDER || world.GetEnabled() == eWORLD_ENABLE_ALL) )
		{
			for (GUInt camIdx = 0; camIdx < world.GetNumberOfCameras(); ++camIdx)
			{
				if(isMatrixToStore)
				{
                    DebugTextDrawer3DPoint::GetInstance()->AppendLog(i_text.c_str(), world.GetCurrentCamera(camIdx), m_matrixCollection.back());
				}
				else
				{
                    DebugTextDrawer3DPoint::GetInstance()->AppendLog(i_text.c_str(), world.GetCurrentCamera(camIdx), i_matrixPos);
				}
			}
		}
    }
}
