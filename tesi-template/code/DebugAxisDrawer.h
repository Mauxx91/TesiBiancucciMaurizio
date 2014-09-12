#ifndef DEBUG_AXIS_DRAWER_H
#define DEBUG_AXIS_DRAWER_H

class DebugAxisDrawer
{
private:

	static DebugAxisDrawer* s_pInstance;

	const GMatrix m_matrix;

	vector<GMatrix> m_matrixCollection;

protected:

	DebugAxisDrawer();

public:

	static DebugAxisDrawer* GetInstance();

	void DrawAxis(CEngine* i_pEngine, const GMatrix& i_matrixPos, const GString& i_text, GUInt32 i_radius, GUInt32 i_scale = 1);

	inline const GMatrix& GetIndentityMatrix();

};

inline const GMatrix& DebugAxisDrawer::GetIndentityMatrix()
{
	return m_matrix;
}

#endif
