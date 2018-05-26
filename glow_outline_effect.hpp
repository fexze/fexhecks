#pragma once

#include "UtlVector.hpp"
#include "IClientEntity.hpp"

#define ENTRY_IN_USE -2
#define END_OF_FREE_LIST -1

struct GlowObjectDefinition_t
{
	GlowObjectDefinition_t() 
	{
		memset(this, 0, sizeof(*this)); 
	}

	class C_BasePlayer* m_pEntity;

	union
	{
		Vector m_vGlowColor;

		struct
		{
			float   m_flRed;
			float   m_flGreen;
			float   m_flBlue;
		};
	};

	float      m_flAlpha;
	uint8_t    pad_0014[4];
	float      m_flSomeFloat;
	uint8_t    pad_001C[4];
	float      m_flAnotherFloat;
	bool       m_bRenderWhenOccluded;
	bool       m_bRenderWhenUnoccluded;
	bool       m_bFullBloomRender;
	uint8_t    pad_0027[5];
	int32_t    m_nGlowStyle;
	int32_t    m_nSplitScreenSlot;
	int32_t    m_nNextFreeSlot;

	bool IsUnused() const 
	{
		return m_nNextFreeSlot != ENTRY_IN_USE;
	}
};

class CGlowObjectManager
{
public:
	int RegisterGlowObject(C_BasePlayer* pEntity, const Vector& vGlowColor, float flGlowAlpha, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot)
	{
		int nIndex;

		if (m_nFirstFreeSlot == END_OF_FREE_LIST)
		{
			nIndex = -1;
		}
		else 
		{
			nIndex = m_nFirstFreeSlot;
			m_nFirstFreeSlot = m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot;
		}

		m_GlowObjectDefinitions[nIndex].m_pEntity = pEntity;
		m_GlowObjectDefinitions[nIndex].m_vGlowColor = vGlowColor;
		m_GlowObjectDefinitions[nIndex].m_flAlpha = flGlowAlpha;
		m_GlowObjectDefinitions[nIndex].m_bRenderWhenOccluded = bRenderWhenOccluded;
		m_GlowObjectDefinitions[nIndex].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
		m_GlowObjectDefinitions[nIndex].m_nSplitScreenSlot = nSplitScreenSlot;
		m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot = ENTRY_IN_USE;
		return nIndex;
	}

	void UnregisterGlowObject(int nGlowObjectHandle)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_nNextFreeSlot = m_nFirstFreeSlot;
		m_GlowObjectDefinitions[nGlowObjectHandle].m_pEntity = NULL;
		m_nFirstFreeSlot = nGlowObjectHandle;
	}

	void SetEntity(int nGlowObjectHandle, C_BasePlayer* pEntity)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_pEntity = pEntity;
	}

	void SetColor(int nGlowObjectHandle, const Vector& vGlowColor)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_vGlowColor = vGlowColor;
	}

	void SetAlpha(int nGlowObjectHandle, float flAlpha)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_flAlpha = flAlpha;
	}

	void SetRenderFlags(int nGlowObjectHandle, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenOccluded = bRenderWhenOccluded;
		m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
	}

	GlowObjectDefinition_t* m_GlowObjectDefinitions;

	int GetSize()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0xC);
	}

	int m_nFirstFreeSlot;
};