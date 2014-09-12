START_ACTIONS(ResourcesManagerDebug::ObjectInfo)
	ACTION("DrawWorldMatrix", &ResourcesManagerDebug::ObjectInfo::DrawWorldMatrix),
	ACTION("ToggleBoundingSphere", &ResourcesManagerDebug::ObjectInfo::ToggleBoundingSphere),
	ACTION("ToggleVisibility", &ResourcesManagerDebug::ObjectInfo::ToggleVisibility),
	ACTION("ToggleDrawSkeleton", &ResourcesManagerDebug::ObjectInfo::ToggleDrawSkeleton),
	ACTION("ToggleDrawSkeletonWithJointNames", &ResourcesManagerDebug::ObjectInfo::ToggleDrawSkeletonWithJointNames)
END_ACTIONS;

START_ACTIONS(ResourcesManagerDebug)
	ACTION("ReloadDescriptors", &ResourcesManagerDebug::ReloadDescriptors),
	ACTION("ToggleHierarchicalView", &ResourcesManagerDebug::ToggleHierarchicalView),
END_ACTIONS;

// ResourcesManagerDebug::Info's members Metadata definition
START_META("ResourceManager::Info",ResourcesManagerDebug::ObjectInfo)
	META_PROPERTY(m_szName),
	META_PROPERTY(m_uiHash),
	META_PROPERTY(m_uiRefCounter),
	META_PROPERTY(m_uiCObjectAddres),
	META_PROPERTY(m_uiCParentAddres),
	META_PROPERTY(m_bIsAnimated),
	META_PROPERTY(m_WMXPosition),
	META_PROPERTY(m_WMYPosition),
	META_PROPERTY(m_WMZPosition),
END_META;

// ResourcesManagerDebug::ResourceInfo's members Metadata definition
START_META("ResourceManager::ResourceInfo",ResourcesManagerDebug::ResourceInfo)
	META_PROPERTY(m_szFileNameBin),
	META_PROPERTY(m_uiRefCounter),
	META_PROPERTY(m_uiWorldAddres),
	META_NAMED_PROPERTY("Manager",m_managers)
END_META;

// ResourcesManagerDebug::ResourcesGroup's members Metadata definition
START_META("ResourceManager::ResourcesGroup",ResourcesManagerDebug::ResourcesGroup)
	META_PROPERTY(m_szGroupName),
	META_NAMED_PROPERTY("Resource", m_aListOfResources)
END_META;

START_META("ResourceManager::DebugData",ResourcesManagerDebug::DebugData)
	META_NAMED_PROPERTY("Group", m_aListOfGroupResources)
END_META;

// ResourcesManagerDebug::DebugData's members Metadata definition
START_META("ResourceManager",ResourcesManagerDebug)
	META_NAMED_GETTER("ResourcesGroup",UpdateResources),
	META_PROPERTY(m_oDebugData)
END_META;

// ResourcesManagerDebug::ManagerInfo's members Metadata definition
START_META("ResourceManager::ManagerInfo",ResourcesManagerDebug::ManagerInfo)
	META_PROPERTY(m_szManagerName),
	META_NAMED_PROPERTY("ChildObject", m_aListOfObjects)
END_META;

// ResourcesManagerDebug::ManagerInfo's members Metadata definition
START_META("ResourceManager::ObjectInfoContainer",ResourcesManagerDebug::ObjectInfoContainer)
	META_PROPERTY(m_szName),
	META_PROPERTY(m_objectInfo),
	META_NAMED_PROPERTY("ChildObject", m_aListOfChildObjects)
END_META;
