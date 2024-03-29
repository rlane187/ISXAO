#include "isxao_main.h"
#include "engine_client_anarchy.h"
#include "special_action_template.h"

namespace ao
{

	identity_t special_action_template::get_identity()
	{
		return get_data()->identity;
	}

	DWORD special_action_template::get_locked_skill_id_1()
	{
		return get_data()->locked_skill_id1;
	}

	DWORD special_action_template::get_locked_skill_id_2()
	{
		return get_data()->locked_skill_id2;
	}

	DWORD special_action_template::get_lockout_time_remaining()
	{
		DWORD a, b;
		const auto p_a = &a;
		const auto p_b = &b;
		P_ENGINE_CLIENT_ANARCHY->n3_msg_get_action_progress(this->get_identity(), p_a, p_b);
		return a;
	}

	PCSTR special_action_template::get_name()
	{
		const identity_t d(0, 0);
		return P_ENGINE_CLIENT_ANARCHY->n3_msg_get_name(this->get_identity(), d);
	}

	special_action_item* special_action_template::get_special_action_item()
	{
		const identity_t d(0, 0);
		return reinterpret_cast<special_action_item*>(P_ENGINE_CLIENT_ANARCHY->get_item_by_template(this->get_identity(), d));
	}


	identity_t special_action_template::get_weapon_identity()
	{
		return this->get_data()->weapon_identity;
	}

	bool special_action_template::is_locked()
	{
		return this->get_data()->is_locked != 0;;
	}

	double special_action_template::get_special_action_progress(DWORD &a, DWORD &b)
	{
		return P_ENGINE_CLIENT_ANARCHY->n3_msg_get_item_progress(get_identity(), a, b);
	}

	bool special_action_template::special_action_compare(special_action_template &a, special_action_template &b)
	{
		return a.get_identity().id < b.get_identity().id;
	}

	bool special_action_template::p_special_action_compare(special_action_template *a, special_action_template *b)
	{
		return a->get_identity().id < b->get_identity().id;
	}
	
}