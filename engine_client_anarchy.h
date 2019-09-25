#pragma once

// ReSharper disable once CppClassNeedsConstructorBecauseOfUninitializedMember
class engine_client_anarchy
{
public:
	static PCSTR get_breed_str(ao::breed_e breed_id);
	ao::character* get_client_char();
	ao::p_identity_t get_client_dynel_id(ao::identity_t &id);
	DWORD get_current_movement_mode();
	ao::n3_engine_client_anarchy_t get_engine_client_anarchy_data() const;
	PCSTR get_faction_str(DWORD faction_id, bool full_name);
	PCSTR get_faction_title(DWORD faction_id);
	PCSTR get_gender_string(ao::identity_t const &id);
	ao::p_dummy_item_base_t get_item_by_template(ao::identity_t template_id, const ao::identity_t &container_id);
	int get_last_speed_mode();
	static PCSTR get_item_rarity_str(DWORD rarity_id);
	static ao::p_nano_item_t get_nano_item(DWORD nano_id);
	static PCSTR get_nano_school_str(DWORD school_id);
	static PCSTR get_profession_str(ao::profession_e prof_id);
	static PCSTR get_sex_str(ao::gender_e gender_id);
	static PCSTR get_side_str(ao::side_e side_id);
	static PCSTR get_title_string(DWORD base, DWORD level);
	bool is_first_login();
	bool is_fixture(const ao::identity_t &template_id, const ao::identity_t &container_id);
	void n3_msg_activate_mech(DWORD mech_id);
	void n3_msg_airstrike(const ao::identity_t &target_id);
	void n3_msg_artillery_attack(const ao::identity_t &target_id);
	void n3_msg_assist_fight(const ao::identity_t &assist_id);
	bool n3_msg_can_attack(ao::identity_t const &target_id);
	bool n3_msg_can_click_target_target(const ao::identity_t &identity_1, const ao::identity_t &identity_2);
	bool n3_msg_can_use_mech();
	void n3_msg_cast_nano_spell(const ao::identity_t &, const ao::identity_t &);
	DWORD n3_msg_consider(const ao::identity_t &, float &);
	void n3_msg_container_add_item(const ao::identity_t &, const ao::identity_t &);
	void n3_msg_crawl_toggle();
	void n3_msg_create_raid();
	void n3_msg_default_action_on_dynel(const ao::identity_t &);
	void n3_msg_default_attack(const ao::identity_t &, bool);
	void n3_msg_delete_item(const ao::identity_t &, const ao::identity_t &);
	void n3_msg_delete_nano(const ao::identity_t &);
	DWORD n3_msg_do_social_action(ao::social_action_e);
	void n3_msg_drop_item(const ao::identity_t &, const ao::vector3_t &);
	void n3_msg_duel_accept();
	void n3_msg_duel_challenge(const ao::identity_t &);
	void n3_msg_duel_draw();
	void n3_msg_duel_refuse();
	void n3_msg_duel_stop();
	void n3_msg_forage();
	DWORD n3_msg_get_action_by_name(PCSTR);
	float n3_msg_get_action_progress(const ao::identity_t &, DWORD*, DWORD*);
	LONG n3_msg_get_agg_def();
	PCSTR n3_msg_get_alien_level_string(const ao::identity_t &);
	void n3_msg_get_area_name(PCSTR*);
	bool n3_msg_get_attacking_id(ao::identity_t &);
	PCSTR n3_msg_get_breed_str(const ao::identity_t &);
	DWORD n3_msg_get_buff_current_time(const ao::identity_t &nano_id, const ao::identity_t &character_id);
	DWORD n3_msg_get_buff_total_time(const ao::identity_t &, const ao::identity_t &);
	bool n3_msg_get_character_body_shape(BYTE &, ao::identity_t);
	bool n3_msg_get_char_orientation_data(const ao::identity_t &, ao::vector3_t &, ao::quaternion_t &, float &);
	PCSTR n3_msg_get_city_name_for_clan_member(const ao::identity_t &);
	PCSTR n3_msg_get_clan_level_string(const ao::identity_t &);
	string* n3_msg_get_clan_string(const ao::identity_t &);
	ao::p_identity_t n3_msg_get_client_pet_id(ao::identity_t &, DWORD);
	bool n3_msg_get_close_target(ao::identity_t &, bool, bool);
	ao::p_inventory_holder_t n3_msg_get_container_inventory_list(const ao::identity_t &);
	void n3_msg_get_correct_action_id(ao::identity_t &);
	LONG n3_msg_get_current_room();
	PCSTR n3_msg_get_current_room_name();
	DWORD n3_msg_get_district_fight_mode();
	void n3_msg_get_dynels_in_vicinity(std::vector<ao::identity_t>&, bool, bool, ao::type_id_e);
	bool n3_msg_get_faction_info_string(const ao::identity_t &, string &);
	bool n3_msg_get_faction_range(DWORD, DWORD &, DWORD &);
	PCSTR n3_msg_get_first_name(const ao::identity_t &);
	double n3_msg_get_formula_progress(const ao::identity_t &, DWORD &, DWORD &);
	DWORD n3_msg_get_formula_radius(const ao::identity_t &);
	static DWORD n3_msg_get_full_perk_map();
	void n3_msg_get_global_character_position(ao::vector3_t &);
	void n3_msg_get_global_character_rotation(ao::quaternion_t &);
	PVOID n3_msg_get_grid_destination_list(const ao::identity_t &);
	PVOID n3_msg_get_grid_destination_list();
	std::vector<ao::p_inventory_data_t>* n3_msg_get_inventory_vec(const ao::identity_t &);
	void n3_msg_get_item(const ao::identity_t &);
	double n3_msg_get_item_progress(const ao::identity_t &, DWORD &, DWORD &);
	DWORD n3_msg_get_movement_mode();
	PCSTR n3_msg_get_name(const ao::identity_t &, const ao::identity_t &);
	DWORD n3_msg_get_nano_cost_modifier();
	std::list<DWORD>* n3_msg_get_nano_spell_list();
	std::list<ao::nano_template_t>* n3_msg_get_nano_template_info_list(const ao::identity_t &);
	std::list<ao::nano_template_t>* n3_msg_get_nano_template_info_list();
	ao::p_identity_t n3_msg_get_next_target(ao::identity_t &, DWORD);
	DWORD n3_msg_get_number_of_available_alien_perks();
	DWORD n3_msg_get_number_of_available_perks();
	DWORD n3_msg_get_number_of_free_inventory_slots();
	DWORD n3_msg_get_number_of_used_alien_perks();
	DWORD n3_msg_get_number_of_used_perks();
	DWORD n3_msg_get_over_equip_level(const ao::identity_t &);
	PCSTR n3_msg_get_pf_name(DWORD);
	PCSTR n3_msg_get_pf_name();
	bool n3_msg_get_parent(const ao::identity_t &, ao::identity_t &);
	bool n3_msg_get_pos(const ao::identity_t &, ao::vector3_t &);
	LONG n3_msg_get_skill(DWORD, DWORD);
	LONG n3_msg_get_skill(const ao::identity_t &, DWORD, DWORD, const ao::identity_t &);
	LONG n3_msg_get_skill_max(DWORD);
	std::list<ao::special_action_t>* n3_msg_get_special_action_list();
	DWORD n3_msg_get_special_action_state(const ao::identity_t &);
	PCSTR n3_msg_get_special_attack_weapon_name(const ao::identity_t &);
	static PVOID n3_msg_get_stat_name_map();
	ao::p_identity_t N3Msg_GetTargetTarget(ao::identity_t &, const ao::identity_t &);
	PVOID n3_msg_get_team_member_list(LONG);
	bool n3_msg_has_perk(DWORD);
	void n3_msg_hide_against_opponent(const ao::identity_t&);
	bool n3_msg_is_attacking();
	bool n3_msg_is_battle_station();
	bool n3_msg_is_character_in_mech(ao::identity_t);
	bool n3_msg_is_character_morphed(ao::identity_t);
	bool n3_msg_is_dungeon();
	bool n3_msg_is_formula_ready(const ao::identity_t &);
	bool n3_msg_is_general_perk(DWORD);
	bool n3_msg_is_group_perk(DWORD);
	bool n3_msg_is_in_raid_team();
	bool n3_msg_is_in_team(const ao::identity_t &);
	bool n3_msg_is_item_disabled(const ao::identity_t &, const ao::identity_t &);
	bool n3_msg_is_item_mine(const ao::identity_t &);
	bool n3_msg_is_item_nf_crystal(const ao::identity_t &, const ao::identity_t &);
	bool n3_msg_is_item_possible_to_un_wear(const ao::identity_t &);
	bool n3_msg_is_item_possible_to_wear(const ao::identity_t &, DWORD, DWORD, bool, DWORD);
	bool n3_msg_is_moving();
	bool n3_msg_is_my_pet_id(const ao::identity_t &);
	bool n3_msg_is_nano_self_only(const ao::identity_t &);
	bool n3_msg_is_npc(const ao::identity_t &);
	bool n3_msg_is_perk(DWORD);
	bool N3Msg_IsPetTower(const ao::identity_t &);
	bool N3Msg_IsProfessionPerk(DWORD);
	bool N3Msg_IsResearch(DWORD);
	bool N3Msg_IsSecondarySpecialAttackAvailable(DWORD);
	bool N3Msg_IsShieldDisablerItem(const ao::identity_t &);
	bool N3Msg_IsSpecialPerk(DWORD);
	bool N3Msg_IsTeamLeader(const ao::identity_t &);
	bool N3Msg_IsTeamMission(const ao::identity_t &);
	bool N3Msg_IsTeamMissionCopy(const ao::identity_t &);
	bool N3Msg_IsTeamNano(const ao::identity_t &);
	bool N3Msg_IsTower(const ao::identity_t &);
	bool N3Msg_IsVisible(const ao::identity_t &);
	bool N3Msg_JoinItems(const ao::identity_t &, const ao::identity_t &);
	void N3Msg_KickTeamMember(const ao::identity_t &);
	void N3Msg_LeaveBattle();
	void N3Msg_LeaveTeam();
	bool N3Msg_MeetsPerkCriteria(DWORD);
	bool N3Msg_MoveItemToInventory(const ao::identity_t &, DWORD, DWORD);
	void N3Msg_MoveRaidMember(const ao::identity_t &, DWORD);
	void n3_msg_movement_changed(ao::movement_action_e, float, float, bool);
	bool n3_msg_name_to_id(const std::string &, ao::identity_t &);
	void N3Msg_NPCChatAddTradeItem(const ao::identity_t &, const ao::identity_t &, const ao::identity_t &);
	void N3Msg_NPCChatCloseWindow(const ao::identity_t &, const ao::identity_t &);
	void N3Msg_NPCChatEndTrade(const ao::identity_t &, const ao::identity_t &, DWORD, bool);
	void N3Msg_NPCChatRemoveTradeItem(const ao::identity_t &, const ao::identity_t &, const ao::identity_t &);
	void N3Msg_NPCChatRequestDescription(const ao::identity_t &, const ao::identity_t &);
	void N3Msg_NPCChatStartTrade(const ao::identity_t &, const ao::identity_t &);
	void N3Msg_OrbitalAttack(const ao::identity_t &);
	bool n3_msg_perform_special_action(DWORD);
	bool n3_msg_perform_special_action(const ao::identity_t &);
	void N3Msg_PetDuel_Accept();
	void N3Msg_PetDuel_Challenge(const ao::identity_t &);
	void N3Msg_PetDuel_Refuse();
	void N3Msg_PetDuel_Stop();
	bool N3Msg_RemoveBuff(const ao::identity_t &);
	void N3Msg_RemoveQuest(const ao::identity_t &);
	std::list<ao::inventory_entry_t>* N3Msg_RequestCharacterInventory();
	std::list<ao::inventory_entry_t>* N3Msg_RequestClothInventory(const ao::identity_t &);
	std::list<ao::inventory_entry_t>* N3Msg_RequestImplantInventory(const ao::identity_t &);
	void n3_msg_request_info_packet(const ao::identity_t &);
	std::list<ao::inventory_entry_t>* N3Msg_RequestSocialInventory(const ao::identity_t &);
	std::list<ao::inventory_entry_t>* N3Msg_RequestWeaponInventory(const ao::identity_t &);
	bool N3Msg_SecondarySpecialAttack(const ao::identity_t &, DWORD);
	void N3Msg_SelectedTarget(const ao::identity_t&);
	void n3_msg_send_pet_command(DWORD, const ao::identity_t &, DWORD, DWORD, PCSTR); // 0, PetIdentity&, PetCommand_e, 0, nullptr
	void n3_msg_sit_toggle();
	bool N3Msg_SplitItem(const ao::identity_t &, DWORD);
	void N3Msg_StartAltState();
	bool N3Msg_StartCamping();
	void N3Msg_StartPvP(const ao::identity_t &);
	bool N3Msg_StartTreatment(const ao::identity_t &);
	void N3Msg_StopAltState();
	void n3_msg_stop_attack();
	void N3Msg_StopCamping();
	DWORD N3Msg_StringToStat(PCSTR);
	void N3Msg_SwitchTarget(const ao::identity_t &);
	bool N3Msg_TeamJoinRequest(const ao::identity_t &, bool);
	ao::p_identity_t N3Msg_TemplateIDToDynelID(ao::identity_t &, const ao::identity_t &); // dummy, template
	bool N3Msg_TextCommand(DWORD, PCSTR, const ao::identity_t &);
	void N3Msg_ToggleReclaim(bool);
	void N3Msg_TradeAbort(bool);
	void N3Msg_TradeAccept();
	void N3Msg_TradeAddItem(const ao::identity_t &, const ao::identity_t &, DWORD);
	void N3Msg_TradeConfirm();
	PVOID N3Msg_TradeGetInventory(const ao::identity_t &);
	DWORD N3Msg_TradeGetInventoryCost(const ao::identity_t &);
	void N3Msg_TradeRemoveItem(const ao::identity_t &, const ao::identity_t &);
	void N3Msg_TradeSetCash(DWORD);
	void N3Msg_TradeStart(const ao::identity_t &);
	void N3Msg_TradeskillCombine(const ao::identity_t &, const ao::identity_t &);
	bool N3Msg_TrainPerk(DWORD);
	void N3Msg_TransferTeamLeadership(const ao::identity_t &);
	void N3Msg_TryAbortNanoFormula();
	void N3Msg_TryEnterSneakMode();
	bool N3Msg_UntrainPerk(DWORD);
	void n3_msg_use_item(const ao::identity_t &, bool);
	void N3Msg_UseItemOnItem(const ao::identity_t &, const ao::identity_t &);

	private:
	// ReSharper disable once CppUninitializedNonStaticDataMember
	ao::n3_engine_client_anarchy_t n3_engine_client_anarchy_;

};
