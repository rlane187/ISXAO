#include "isxao_main.h"

bool InventorySlotType::GetMember(LSOBJECTDATA ObjectData, PLSTYPEMEMBER Member, int argc, char* argv[], LSOBJECT& Object)
{
	if (isxao_utilities::GetGameState() != GAMESTATE_IN_GAME)
		return false;
	if (!ObjectData.Ptr)
		return false;
	__try
	{
#define pInventorySlot ((INVENTORYSLOT*)ObjectData.Ptr)
		switch (InventorySlotTypeMembers(Member->ID))
		{
		case IsReady:
		{
			bool IsReady = pInventorySlot->IsItemLocked();
			Object.DWord = !IsReady;
			Object.Type = pBoolType;
			break;
		}
		case Item:
		{
			if((Object.Ptr = pInventorySlot->pItem))
			{
				Object.Type = pInventoryItemType;
				return true;
			}
			return false;
		}
		case ItemCount:
		{
			Object.DWord = pInventorySlot->GetInvSlotData()->count;
			Object.Type = pUintType;
			break;
		}
		case LockOutRemaining:
		{
			DWORD a;
			DWORD b;
			double progress = pInventorySlot->GetItemProgress(a, b);
			Object.DWord = a;
			Object.Type = pUintType;
			break;
		}
		case Name:
		{
			Object.ConstCharPtr = pInventorySlot->GetSlotName();
			Object.Type = pStringType;
			break;
		}
		case QualityLevel:
		{
			Object.DWord = pInventorySlot->GetInvSlotData()->quality_level;
			Object.Type = pUintType;
			break;
		}
		case Slot:
		{
			identity_t id = pInventorySlot->SlotID;
			p_identity_t pId = static_cast<p_identity_t>(pISInterface->GetTempBuffer(sizeof(identity_t), &id));
			Object.Ptr = pId;
			Object.Type = pIdentityType;
			break;
		}
		default: break;
		}
#undef pInventorySlot
	}
	__except (pExtension->HandleLSTypeCrash(__FUNCTION__, Member->ID, ObjectData.Ptr, argc, argv, GetExceptionCode(), GetExceptionInformation()))
	{

	}
	return true;
}

bool InventorySlotType::GetMethod(LSOBJECTDATA& ObjectData, PLSTYPEMETHOD pMethod, int argc, char* argv[])
{
	if (!ObjectData.Ptr)
		return false;
	__try
	{
#define pInventorySlot ((INVENTORYSLOT*)ObjectData.Ptr)
		switch (InventorySlotTypeMethods(pMethod->ID))
		{
		case Use:
		{
			identity_t d;
			ZeroMemory(&d, sizeof(identity_t));
			if(P_ENGINE_CLIENT_ANARCHY && P_ENGINE_CLIENT_ANARCHY->get_client_char() && P_ENGINE_CLIENT_ANARCHY->GetItemByTemplate(pInventorySlot->SlotID, d))
			{
				P_ENGINE_CLIENT_ANARCHY->N3Msg_UseItem(pInventorySlot->SlotID, false);
				return true;
			}
			return false;
		}
		default: break;
		}
#undef pInventorySlot
	}
	__except (pExtension->HandleLSTypeCrash(__FUNCTION__, pMethod->ID, ObjectData.Ptr, argc, argv, GetExceptionCode(), GetExceptionInformation()))
	{

	}
	return true;
}

bool InventorySlotType::ToText(LSOBJECTDATA ObjectData, char *buf, unsigned int buflen)
{
	if (isxao_utilities::GetGameState() != GAMESTATE_IN_GAME)
		return false;
	if (!ObjectData.Ptr)
		return false;
#define pInventorySlot ((INVENTORYSLOT*)ObjectData.Ptr)	
	sprintf_s(buf, buflen, "%s", pInventorySlot->GetSlotName());
#undef pInventorySlot

	return true;
}