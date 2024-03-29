#include "isxao_main.h"
#include "playfield_anarchy.h"

bool PlayfieldType::GetMember(LSOBJECTDATA ObjectData, PLSTYPEMEMBER Member, int argc, char* argv[], LSOBJECT& Object)
{
	if (ao::g_game_state != GAMESTATE_IN_GAME)
		return false;
	if (!ObjectData.Ptr)
		return false;
	__try
	{
#define P_PLAYFIELD ((ao::playfield_anarchy*)ObjectData.Ptr)  // NOLINT(cppcoreguidelines-macro-usage)
		switch (PlayfieldTypeMembers(Member->ID))
		{
		case Name:
		{
			Object.ConstCharPtr = P_PLAYFIELD->get_playfield_name();
			Object.Type = pStringType;
			break;
		}
		case ID:
		{
			Object.DWord = P_PLAYFIELD->get_playfield_instance();
			Object.Type = pUintType;
			break;
		}
		default:
			return false;
		}
#undef pIdentity
	}
	__except (pExtension->HandleLSTypeCrash(__FUNCTION__, Member->ID, ObjectData.Ptr, argc, argv, GetExceptionCode(), GetExceptionInformation()))
	{

	}
	return true;
}

bool PlayfieldType::ToText(LSOBJECTDATA ObjectData, char* buf, unsigned buflen)
{
	if (ao::g_game_state != GAMESTATE_IN_GAME)
		return false;
	if (!ObjectData.Ptr)
		return false;
#define P_PLAYFIELD ((ao::playfield_anarchy*)ObjectData.Ptr)  // NOLINT(cppcoreguidelines-macro-usage)
	sprintf_s(buf, buflen, "%s", P_PLAYFIELD->get_playfield_name());
#undef pIdentity
	return true;
}