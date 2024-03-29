#include "isxao_main.h"
#include "surface_i.h"
#include "character.h"
#include "dynel.h"
#include "engine_client_anarchy.h"
#include "playfield_anarchy.h"
#include "vehicle.h"


#pragma region Testing and Debugging

int CMD_AO(int argc, char *argv[])
{
	// printf("%" PRIX32, P_PLAYFIELD_DIR->get_playfield()->get_cell_surface());
	//const auto func = reinterpret_cast<int(*)(void*, const ao::vector3_t&, const ao::vector3_t&, ao::vector3_t&, bool, void*)>(ao::cell_surface_t__get_line_intersection);
	//auto this_ptr = P_PLAYFIELD_DIR->get_playfield()->get_tile_map_surface()->p_cell_surface;
	const auto v_1 = P_ENGINE_CLIENT_ANARCHY->get_client_char()->to_dynel()->get_position();
	const ao::vector3_t offset(5.0f, 0.0f, 5.0f);
	const auto v_2 = ao::vector3_t::add(v_1, offset);	
	ao::vector3_t r_1;
	ao::quaternion_t q = P_ENGINE_CLIENT_ANARCHY->get_client_char()->get_facing_to(r_1);
	ao::vector3_t r_2;
	ao::gp_navigation->activate(v_2);
	ao::gp_navigation->set_active(true);
	// printf("%" PRIX32, );
	return 0;
}

int CMD_TESTINTERFACE(int argc, char *argv[])
{
	printf("0x%.8X", sizeof(string));
	return 0;
}

int CMD_DUMPSPECIALACTIONS(int argc, char *argv[])
{
	
	return 0;
}

int CMD_DUMPSTATS(int argc, char *argv[])
{
	ao::identity_t id;
	ao::identity_t dummy;
	if (P_ENGINE_CLIENT_ANARCHY->get_client_char()->get_target(id))
	{
		FILE * pFILE;
		fopen_s(&pFILE, "stats.txt", "a");
		std::map<ao::stat_e, const char *> m;
		P_STAT_NAME_MAP->copy_map(m);
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			char buffer[MAX_VARSTRING];
			LONG stat = P_ENGINE_CLIENT_ANARCHY->n3_msg_get_skill(id, static_cast<ao::stat_e>((*it).first), 2, dummy);
			sprintf_s(buffer, "ST_%s = %d,\n", (*it).second, /*(*it).first*/stat);
			for (auto i = 0; i < (int)strlen(buffer); i++)
				buffer[i] = toupper(buffer[i]);
			fputs(buffer, pFILE);

		}
		fclose(pFILE);
	}
	//if (P_ENGINE_CLIENT_ANARCHY->GetWeaponTarget(id))
	//{
	//	FILE * pFILE;
	//	pFILE = fopen("stats.txt", "a");
	//	std::map<DWORD, const char *> m;
	//	P_ENGINE_CLIENT_ANARCHY->GetStatNameMap(m);
	//	for (auto it = m.begin(); it != m.end(); ++it)
	//	{
	//		char buffer[MAX_VARSTRING];
	//		LONG stat = P_ENGINE_CLIENT_ANARCHY->GetSkill(id, static_cast<AOData::stat_e>((*it).first));
	//		if (stat != 1234567890)
	//		{
	//			sprintf_s(buffer, "PS_%s = %d,\n", (*it).second, (*it).first);
	//			for (auto i = 0; i < strlen(buffer); i++)
	//				buffer[i] = toupper(buffer[i]);
	//			fputs(buffer, pFILE);
	//		}

	//	}
	//	fclose(pFILE);
	//}
	return 0;
}

int CMD_ATTACK(int argc, char *argv[])
{
	ao::identity_t p1;
	ao::identity_t p2;
	P_ENGINE_CLIENT_ANARCHY->n3_msg_get_client_pet_id(p1, 0);
	printf("%d", p1.id);
	P_ENGINE_CLIENT_ANARCHY->n3_msg_get_client_pet_id(p2, 1);
	printf("%d", p2.id);
	return 0;
}

int CMD_TESTSTRUCTS(int argc, char *argv[])
{	
	printf("Testing struct sizes...");

	DWORD r = sizeof(ao::bank_entry_t);
	DWORD a = 0x38;
	printf("bank_entry_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::binary_stream_t);
	a = 0x44;
	printf("binary_stream_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::camera_vehicle_t);
	a = 0x1EC;
	printf("camera_vehicle_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::char_movement_status_t);
	a = 0x34;
	printf("char_movement_status_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::char_vehicle_t);
	a = 0x3AC;
	printf("char_vehicle_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::chest_t);
	a = 0x1E8;
	printf("chest_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::coll_info_t);
	a = 0x14;
	printf("coll_info_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::coll_prim_t);
	a = 0x18;
	printf("coll_prim_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::coll_sphere_t);
	a = 0x1C;
	printf("coll_sphere_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::db_object_t);
	a = 0x18;
	printf("db_object_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::dummy_item_base_t);
	a = 0xA4;
	printf("dummy_item_base_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::dummy_vehicle_t);
	a = 0x160;
	printf("dummy_vehicle_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");	

	r = sizeof(ao::event_caster_t);
	a = 0x18;
	printf("event_caster_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::flow_control_t);
	a = 0x58;
	printf("flow_control_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::game_time_t);
	a = 0xC8;
	printf("game_time_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::hot_spot_t);
	a = 0x54;
	printf("hot_spot_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::identity_t);
	a = 0x8;
	printf("identity_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::input_config_t);
	a = 0x1D0;
	printf("input_config_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::inventory_data_t);
	a = 0x2C;
	printf("inventory_data_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::inventory_holder_t);
	a = 0x1B8;
	printf("inventory_holder_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::i_point_t);
	a = 0x8;
	printf("i_point_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::mesh_coll_sphere_t);
	a = 0x1C;
	printf("mesh_coll_sphere_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_camera_t);
	a = 0x258;
	printf("n3_camera_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_cell_monitor_t);
	a = 0x38;
	printf("n3_cell_monitor_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_dynel_t);
	a = 0xA0;
	printf("n3_dynel_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_dynel_event_source_t);
	a = 0x8;
	printf("n3_dynel_event_source_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_dyna_mesh_coll_sphere_t);
	a = 0x30;
	printf("n3_dyna_mesh_coll_sphere_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_engine_client_anarchy_t);
	a = 0x130;
	printf("n3_engine_client_anarchy_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_f_obj_t);
	a = 0x18;
	printf("n3_f_obj_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_fsm_t);
	a = 0x1C;
	printf("n3_fsm_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_object_factory_t);
	a = 0x8;
	printf("n3_object_factory_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_root_t);
	a = 0x3C;
	printf("n3_root_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_sf_base_t);
	a = 0x8;
	printf("n3_sf_base_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_tile_map_t);
	a = 0x424;
	printf("n3_tile_map_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::n3_visual_dynel_t);
	a = 0xE0;
	printf("n3_visual_dynel_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::nano_item_t);
	a = 0xC0;
	printf("nano_item_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::nano_template_t);
	a = 0x1C;
	printf("nano_template_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::new_inventory_t);
	a = 0x2C;
	printf("new_inventory_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::playfield_anarchy_t);
	a = 0x110;
	printf("playfield_anarchy_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::quaternion_t);
	a = 0x10;
	printf("quaternion_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::rdb_identity_t);
	a = 0xC;
	printf("rdb_identity_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::rdb_playfield_t);
	a = 0x6C;
	printf("rdb_playfield_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::rdb_tile_map_t);
	a = 0x8270;
	printf("rdb_tile_map_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::reference_counted_t);
	a = 0x8;
	printf("reference_counted_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::simple_char_t);
	a = 0x2D8;
	printf("simple_char_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::spell_template_data_t);
	a = 0x50;
	printf("spell_and_template_data_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::vector3_t);
	a = 0xC;
	printf("vector3_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::weapon_holder_t);
	a = 0xA4;
	printf("weapon_holder_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	r = sizeof(ao::weapon_item_t);
	a = 0x204;
	printf("weapon_item_t: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");
	
	r = sizeof(ao::character);
	a = sizeof(ao::player);
	printf("LocalPlayer: 0x%.8X. Should be 0x%.8X - %s", r, a, r == a ? "Passed" : "FAILED!");

	return 0;
}

#pragma endregion

#pragma region Game Commands

int CMD_ACTIVATE(int argc, char *argv[])
{
	if(ao::g_game_state == GAMESTATE_IN_GAME)
		return Activate(1, argc, argv);
	return 0;
}

int CMD_AOECHO(int argc, char *argv[])
{
	if (ao::g_game_state == GAMESTATE_IN_GAME)
		return AOEcho(argc, argv);
	return 0;
}

int CMD_AOEXECUTE(int argc, char *argv[])
{
	if (ao::g_game_state == GAMESTATE_IN_GAME)
		return AOExecute(argc, argv);
	return 0;
}

int CMD_CAST(int argc, char *argv[])
{
	if (ao::g_game_state == GAMESTATE_IN_GAME)
		return Cast(1, argc, argv);
	return 0;
}

int CMD_FACE(int argc, char *argv[])
{ 
	if (ao::g_game_state == GAMESTATE_IN_GAME)
		return Face(argc, argv);
	return 0;
}

int CMD_TARGET(int argc, char *argv[])
{
	if (ao::g_game_state == GAMESTATE_IN_GAME)
		return Target(argc, argv);
	return 0;
}

int CMD_MOVE_TO(int argc, char *argv[])
{
	if (ao::g_game_state == GAMESTATE_IN_GAME)
		return MoveTo(argc, argv);
	return 0;
}

#pragma endregion