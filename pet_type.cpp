#include "isxao_main.h"
#include "dynel.h"
#include "pet.h"

bool PetType::GetMember(LSOBJECTDATA ObjectData, PLSTYPEMEMBER Member, int argc, char *argv[], LSOBJECT &Object)
{
	if (ao::g_game_state != GAMESTATE_IN_GAME)
		return false;
	if (!ObjectData.Ptr)
		return false;
	__try
	{
#define P_PET ((ao::pet*)ObjectData.Ptr)  // NOLINT(cppcoreguidelines-macro-usage)
		switch (PetTypeMembers(Member->ID))
		{
		case Type:
		{
			Object.DWord = P_PET->get_pet_type();
			Object.Type = pUintType;
			break;
		}
		default:
			return false;
		}
	}
	__except (pExtension->HandleLSTypeCrash(__FUNCTION__, Member->ID, ObjectData.Ptr, argc, argv, GetExceptionCode(), GetExceptionInformation()))
	{

	}
	return true;
#undef P_PET
}

bool PetType::GetMethod(LSOBJECTDATA &ObjectData, PLSTYPEMETHOD pMethod, int argc, char *argv[])
{
	if (ao::g_game_state != GAMESTATE_IN_GAME)
		return false;
	if (!ObjectData.Ptr)
		return false;
	__try
	{
#define P_PET ((ao::pet*)ObjectData.Ptr)  // NOLINT(cppcoreguidelines-macro-usage)
		switch (PetTypeMethods(pMethod->ID))
		{
		case Attack:
		{
			P_PET->attack();
			break;
		}
		case Behind:
		{
			P_PET->behind();
			break;
		}
		case Follow:
		{
			P_PET->follow();
			break;
		}
		case Guard:
		{
			P_PET->guard();
			break;
		}
		case Heal:
		{
			P_PET->heal();
			break;
		}
		case Report:
		{
			P_PET->report();
			break;
		}
		case Terminate:
		{
			P_PET->terminate();
			break;
		}
		case Wait:
		{
			P_PET->wait();
			break;
		}
		default: break;
		}
#undef P_PET
	}
	__except (pExtension->HandleLSTypeCrash(__FUNCTION__, pMethod->ID, ObjectData.Ptr, argc, argv, GetExceptionCode(), GetExceptionInformation()))
	{

	}
	return true;
}

bool PetType::ToText(LSOBJECTDATA ObjectData, char *buf, unsigned int buflen)
{
	if (ao::g_game_state != GAMESTATE_IN_GAME)
		return false;
	if (!ObjectData.Ptr)
		return false;
#define P_PET ((ao::pet*)ObjectData.Ptr)  // NOLINT(cppcoreguidelines-macro-usage)
	sprintf_s(buf, buflen, "%I64u", P_PET->to_dynel()->get_identity().get_combined_identity());
#undef P_PET

	return true;
}