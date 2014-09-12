#ifndef RESOURCES_MANAGER_DEBUG
#define RESOURCES_MANAGER_DEBUG

class ResourcesManagerDebug
{
public:
	ENABLE_METADATA;

	ResourcesManagerDebug();
	~ResourcesManagerDebug();

	GBool Init(ResourcesManager* i_pResourcesManager);
	GBool Uninit();

	void Update() const;

	GBool IsInitialized() const;

	static GBool ResourcesManagerDebugEnabled();

private:

	void RegisterToBeholder();
	void UnregisterFromBeholder();

	void UpdateGroupResources() const;

	ResourcesManagerDebug(const ResourcesManagerDebug& i_that);
	ResourcesManagerDebug& operator=(const ResourcesManagerDebug& i_that);

private:

	static const FixedString64 s_szMetadataName;

public:

	struct ObjectInfo
	{
		ENABLE_METADATA;

		ObjectInfo();

		GUInt64		m_uiHash;
		char		m_szName[256];
		uint_t		m_uiRefCounter;
		GUInt64		m_uiCObjectAddres;
		GUInt64		m_uiCParentAddres;

		void DrawWorldMatrix();
		void ToggleBoundingSphere();
		void ToggleVisibility();
		void ToggleDrawSkeleton();
		void ToggleDrawSkeletonWithJointNames();

		void ToggleFlag(uint32_t flag);

		void RealDrawSkeleton(GBool i_bDrawJointNames);

		float m_WMXPosition;
		float m_WMYPosition;
		float m_WMZPosition;

		GUInt32 m_uiAxesScale;
		GReal 	m_fAxesRadius;
		GBool	m_bIsAnimated;
	};

private:

	struct ObjectInfoContainer
	{
		ENABLE_METADATA;

		char				m_szName[256];
		ObjectInfo			m_objectInfo;
		vector<ObjectInfoContainer>	m_aListOfChildObjects;
	};

	struct ManagerInfo
	{
		ENABLE_METADATA;
	
		char				m_szManagerName[256];
		vector<ObjectInfoContainer>	m_aListOfObjects;
	};

	struct ResourceInfo
	{
		ResourceInfo() { }
		ResourceInfo(const GString& i_szFileNameBin, const CWorld* i_pWorld, GBool i_bIsHierarchicalViewEnabled);

		ENABLE_METADATA;

		char				m_szFileNameBin[256];
		GUInt				m_uiRefCounter;
		GUInt64				m_uiWorldAddres;
		ManagerInfo			m_managers[7];
	};

	template<typename T>
	static void FillObjectInfoContainer(ObjectInfoContainer& i_infoContainer, T* i_pRes);

	static void FillObjectInfoContainer(ObjectInfoContainer& i_infoContainer, CObject3d* i_pRes);

	template<typename T>
	static void GeneralFillObjectInfoContainer(ObjectInfoContainer& i_infoContainer, T* i_pRes);

	template<typename T>
	static void FillResourcesHelper(const TManager<T>& i_manager, vector<ResourcesManagerDebug::ObjectInfoContainer>& o_aListOfResources);

	static void FillResourcesHelperAsTree(const CObjects3dManager& i_manager, vector<ResourcesManagerDebug::ObjectInfoContainer>& o_aListOfRootContainer);

	struct ResourcesGroup
	{
		ENABLE_METADATA;
		
		char					m_szGroupName[256];
		vector<ResourceInfo>	m_aListOfResources;
	};

	struct DebugData
	{
		ENABLE_METADATA;

		vector<ResourcesGroup>	m_aListOfGroupResources;
	};

	ResourcesManager*		m_pResourcesManager;

	GBool				m_bBeholderRegistered;
	GBool				m_bInitialized;	
	GBool				m_bIsHierarchicalViewEnabled;					

	mutable CMutex		m_oMutex;
	mutable DebugData	m_oDebugData;

	const DebugData& UpdateResources() const { Update(); return m_oDebugData; }

public:
	void ReloadDescriptors();

	void ToggleHierarchicalView();
};

inline GBool ResourcesManagerDebug::IsInitialized() const
{
	CMutexAutoLock oMutexAutoLock(m_oMutex);
	return m_bInitialized;
}

#endif