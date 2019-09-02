#include "isxao_main.h"

namespace isxao_classes
{

	DWORD Actor::BuildLSNCU(LSIndex* pIndex)
	{
		auto l = GetSpellTemplateData()->GetSpellTemplateDataData().NanoTemplateList;
		for (auto it = l.begin(); it != l.end(); ++it)
		{
			auto pNanoTemplate = reinterpret_cast<NanoTemplate*>(&(*it));
			pIndex->AddItem(reinterpret_cast<LSOBJECTDATA&>(pNanoTemplate));
		}
		return pIndex->GetContainerUsed();
	}

	DWORD Actor::BuildLSPets(LSIndex* pIndex)
	{
		std::map<IDENTITY, DWORD> pet_map;
		if (GetPetIDs(pet_map) == 0)
			return 0;
		for (auto it = pet_map.begin(); it != pet_map.end(); ++it)
		{
			auto p_dynel = isxao_utilities::GetDynel(it->first);
			pIndex->AddItem(reinterpret_cast<LSOBJECTDATA&>(p_dynel));
		}
		return pIndex->GetContainerUsed();
	}

	DWORD Actor::Casting()
	{
		return GetSpellTemplateData()->GetNanoBeingCast();
	}

	PCSTR Actor::Consider()
	{
		float consider;
		P_ENGINE_CLIENT_ANARCHY->N3Msg_Consider(GetIdentity(), consider);
		if (consider >= 0.0f && consider < 0.2000000029802322)
			return "Easy";
		if (consider >= 0.2000000029802322 && consider < 0.4900000095367432)
			return "Probable";
		if (consider >= 0.4900000095367432 && consider < 0.50999999)
			return "MaybePossible";
		if (consider >= 0.50999999 && consider < 0.69999999)
			return "Hard";
		if (consider >= 0.69999999 && consider < 0.8999999761581421)
			return "AlmostImpossible";
		if (consider >= 0.8999999761581421)
			return "Impossible";
		return "VeryEasy";
	}

	PCSTR Actor::ConColor()
	{
		RGBCOLOR argb = ConColorARGB();
		if (argb.ARGB == 0xFFFFFF)
			return "White";
		if (argb.ARGB == 0xAAAAAA)
			return "Grey";
		float fR = float(argb.R / 255);
		float fG = float(argb.G / 255);
		float fB = float(argb.B / 255);
		float fH = 0.0f;
		float fS = 0.0f;
		float fV = 0.0f;
		float fCMax = max(max(fR, fG), fB);
		float fCMin = min(min(fR, fG), fB);
		float fDelta = fCMax - fCMin;

		if (fDelta > 0) {
			if (fCMax == fR) {
				fH = 60 * (fmod(((fG - fB) / fDelta), 6));
			}
			else if (fCMax == fG) {
				fH = 60 * (((fB - fR) / fDelta) + 2);
			}
			else if (fCMax == fB) {
				fH = 60 * (((fR - fG) / fDelta) + 4);
			}

			if (fCMax > 0) {
				fS = fDelta / fCMax;
			}
			else {
				fS = 0;
			}

			fV = fCMax;
		}
		else {
			fH = 0;
			fS = 0;
			fV = fCMax;
		}

		if (fH < 0) {
			fH = 360 + fH;
		}
		if (fH < 0.0f)
			return "Unknown";
		if (fH <= 15.0f)
			return "Red";
		if (fH <= 45.0f)
			return "Orange";
		if (fH <= 75.0f)
			return "Yellow";
		return "Green";
	}

	RGBCOLOR Actor::ConColorARGB()
	{
		RGBCOLOR argb;
		argb.ARGB = 0xFFFFFF;
		float consider;
		DWORD con_type = P_ENGINE_CLIENT_ANARCHY->N3Msg_Consider(GetIdentity(), consider);
		if (consider > 1.0f)
			consider = 1.0f;
		if(con_type == 3)
		{
			if(consider  > 0.0f)
			{
				if(consider >= 0.5f)
				{
					argb.ARGB = (int(2 * (1 - consider) * 240) | 0xFF00) << 8;
					return argb;
				}
				argb.ARGB = (int(2 * (consider * 255)) << 16) | 0xFF00;
				return argb;
			}
			argb.ARGB = 0xAAAAAA;
			return argb;				
		}
		return argb;
	}

	void Actor::DoFace()
	{
		VECTOR3 client_position;
		P_ENGINE_CLIENT_ANARCHY->N3Msg_GetGlobalCharacterPosition(client_position);
		auto position = GetPosition();
		auto new_rotation = QUATERNION::GetQuaternionToFace(position, client_position);
		P_ENGINE_CLIENT_ANARCHY->GetClientChar()->SetRotation(new_rotation);
	}

	void Actor::DoTarget()
	{
		pTargetingModule->SetTarget(GetIdentity(), false);
	}

	float Actor::EstimatedDistanceTo(VECTOR3 &position)
	{
		VECTOR3 dynel_position = GetPosition();
		float uncompensated_distance = GetDistanceTo(position);
		float x = position.X - (dynel_position.X + GetVehicle()->GetVelocity().X*uncompensated_distance);
		float z = position.Z - (dynel_position.Z + GetVehicle()->GetVelocity().Z*uncompensated_distance);
		VECTOR3 estimated_position;
		estimated_position.X = x;
		estimated_position.Y = 0.0f;
		estimated_position.Z = z;
		return estimated_position.Length();
	}

	NanoTemplate* Actor::GetNCU(DWORD index)
	{
		std::vector<NanoTemplate> nano_template_vector;
		GetSpellTemplateData()->GetNanoTemplateList(nano_template_vector);
		if (index < 0 || index >= nano_template_vector.size())
			return nullptr;
		return static_cast<NanoTemplate*>(&(nano_template_vector[index]));
	}

	NanoTemplate* Actor::GetNCU(PCSTR effect_name)
	{
		IDENTITY dummy_identity;
		ZeroMemory(&dummy_identity, sizeof(IDENTITY));
		char name[MAX_STRING] = { 0 };
		char search_name[MAX_STRING];
		strcpy_s(search_name, MAX_STRING, effect_name);
		_strlwr_s(search_name);
		std::vector<NanoTemplate> nano_template_vector;
		GetSpellTemplateData()->GetNanoTemplateList(nano_template_vector);
		for (auto it = nano_template_vector.begin(); it != nano_template_vector.end(); ++it)
		{
			auto entry = static_cast<NanoTemplate*>(&(*it));
			strcpy_s(name, MAX_STRING, P_ENGINE_CLIENT_ANARCHY->N3Msg_GetName(entry->GetNanoIdentity(), dummy_identity));
			_strlwr_s(name);
			if (strstr(name, search_name))
				return entry;
		}
		return nullptr;
	}

	DWORD Actor::GetNCUCount()
	{
		std::vector<NanoTemplate> nano_template_vector;
		GetSpellTemplateData()->GetNanoTemplateList(nano_template_vector);
		return nano_template_vector.size();
	}

	SpellTemplateData* Actor::GetSpellTemplateData()
	{
		return reinterpret_cast<SpellTemplateData*>(GetSimpleCharData()->pSpellTemplateData);
	}

	float Actor::GetScale()
	{
		return GetSimpleCharData()->BodyScale;
	}

	PSIMPLECHAR Actor::GetSimpleCharData()
	{
		return PSIMPLECHAR(GetData());
	}

	Vehicle* Actor::GetVehicle()
	{
		return reinterpret_cast<Vehicle*>(GetSimpleCharData()->pVehicle);
	}

	bool Actor::IsBackingUp()
	{
		return GetVehicle()->GetCharMovementStatus()->IsMovingFwdBck != 1 && GetVehicle()->GetCharMovementStatus()->FwdBckDirection == 2;
	}

	bool Actor::IsCasting()
	{
		return GetSpellTemplateData()->IsCasting();
	}

	bool Actor::IsFighting()
	{
		return GetSimpleCharData()->pWeaponHolder->IsAttacking != 1;

	}

	bool Actor::IsFightingMe()
	{
		return IsClientId(GetSimpleCharData()->pWeaponHolder->WeaponTargetIdentity.Id);
	}

	bool Actor::IsIdle()
	{
		return GetVehicle()->GetCharMovementStatus()->IsMovingFwdBck == 1 && GetVehicle()->GetCharMovementStatus()->IsMovingStrafe == 1;
	}

	bool Actor::IsInMyTeam()
	{
		return IsPlayer() && IsInTeam() && (GetTeamRaid()->GetTeamIdentity() == P_ENGINE_CLIENT_ANARCHY->GetClientChar()->GetTeamRaid()->GetTeamIdentity());
	}

	bool Actor::IsInTeam()
	{
		if(IsPlayer())
			return GetTeamRaid()->GetTeamIdentity().Type != 0;
		return false;
	}

	bool Actor::IsInRaid()
	{
		return IsPlayer() && IsInTeam() && GetTeamRaid()->GetTeamRaidIndex() != -1;
	}

	bool Actor::IsInMyRaidTeam()
	{
		if(IsTeamMember() && IsInRaid())
		{
			std::vector<TeamEntry*> v;
			P_ENGINE_CLIENT_ANARCHY->GetClientChar()->GetTeamRaid()->GetTeam(v);
			for (auto it = v.begin(); it != v.end(); ++it)
			{
				if ((*it)->GetIdentity() == GetIdentity())
					return true;
			}
		}		
		return false;
	}

	bool Actor::IsInvis()
	{
		return !(GetSimpleCharData()->IsVisible);
	}

	bool Actor::IsKOS()
	{
		return GetSkill(ST_BREEDHOSTILITY) > 30;
	}

	bool Actor::IsMovingForward()
	{
		return GetVehicle()->GetCharMovementStatus()->IsMovingFwdBck != 1 && GetVehicle()->GetCharMovementStatus()->FwdBckDirection == 1;
	}

	bool Actor::IsNPC()
	{
		return GetSimpleCharData()->IsNPC == 1;
	}

	bool Actor::IsStrafingLeft()
	{
		return GetVehicle()->GetCharMovementStatus()->IsMovingStrafe != 1 && GetVehicle()->GetCharMovementStatus()->StrafeDirection == 3;
	}

	bool Actor::IsStrafingRight()
	{
		return GetVehicle()->GetCharMovementStatus()->IsMovingStrafe != 1 && GetVehicle()->GetCharMovementStatus()->StrafeDirection == 4;
	}
	
	bool Actor::IsTeamLeader()
	{
		if(IsPlayer())
		{
			auto result = false;
			if (P_ENGINE_CLIENT_ANARCHY && IsInTeam() && IsInMyTeam())
				result = P_ENGINE_CLIENT_ANARCHY->N3Msg_IsTeamLeader(GetIdentity());
			return result;
		}
		return false;
	}
	
	DWORD Actor::GetMasterId()
	{
		return GetSkill(ST_PETMASTER);
	}

	Actor* Actor::GetMaster()
	{
		if (!IsPet())
			return nullptr;
		IDENTITY master;
		DWORD id = 0;
		master.Type = 50000;
		id = GetSkill(ST_PETMASTER);
		master.Id = id;
		return isxao_utilities::GetDynel(master)->ToActor();
	}

	Actor* Actor::GetPet(DWORD index)
	{
		std::map<IDENTITY, DWORD> pet_map;
		if (GetPetIDs(pet_map) == 0)
			return nullptr;
		if (index < 0 || index >= pet_map.size())
			return nullptr;
		for (auto it = pet_map.begin(); it != pet_map.end(); ++it)
		{
			if (it->second == index)
				return isxao_utilities::GetDynel(it->first)->ToActor();
		}
		return nullptr;
	}

	Actor* Actor::GetPet(PCSTR pet_name)
	{
		IDENTITY dummy_identity;
		std::map<IDENTITY, DWORD> pet_map;
		char name[MAX_STRING];
		char search_name[MAX_STRING];
		if (GetPetIDs(pet_map) == 0)
			return nullptr;		
		ZeroMemory(&dummy_identity, sizeof(IDENTITY));
		strcpy_s(search_name, MAX_STRING, pet_name);
		_strlwr_s(search_name);
		for (auto it = pet_map.begin(); it != pet_map.end(); ++it)
		{
			strcpy_s(name, MAX_STRING, P_ENGINE_CLIENT_ANARCHY->N3Msg_GetName((*it).first, dummy_identity));
			_strlwr_s(name);
			if (strstr(name, search_name))
				return isxao_utilities::GetDynel((*it).first)->ToActor();
		}
		return nullptr;
	}

	DWORD Actor::GetPetCount()
	{
		std::map<IDENTITY, DWORD> pet_map;
		return GetPetIDs(pet_map);
	}

	DWORD Actor::GetPetIDs(std::map<IDENTITY, DWORD>& m)
	{
		std::map<IDENTITY, DWORD> pet_map;
		if (!IsClientId(GetIdentity().Id))
		{
			std::map<IDENTITY, PN3DYNEL> dynel_map;
			GetDynelMap(dynel_map);
			DWORD pet_count = 0;
			for (auto it = dynel_map.begin(); it != dynel_map.end(); ++it)
			{
				if (it->first.Type == 50000)
				{
					auto is_npc = P_ENGINE_CLIENT_ANARCHY->N3Msg_IsNpc(it->first);
					if (is_npc)
					{
						auto dynel = reinterpret_cast<Dynel*>(it->second);
						if (dynel->GetSkill(ST_PETMASTER) == GetIdentity().Id)
						{
							pet_map.insert_or_assign(it->first, pet_count);
							pet_count++;
						}
					}
				}
			}
			return pet_map.size();
		}
		isxao_utilities::GetPetMap(m, ToCharacter()->GetNPCHolder()->GetNPCHolderData().pPetDir);
		return m.size();
	}

	bool Actor::HasPet()
	{
		if(IsCharacter())
		{
			std::map <IDENTITY, DWORD> pet_map;
			isxao_utilities::GetPetMap(pet_map, ToCharacter()->GetNPCHolder()->GetNPCHolderData().pPetDir);
			if (pet_map.size() == 0)
				return false;
			return true;
		}
		std::map<IDENTITY, PN3DYNEL> dynel_map;
		isxao_utilities::GetDynelMap(dynel_map);
		for (auto it = dynel_map.begin(); it != dynel_map.end(); ++it)
		{
			if (it->first.Type == 50000)
			{
				auto is_npc = P_ENGINE_CLIENT_ANARCHY->N3Msg_IsNpc(it->first);
				if (is_npc)
				{
					auto dynel = reinterpret_cast<Dynel*>(it->second);
					if (dynel->GetSkill(ST_PETMASTER) == GetIdentity().Id)
						return true;
				}
			}
		}
		return false;
	}

	TeamRaid* Actor::GetTeamRaid()
	{
		if(IsPlayer())
			return reinterpret_cast<TeamRaid*>(GetSimpleCharData()->pTeamRaidInfo);
		return nullptr;
	}

	WeaponHolder* Actor::GetWeaponHolder()
	{
		return reinterpret_cast<WeaponHolder*>(GetSimpleCharData()->pWeaponHolder);
	}

	void Actor::Kick()
	{
		if(IsTeamMember())
			P_ENGINE_CLIENT_ANARCHY->N3Msg_KickTeamMember(GetIdentity());
	}

	void Actor::MakeLeader()
	{
		if(IsTeamMember())
			P_ENGINE_CLIENT_ANARCHY->GetClientChar()->MakeTeamLeader(GetIdentity());
	}

	bool Actor::SendTeamInvite()
	{
		if(IsPlayer())
			return P_ENGINE_CLIENT_ANARCHY->N3Msg_TeamJoinRequest(GetIdentity(), true);
		return false;
	}	

}