#pragma once

#include "identity_t.h"

namespace ao
{
#pragma region Forward Declarations

	class action_lock;
	class actor;
	class bank_entry;
	class casting_data;
	class character;
	class chat_gui_module;
	class chat_window_controller;
	class chat_window_node;
	class command_interpreter;
	class database_handler;
	class dummy_item_base;
	class dynel;
	class engine_client_anarchy;
	class flow_control;
	class game_time;
	class graph_path_finder;
	class html_parser;
	class info_item_remote;
	class input_config;
	class inventory_data;
	class inventory_holder;
	class inventory_item;
	struct inventory_slot;
	class message;
	class n3_message;
	class nano_item;
	class nano_template;
	class new_inventory;
	class npc_holder;
	class perk_holder;
	class pet;
	class player;
	class playfield_anarchy;
	class playfield_dir;
	class resource_database;
	class special_action_holder;
	class special_action_item;
	class special_action_template;
	class spell_template_data;
	class stat_holder;
	class targeting_module;
	class team_entry;
	class team_member;
	class team_raid;
	class text_message;
	class weapon_holder;
	class vehicle;

#pragma endregion


#include "ao_enums.h"

#pragma region Struct Definitions and Typedefines
	
#pragma region Common Structs (by alpha)

#ifndef _IDENTITY_T_H
#define _IDENTITY_T_H

	// Size = 0x08
	typedef struct ao_identity
	{
		unsigned long type;
		unsigned long id;

		unsigned long long get_combined_identity() const
		{
			return (static_cast<unsigned long long>(type) << 32) | id;
		}

		static struct ao_identity get_identity_from_combined(const unsigned long long combined)
		{
			struct ao_identity id;
			id.type = unsigned long(combined >> 32);
			id.id = unsigned long(combined);
			return id;
		}

		bool operator==(const struct ao_identity& other) const
		{
			return get_combined_identity() == other.get_combined_identity();
		}

		bool operator!=(const struct ao_identity& other) const
		{
			return get_combined_identity() != other.get_combined_identity();
		}

		bool operator<(const struct ao_identity& other) const
		{
			return get_combined_identity() < other.get_combined_identity();
		}

		void zero()
		{
			this->type = 0;
			this->id = 0;
		}

		void copy(const struct ao_identity& other)
		{
			this->type = other.type;
			this->id = other.id;
		}

		ao_identity()
		{
			type = 0;
			id = 0;
		}

		ao_identity(const unsigned long t, const unsigned long i)
		{
			type = t;
			id = i;
		}

	} identity_t, *p_identity_t;

#endif

	// Size = 0x0C
	typedef struct ao_vector3
	{
		float x;					// 0x00
		float y;					// 0x04
		float z;					// 0x08		

		static struct ao_vector3 add(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			struct ao_vector3 a;
			a.x = v1.x + v2.x;
			a.y = v1.y + v2.y;
			a.z = v1.z + v2.z;
			return a;
		}

		static struct ao_vector3 subtract(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			struct ao_vector3 a;
			a.x = v1.x - v2.x;
			a.y = v1.y - v2.y;
			a.z = v1.z - v2.z;
			return a;
		}

		void copy(const struct ao_vector3& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}

		void normalize()
		{
			const auto m = sqrt(x*x + y*y + z*z);
			x = x / m;
			y = y / m;
			z = z / m;
		}

		static struct ao_vector3 cross(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			struct ao_vector3 c;

			c.x = v1.y*v2.z - v1.z*v2.y;
			c.y = v1.z*v2.x - v1.x*v2.z;
			c.z = v1.x*v2.y - v1.y*v2.x;

			return c;
		}

		static float dot(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
		}

		float length() const
		{
			return sqrt(x*x + y*y + z*z);
		}

		float length_squared() const
		{
			return x*x + y*y + z*z;
		}

		static float distance(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			const auto r = subtract(v1, v2);
			return r.length();
		}

		static float distance_squared(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			const auto r = subtract(v1, v2);
			return r.length_squared();
		}

		static float distance_x(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			return abs(v1.x - v2.x);
		}

		static float distance_y(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			return abs(v1.y - v2.y);
		}

		static float distance_z(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			return abs(v1.z - v2.z);
		}

		static float distance_xz(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			const auto d_x = v1.x - v2.x;
			const auto d_z = v1.z - v2.z;
			return sqrt(d_x*d_x + d_z*d_z);
		}

		static float distance_xz_squared(const struct ao_vector3& v1, const struct ao_vector3& v2)
		{
			const auto d_x = v1.x - v2.x;
			const auto d_z = v1.z - v2.z;
			return d_x*d_x + d_z*d_z;
		}

		float get_raw_yaw() const
		{
			return atan2f(x, z);
		}

		float get_yaw() const
		{
			const auto is_normalized = this->is_normalized();
			ao_vector3 norm_v;
			if (!is_normalized)
			{				
				norm_v.copy(*this);
				norm_v.normalize();
			}			
			float heading;
			const auto raw_heading = is_normalized ? this->get_raw_yaw() : norm_v.get_raw_yaw();
			if (raw_heading > 0.0f)
				heading = float(raw_heading * 180.0f / M_PI);
			else
				heading = float(raw_heading * 180.0f / M_PI) + 360.0f;
			return heading;
		}

		float get_raw_pitch() const
		{
			ao_vector3 norm_v;
			const auto is_normalized = this->is_normalized();
			if (!is_normalized)
			{
				norm_v.copy(*this);
				norm_v.normalize();
			}			
			return asinf(is_normalized ? -this->y : -norm_v.y);
		}

		float get_pitch() const
		{
			return float(this->get_raw_pitch() * 180.0f / M_PI);
		}

		bool is_normalized() const
		{
			return this->x <= 1.0f && this->y <= 1.0f && this->z <= 1.0f;
		}

		void zero()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}

		ao_vector3()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		ao_vector3(const float& x, const float& y, const float& z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

	} vector3_t, *p_vector3_t;
	
	// Size = 0x10
	typedef struct ao_quaternion
	{
		float x;					// 0x00
		float y; // sin(theta/2)	// 0x04
		float z;					// 0x08
		float w; // cos(theta/2)	// 0x0C

		void copy(const struct ao_quaternion& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}

		void normalize()
		{
			const auto m = sqrt(x*x + y*y + z*z + w*w);
			x = x / m;
			y = y / m;
			z = z / m;
			w = w / m;
		}

		float get_raw_heading() const
		{
			return atan2f(2 * y*w - 2 * x*z, 1 - 2 * y*y - 2 * z*z);
		}

		float get_heading() const
		{
			float heading;
			const auto raw_heading = this->get_raw_heading();
			if (raw_heading > 0.0f)
				heading = float(raw_heading * 180.0f / M_PI);
			else
				heading = float(raw_heading * 180.0f / M_PI) + 360.0f;
			return heading;
		}

		float get_raw_pitch() const
		{
			return atan2f(2*(x*w - y*z), 1 - 2 * x*x - 2 * z*z);
		}

		float get_pitch() const
		{
			return float(this->get_raw_pitch() * 180.0f / M_PI);
		}

		static struct ao_quaternion get_quaternion_from_raw(const float& raw_yaw, const float& raw_pitch)
		{
			struct ao_quaternion q;

			const auto c1 = cosf(raw_yaw);
			const auto c2 = cosf(0);
			const auto c3 = cosf(raw_pitch);

			const auto s1 = sinf(raw_yaw);
			const auto s2 = sinf(0);
			const auto s3 = sinf(raw_pitch);

			q.w = sqrt(1 + c1*c2 + c1*c3 - s1*s2*s3 + c2*c3) / 2;
			q.x = (c2*s3 + c1*s3 + s1*s2*c3) / (4 * q.w);
			q.y = (s1*c2 + s1*c3 + c1*s2*s3) / (4 * q.w);
			q.z = (-s1*s3 + c1*s2*c3 + s2) / (4 * q.w);

			const auto m = sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);

			q.w = q.w / m;
			q.x = q.x / m;
			q.y = q.y / m;
			q.z = q.z / m;

			return q;
		}

		static struct ao_quaternion get_quaternion_from_raw(const float& raw_yaw)
		{
			struct ao_quaternion q;

			q.w = cosf(raw_yaw / 2);
			q.x = 0;
			q.y = sinf(raw_yaw / 2);
			q.z = 0;

			return q;
		}

		static struct ao_quaternion get_quaternion(const float& yaw)
		{
			float raw_yaw;
			if (yaw > 180.0f)
				raw_yaw = -1.0f * float((yaw - 180.0f) * M_PI / 180.0f);
			else
				raw_yaw = float(yaw * M_PI / 180.0f);
			return ao_quaternion::get_quaternion_from_raw(raw_yaw);
		}

		static struct ao_quaternion get_quaternion(const float& yaw, const float& pitch)
		{
			float raw_yaw;
			if (yaw > 180.0f)
				raw_yaw = -1.0f * float((yaw - 180.0f) * M_PI / 180.0f);
			else
				raw_yaw = float(yaw * M_PI / 180.0f);
			const auto raw_pitch = float(pitch * M_PI / 180.0f);
			return ao_quaternion::get_quaternion_from_raw(raw_yaw, raw_pitch);
		}

		static struct ao_quaternion get_quaternion_to_face(const vector3_t& to, const vector3_t& from)
		{
			// Get the unit vector pointing from the element at v1 and v2 and normalize it
			auto dir = vector3_t::subtract(to, from);
			dir.normalize();
			auto yaw = dir.get_raw_yaw();
			auto q = get_quaternion_from_raw(yaw);
			q.normalize();
			return q;
		}

		static struct ao_quaternion product(const struct ao_quaternion& a, const struct ao_quaternion& b)
		{			
			const auto w = a.w * b.w - a.x*b.x - a.y*b.y - a.z*b.z;
			const auto x = a.w*b.x + b.w*a.x + a.y*b.z - a.z*b.y;
			const auto y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
			const auto z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
			ao_quaternion q(w, x, y, z);
			return q;
		}

		void zero()
		{
			this->w = 0.0f;
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}

		ao_quaternion()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}

		explicit ao_quaternion(const struct ao_vector3& v)
		{
			auto const yaw = v.get_raw_yaw();
			auto const pitch = v.get_raw_pitch();
			auto const roll = 0.0f;
			const auto c1 = cosf(yaw);
			const auto c2 = cosf(roll);
			const auto c3 = cosf(pitch);

			const auto s1 = sinf(yaw);
			const auto s2 = sinf(roll);
			const auto s3 = sinf(pitch);

			w = sqrt(1 + c1*c2 + c1*c3 - s1*s2*s3 + c2*c3) / 2;
			x = (c2*s3 + c1*s3 + s1*s2*c3) / (4 * w);
			y = (s1*c2 + s1*c3 + c1*s2*s3) / (4 * w);
			z = (-s1*s3 + c1*s2*c3 + s2) / (4 * w);

			const auto m = sqrt(w*w + x*x + y*y + z*z);

			w = w / m;
			x = x / m;
			y = y / m;
			z = z / m;
		}

		ao_quaternion(const float w, const float x, const float y, const float z)
		{
			this->w = w;
			this->x = x;
			this->y = y;
			this->z = z;
		}

		bool operator==(const ao_quaternion& other)
		{
			return &*this == &other || (this->w == other.w && this->x == other.x && this->y == other.y && this->z == other.z);
		}

	} quaternion_t, *p_quaternion_t;	

	// Size = 0xC
	typedef struct ao_rdb_identity
	{
		DWORD low_quality_level_id;
		DWORD high_quality_level_id;
		DWORD quality_level;
	} rdb_identity_t, *p_rdb_identity_t;
	
	// Size = 0x8
	// From Utils.dll
	typedef struct ao_i_point
	{
		DWORD x;
		DWORD y;
	} i_point_t, *p_i_point_t;

#pragma region AOMap

	template <typename T, typename U>
	struct map_node
	{
		struct map_node* p_lower;
		struct map_node* p_back;
		struct map_node* p_higher;
		T key;
		U value;
	};

	template <typename T, typename U>
	struct map_root
	{
		struct map_node<T, U>* p_begin;
		struct map_node<T, U>* p_node;
		struct map_node<T, U>* p_end;
	};

	template <typename T, typename U>
	struct ao_map {
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x01[0x4];
		struct map_root<T, U>* p_root;
		DWORD count;

		DWORD copy_map(std::map<T, U>& m)
		{
			auto count = this->count;
			const auto p_root = this->p_root;
			const auto p_node = this->p_root->p_node;
			if (count > 0)
				this->recursive_add_to_map(m, p_node, p_root, count);
			return m.size();
		}

	private:
		void recursive_add_to_map(std::map<T, U>& m, struct map_node<T, U>* p_node, struct map_root<T, U>* p_root, DWORD& count)
		{
			m.insert_or_assign(p_node->key, p_node->value);
			count--;
			if (reinterpret_cast<PVOID>(p_node->p_lower) != reinterpret_cast<PVOID>(p_root) && count > 0)
				this->recursive_add_to_map(m, p_node->p_lower, p_root, count);
			if (reinterpret_cast<PVOID>(p_node->p_higher) != reinterpret_cast<PVOID>(p_root) && count > 0)
				recursive_add_to_map(m, p_node->p_higher, p_root, count);
		}
	};

#pragma endregion

#pragma region Maps

	typedef ao_map<identity_t, struct ao_n3_dynel*> dynel_map_t, *p_dynel_map_t;
	typedef ao_map<DWORD, struct ao_nano_item*> nano_item_map_t, *p_nano_item_map_t;	
	typedef ao_map<identity_t, struct ao_playfield_anarchy*> playfield_map, *p_playfield_map;
	typedef ao_map<string, class chat_window_node*> chat_window_node_map_t, *p_chat_window_node_map_t;
	typedef ao_map<DWORD, struct ao_playfield_anarchy*> playfield_dir_t, *p_playfield_dir_t;
	typedef ao_map<DWORD, struct ao_dummy_item_base*> weapon_item_dir_t, *p_weapon_item_dir_t;

	typedef ao_map<DWORD, PVOID> command_map_t, *p_command_map_t;

	typedef ao_map<identity_t, DWORD> perk_map_t, *p_perk_map_t;
	typedef ao_map<identity_t, DWORD> pet_map_t, *p_pet_map_t;
	typedef ao_map<stat_e, PCSTR> stat_name_map_t, *p_stat_name_map_t;
	typedef ao_map<DWORD, LONG> stat_map_t, *p_stat_map_t;
	typedef ao_map<DWORD, DWORD> lock_map_t, *p_lock_map_t;

#pragma endregion

#pragma region AOList

	template<typename T>
	struct ao_list_node
	{
		struct ao_list_node<T>* p_back;
		struct ao_list_node<T>* p_next;
		T value;
	};

	template<typename T>
	struct ao_list_root
	{
		struct ao_list_node<T>* p_first;
		struct ao_list_node<T>* p_last;
	};

	template<typename T>
	struct ao_list
	{
		struct ao_list_root<T>* p_root;
		DWORD count;

		DWORD copy_list(std::vector<T>& v)
		{
			if (this->count == 0)
				return this->count;
			auto count = this->count;
			struct ao_list_node<T>* current_node = this->p_root->p_first;
			while (count > 0 && current_node != this->p_root->p_last)
			{
				v.push_back(current_node->value);
				if (current_node != this->p_root->p_last && DWORD(current_node->p_next) != DWORD(this->p_root))
					current_node = current_node->p_next;
				count = count - 1;
			}
			return v.size();
		}
	};

#pragma region Lists

	typedef ao_list<DWORD> nano_id_list_t, *p_nano_list_id;

#pragma endregion

#pragma endregion

#pragma endregion

#pragma region ACGGameItem

	typedef struct ao_acg_game_item
	{
		rdb_identity_t rdb_identity;				// 0x00
		struct ao_dummy_item_base* p_dummy_item_base;	// 0x04
		DWORD interpolated_id;						// 0x10
	} acg_game_item_t, *p_acg_game_item_t;

#pragma endregion

#pragma region ActionLock

	// Size = 0x10
	typedef struct ao_action_lock  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		identity_t action_identity;		// 0x00
		DWORD total_lockout_time;		// 0x08
		DWORD lockout_time_remaining;	// 0x0C
	} action_lock_t, *p_action_lock_t;

#pragma endregion
	
#pragma region GameData::AreaData

	// Size = 0x5C
	// From GameData.dll
	typedef struct ao_area_data  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;				// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x4[0x4];			// 0x04
		std::string* p_name;			// 0x08
		std::string* p_description;		// 0x0C
		std::string* p_owner_name;		// 0x10
		std::string* p_knowledge;		// 0x14
		DWORD min_level;				// 0x18
		DWORD max_level;				// 0x1C
		PVOID p_environment_data;		// 0x20
		DWORD omni_tek_control;			// 0x24
		DWORD clan_control;				// 0x28
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x2C[0x1C];		// 0x2C
		struct ao_vector3 center;			// 0x48
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x54[0x8];			// 0x54
	} area_data_t, *p_area_data_t;

#pragma endregion

#pragma region BankEntry

	// Size = 0x38
	// From Gamecode.dll
	typedef struct ao_bank_entry
	{
		PVOID p_v_table;									// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x4];								// 0x04
		PVOID p_v_table_new_inventory;						// 0x08
		std::vector <struct ao_inventory_data*> inventory;		// 0x0C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0x4];								// 0x18
		DWORD count;										// 0x1C
		identity_t inventory_identity;						// 0x20
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x28[0x10];							// 0x28
	} bank_entry_t, *p_bank_entry_t;

#pragma endregion 
	
#pragma region BinaryStream

	// Size = 0x44
	// From BinaryStream.dll
	typedef struct ao_binary_stream
	{
		// ReSharper disable once CppInconsistentNaming
		// ReSharper disable once IdentifierTypo
		struct ao_binary_stream* (__thiscall *BinaryStream__readmemAddress)(ao_binary_stream*, PCSTR, size_t);	// 0x00
		// ReSharper disable once CppInconsistentNaming
		// ReSharper disable once IdentifierTypo
		struct ao_binary_stream* (__thiscall *BinaryStream__writememAddress)(ao_binary_stream*, BYTE, size_t);	// 0x04
		// ReSharper disable once CppInconsistentNaming
		// ReSharper disable once IdentifierTypo
		DWORD(__thiscall *BinaryStream__bgetcAddress)(ao_binary_stream*);										// 0x08
		// ReSharper disable once CppInconsistentNaming
		// ReSharper disable once IdentifierTypo
		DWORD(__thiscall *BinaryStream__bputcAddress)(ao_binary_stream*, BYTE);								// 0x0C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x2C];																			// 0x10
		DWORD machine_endian;																				// 0x3C
		DWORD endian;																						// 0x40
	} binary_stream_t, *p_binary_stream;

#pragma endregion

#pragma region CameraVehicle

	// Size = 0x1EC
	// From N3.dll
	typedef struct ao_camera_vehicle  // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
	{
		PVOID p_v_table;									// 0x0000
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x20];							// 0x0004
		PVOID p_locality_listener_i;						// 0x0024
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x28[0x4];								// 0x0028
		struct ao_dummy_vehicle* p_parent_vehicle;				// 0x002C
		PVOID p_vehicle_body_i;								// 0x0030
		float mass;											// 0x0034
		float max_force;									// 0x0038
		float max_vel1;										// 0x003C
		float brake_distance;								// 0x0040
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];								// 0x0044
		float max_vel2;										// 0x0048
		float radius;										// 0x004C
		BYTE falling_enabled;								// 0x0050
		BYTE surface_hug_enabled;							// 0x0051
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x52[0x6];								// 0x0052
		vector3_t global_pos;								// 0x0058
		vector3_t velocity;									// 0x0064
		quaternion_t direction;								// 0x0070
		quaternion_t body_rot;								// 0x0080
		float dir;											// 0x0090
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x94[0x1C];							// 0x0094
		DWORD orientation_mode;								// 0x00B0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xB4[0x18];							// 0x00B4
		float velocity_magnitude;							// 0x00CC
		vector3_t last_pos;									// 0x00D0
		vector3_t parent_global_pos;						// 0x00DC
		quaternion_t parent_body_rot;						// 0x00E8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xF8[0x5C];							// 0x00F8	
		struct ao_n3_tile_map_surface* p_n3_tile_map_surface;	// 0x0154
		DWORD playfield_instance_id;						// 0x0158
		BYTE surface_collision_enabled;						// 0x015C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x15D[0x3];							// 0x015D
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x160[0x1C];							// 0x0160
		struct ao_char_vehicle* p_secondary_target_vehicle;	// 0x017C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x180[0x6C];							// 0x180
	} camera_vehicle_t, *p_camera_vehicle_t;

#pragma endregion

#pragma region CastNanoSpellIIR

	// Size = 0x30
	// From Gamecode.dll
	typedef struct ao_cast_nanospell_iir  // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
	{
		PVOID p_v_table;
		identity_t identity;		
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0xC;
		BYTE unknown_0xD[0x3];
		DWORD unknown_0x10;
		DWORD unknown_0x14;
		DWORD unknown_0x18;
		DWORD unknown_0x1C;
		DWORD unknown_0x20;
		DWORD unknown_0x24;
		DWORD unknown_0x28;
		DWORD unknown_0x2C;
		// ReSharper restore CppInconsistentNaming
	} cast_nanospell_iir_t, *p_cast_nanospell_iir_t;

#pragma endregion

#pragma region CharMovementStatus

	// Size = 0x34
	// From Gamecode.dll
	typedef struct ao_char_movement_status
	{
		PVOID p_v_table;				// 0x00
		DWORD movement_mode;			// 0x04
		DWORD is_moving_forward_back;	// 0x08 If != 1
		DWORD forward_back_direction;	// 0x0C
		DWORD is_moving_strafe;			// 0x10 If != 1
		DWORD strafe_direction;			// 0x14
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0x8];			// 0x18
		DWORD is_rotating;				// 0x20 If != 1
		DWORD rotation_direction;		// 0x24
		DWORD is_jumping;				// 0x28 If = 3
		PVOID p_vehicle;				// 0x2C
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x30;				// 0x30
	} char_movement_status_t, *p_char_movement_status_t;

#pragma endregion

#pragma region CharVehicle	

	// Size = 0x3AC
	// From Gamecode.dll
	typedef struct ao_char_vehicle  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;									// 0x0000
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x10];							// 0x0004
		DWORD zone_instance_id;								// 0x0014
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0xC];								// 0x0018
		PVOID p_locality_listener_i;						// 0x0024
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x28[0x4];								// 0x0028
		struct ao_dummy_vehicle* p_parent_vehicle;				// 0x002C
		PVOID p_vehicle_body_i;								// 0x0030
		float mass;											// 0x0034
		float max_force;									// 0x0038
		float max_vel1;										// 0x003C
		float brake_distance;								// 0x0040
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];								// 0x0044
		float max_vel2;										// 0x0048
		float radius;										// 0x004C
		BYTE falling_enabled;								// 0x0050
		BYTE surface_hug_enabled;							// 0x0051
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x52[0x6];								// 0x0052
		vector3_t global_pos;								// 0x0058
		vector3_t velocity;									// 0x0064
		quaternion_t direction;								// 0x0070
		quaternion_t body_rot;								// 0x0080
		float dir;											// 0x0090
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x94[0x1C];							// 0x0094
		DWORD orientation_mode;								// 0x00B0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xB4[0xC];								// 0x00B4
		vector3_t body_forward;								// 0x00C0
		float velocity_magnitude;							// 0x00CC
		vector3_t last_pos;									// 0x00D0
		vector3_t parent_global_pos;						// 0x00DC
		quaternion_t parent_body_rot;						// 0x00E8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xF8[0x5C];							// 0x00F8	
		struct ao_n3_tile_map_surface* p_n3_tile_map_surface;	// 0x0154
		DWORD playfield_instance_id;						// 0x0158
		BYTE surface_collision_enabled;						// 0x015C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x15D[0x3];							// 0x015D
		PVOID pn_3dynel_event_listener_i;					// 0x0160
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x164[0x4];							// 0x0164
		identity_t identity;								// 0x0168
		float max_speed;									// 0x0170
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x174[0x4];							// 0x0174
		p_char_movement_status_t p_char_movement_status;	// 0x0178
		struct ao_char_vehicle* p_this_vehicle;				// 0x017C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x180[0x22C];							// 0x0180
	} char_vehicle_t, *p_char_vehicle_t;

#pragma endregion

#pragma region ChatGUIModule

	// Size = 0x2C0
	// From GUI.dll
	typedef struct ao_chat_gui_module
	{
		PVOID p_v_table;					// 0x00
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x4[0x4C];				// 0x04		
		PVOID p_v_table_signal_target_c;	// 0x50
		BYTE unknown_0x54[0x4];				// 0x54
		PVOID p_v_table_base_target_c;		// 0x58
		BYTE unknown_0x5C[0x84];			// 0x5C
		PVOID p_chat_sound_effect;			// 0xE0
		PVOID p_friend_list_view;			// 0xE4
		BYTE unknown_0xE8[0x14];			// 0xE8
		PVOID p_window;						// 0xFC
		BYTE unknown_0x100[0x18];			// 0x100
		PVOID p_friends_window;				// 0x118
		BYTE unknown_0x11C[0xB4];			// 0x11C
		PVOID p_chat_group_window;			// 0x1D0
		BYTE unknown_0x1D4[0xC0];			// 0x1D4
		string afk_message;					// 0x294
		BYTE Unknown0x2AC[0x4];				// 0x2AC
		DWORD64 AFKTimer;					// 0x2B0
		PVOID p_cc_generic;					// 0x2B8
		BYTE unknown_0x2BC[0x4];			// 0x2BC
		// ReSharper restore CppInconsistentNaming
	} chat_gui_module_t, *p_chat_gui_module_t;

#pragma endregion

#pragma region ChatWindowNode

	// Size = 0x218
	// From GUI.dll
	typedef struct ao_chat_window_node
	{
		PVOID p_v_table;				// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x4];			// 0x04
		PVOID p_v_table_base_object_c;	// 0x08
		string window_name;				// 0x0C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x24[0x1F4];		// 0x24
	} chat_window_node_t, *p_chat_window_node_t;

	typedef struct ao_chat_window_node_node
	{
		struct ao_chat_window_node_node* p_lower;		// 0x00
		struct ao_chat_window_node_node* p_back;		// 0x04
		struct ao_chat_window_node_node* p_higher;		// 0x08
		string window_name;							// 0x0C
		struct ao_chat_window_node* p_chat_window;		// 0x24
	} chat_window_node_node_t, *p_chat_window_node_node_t;

	typedef struct ao_chat_window_node_root
	{
		struct ao_chat_window_node_node* p_begin;
		struct ao_chat_window_node_node* p_node;
		struct ao_chat_window_node_node* pEnd;
	} chat_window_node_root_t, *p_chat_window_node_root_t;

	typedef struct ao_chat_window_node_dir
	{
		BYTE unknown_0x00[0x4];
		p_chat_window_node_root_t p_root;
		DWORD count;
	} chat_window_node_dir_t, *p_chat_window_node_dir_t;

#pragma endregion

#pragma region ChatWindowController
	
	// Size = 0x80
	// From GUI.dll
	typedef struct ao_chat_window_controller
	{
		PVOID p_v_table;										// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x10];								// 0x04
		chat_window_node_map_t chat_window_node_dir;			// 0x14
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x20[0x1C];								// 0x20
	} chat_window_controller_t, *p_chat_window_controller;

#pragma endregion

#pragma region CommandInterpreter

	// Size = 0x80
	// From GUI.dll
	typedef struct ao_command_interpreter
	{
		PVOID p_v_table;				// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x7C];		// 0x04
	} command_interpreter_t, *p_command_interpreter_t;
#pragma endregion

#pragma region CollInfo

	// Size = 0x14
	// From 
	typedef struct ao_coll_info
	{
		identity_t identity;	// 0x00
		vector3_t location;		// 0x08
	} coll_info_t, *p_coll_info_t;

#pragma endregion

#pragma region Chest

	// Size = 0x1E8
	// From Gamecode.dll
	typedef struct ao_chest  // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
	{
		PVOID p_v_table;									// 0x0000
		PVOID p_v_table_n3_dynel_event_source;				// 0x0004
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x08[0x4];								// 0x0008
		PVOID p_v_table_db_object;							// 0x000C
		struct ao_event_caster* p_event_caster;				// 0x0010
		identity_t identity;								// 0x0014
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C[0x8];								// 0x001C
		std::vector<coll_info_t>* p_coll_info_vector;		// 0x0024
		PVOID p_v_table_n3_fsm;								// 0x0028
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x2C[0x10];							// 0x002C
		BYTE is_dead;										// 0x003C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x3D[0x3];								// 0x003D
		struct ao_n3_sf_base *p_first_state_function;			// 0x0040
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];								// 0x0044
		BYTE is_not_snoozing;								// 0x0048
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x49[0x7];								// 0x0049
		struct ao_dummy_vehicle* p_vehicle;					// 0x0050
		BYTE is_ground_collision_enabled;					// 0x0054
		BYTE is_coll_enabled;								// 0x0055
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x56[0x2];								// 0x0056
		BYTE is_stationary;									// 0x0058
		BYTE is_repulsive;									// 0x0059
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x5A[0x2];								// 0x005A
		struct n3_dyna_mesh_coll_sphere* p_coll_prim;		// 0x005C
		DWORD playfield_instance_id;						// 0x0060
		struct ao_n3_tile_map* p_n3_tile_map;					// 0x0064
		BYTE is_on_border;									// 0x0068
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x69[0x2];								// 0x0069
		// ReSharper disable once CppInconsistentNaming
		BYTE IsInTree;										// 0x006B	
		PVOID p_n3_info_item_remote;						// 0x006C
		std::vector<struct ao_n3_dynel*>* p_child_dynel_vector;// 0x0070
		identity_t parent_identity;							// 0x0074
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x7C[0x4];								// 0x007C
		BYTE is_dying;										// 0x0080
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x81[0xB];								// 0x0081
		DWORD rdb_dynel_status;								// 0x008C
		DWORD last_allowed_zone_inst;						// 0x0090
		vector3_t last_allowed_global_position_in_zone;		// 0x0094
		vector3_t reconcile_position;						// 0x00A0
		float body_scale;									// 0x00AC
		PVOID p_texture_data_list;							// 0x00B0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xB4[0xC];								// 0x00B4
		PVOID p_visual_mesh_t;								// 0x00C0
		PVOID p_visual_cat_mesh_t;							// 0x00C4
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC8[0x4];								// 0x00C8
		BYTE is_client_char;								// 0x00CC
		BYTE is_visible;									// 0x00CD
		BYTE use_char_dist_cap;								// 0x00CE
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xCF;									// 0x00CF
		vector3_t bounding_sphere_pos;						// 0x00D0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xDC[0x4C];							// 0x00DC
		struct ao_new_inventory* p_inventory;					// 0x0128
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x12C[0x1C];							// 0x012C
		PVOID p_v_table_signal_holder;						// 0x0148
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x14C[0x30];							// 0x014C
		PVOID p_v_table_stat_holder;						// 0x017C
		std::vector<LONG>* pStatVector;						// 0x0180
		identity_t unknown_identity;						// 0x0184
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18C[0x2C];							// 0x018C
		// ReSharper disable once CppInconsistentNaming
		identity_t TemplateIdentity;						// 0x01B8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C0[0x28];							// 0x01C0
	} chest_t, *p_chest_t;

#pragma endregion

#pragma region DatabaseController

	// Size = 0x08
	// From DatabaseController.dll
	typedef struct ao_database_controller
	{
		PVOID p_v_table;		// 0x00
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x08;		// 0x08		
		BYTE unknown_0x09[0x3];	// 0x09
		// ReSharper restore CppInconsistentNaming
	} database_controller_t, *p_database_controller_t;

#pragma endregion

#pragma region DbObject
	
	// Size = 0x18
	// From DatabaseController.dll
	typedef struct ao_db_object  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;						// 0x00
		struct ao_event_caster* p_event_caster;	// 0x04
		identity_t identity;					// 0x08
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x8];					// 0x10
	} db_object_t, *p_db_object_t;

#pragma endregion

#pragma region GameData::DistrictData

	// Size = 0x6C
	// From GameData.dll
	typedef struct ao_district_data  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;				// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x4[0x8];			// 0x04
		vector3_t center;				// 0x0C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0x24];		// 0x18
		DWORD npc_min_level;			// 0x3C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x40[0x4];			// 0x40
		DWORD land_control_min_level;	// 0x44
		DWORD land_control_max_level;	// 0x48
		DWORD respawn_chance;			// 0x4C
		float respawn_time;				// 0x50
		DWORD fight_mode;				// 0x54
		PVOID p_spawn_point_vec;		// 0x58
		PVOID p_hash_spawn_point_vec;	// 0x5C
		PVOID p_spawn_info_vec;			// 0x60
		PVOID p_wildlife_entries;		// 0x64
		DWORD data_format_version;		// 0x68
	} district_data_t, *p_district_data_t;

#pragma endregion

#pragma region DummyItemBase

	// Size = 0xA4
	// From Gamecode.dll
	typedef struct ao_dummy_item_base  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;					// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x34];			// 0x04
		std::vector<LONG>* p_stats_vector;	// 0x38
		identity_t parent_identity;			// 0x3C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x2C];			// 0x44
		identity_t identity;				// 0x70
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x78[0x24];			// 0x78
		char const * name;					// 0x9C
		char const * description;			// 0xA0
	} dummy_item_base_t, *p_dummy_item_base_t;

#pragma endregion

#pragma region DummyVehicle

	// Size = 0x160
	// From N3.dll
	typedef struct ao_dummy_vehicle  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;									// 0x0000
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x10];							// 0x0004
		DWORD zone_instance_id;								// 0x0014
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0xC];								// 0x0018
		PVOID p_locality_listener_i;						// 0x0024
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x28[0x4];								// 0x0028
		struct ao_dummy_vehicle* p_parent_vehicle;				// 0x002C
		PVOID p_vehicle_body_i;								// 0x0030
		float mass;											// 0x0034
		float max_force;									// 0x0038
		float max_vel1;										// 0x003C
		float brake_distance;								// 0x0040
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];								// 0x0044
		float max_vel2;										// 0x0048
		float radius;										// 0x004C
		BYTE falling_enabled;								// 0x0050
		BYTE surface_hug_enabled;							// 0x0051
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x52[0x6];								// 0x0052
		vector3_t global_pos;								// 0x0058
		vector3_t velocity;									// 0x0064
		quaternion_t direction;								// 0x0070
		quaternion_t body_rot;								// 0x0080
		float dir;											// 0x0090
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x94[0x1C];							// 0x0094
		DWORD orientation_mode;								// 0x00B0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xB4[0xC];								// 0x00B4
		vector3_t body_forward;								// 0x00C0
		float velocity_magnitude;							// 0x00CC
		vector3_t last_pos;									// 0x00D0
		vector3_t parent_global_pos;						// 0x00DC
		quaternion_t parent_body_rot;						// 0x00E8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xF8[0x5C];							// 0x00F8	
		struct ao_n3_tile_map_surface* p_n3_tile_map_surface;	// 0x0154
		DWORD playfield_instance_id;						// 0x0158
		BYTE surface_collision_enabled;						// 0x015C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x15D[0x3];							// 0x015D
	} dummy_vehicle_t, *p_dummy_vehicle_t;

#pragma endregion

#pragma region DummyWeapon

	// Size = 0xE8
	// From Gamecode.dll
	typedef struct ao_dummy_weapon  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;					// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x34];			// 0x04
		std::vector<LONG>* p_stats_vector;	// 0x38
		identity_t unknown_identity;		// 0x3C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x2C];			// 0x44
		identity_t identity;				// 0x70
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x78[0x24];			// 0x78
		char const * name;					// 0x9C
		char const * description;			// 0xA0
		identity_t weapon_identity;			// 0xA4
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xAC[0x3C];			// 0xAC
	} dummy_weapon_t, *p_dummy_weapon_t;

#pragma endregion

#pragma region EventCaster

	// Size = 0x18
	// From N3.dll
	typedef struct ao_event_caster
	{
		PVOID p_v_table;										// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x4[0x10];									// 0x04
		VOID(__thiscall *p_n3_dynel_t__release_address)(PVOID);	// 0x14
	} event_caster_t, *p_event_caster_t;

#pragma endregion

#pragma region FlowControl

	// Size = 0x58
	// From GUI.dll
	typedef struct ao_flow_control_t
	{
		PVOID p_v_table;			// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x54];	// 0x04
	} flow_control_t, *p_flow_control_t;

#pragma endregion

#pragma region Gametime

	// Size = 0xC8
	// From Gamecode.dll
	typedef struct ao_game_time
	{
		PVOID p_v_table;				// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x14];		// 0x04
		WORD second_ping;				// 0x18
		BYTE odd_second_ping;			// 0x1A
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1B[0xD];			// 0x1B
		DOUBLE normal_time;				// 0x28
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x30[0x8];			// 0x30
		DOUBLE second_ping_delta_time;	// 0x38
		DWORD current_second;			// 0x40
		DWORD current_minute;			// 0x44	
		DWORD current_hour;				// 0x48
		DWORD current_day;				// 0x4C
		DOUBLE current_real_time;		// 0x50
		DWORD day_period;				// 0x58
		DWORD game_seconds_per_second;	// 0x5C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x60[0x18];		// 0x60
		DOUBLE delta_time;				// 0x78
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x80[0x4];			// 0x80
		DWORD minutes_per_hour;			// 0x84
		DWORD seconds_per_minute;		// 0x88
		DWORD seconds_in_a_hour;		// 0x8C
		DWORD hour_of_dusk_start;		// 0x90
		DWORD dusk_duration_in_hours;	// 0x94
		DWORD hour_of_dawn_start;		// 0x98
		DWORD dawn_duration_in_hours;	// 0x9C
		float seconds_in_a_day;			// 0xA0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xA4[0x10];		// 0xA4
		DWORD system_time;				// 0xB4
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xB8[0x8];			// 0xB8
		DWORD system_time_counted;		// 0xC0
		float system_seconds_counted;	// 0xC4
	} game_time_t, *p_game_time_t;

#pragma endregion

#pragma region GraphPathFinder

	// Size = 0x24
	// From PathFinder.dll
	typedef struct ao_graph_path_finder
	{
		PVOID p_v_table;			// 0x00
		PVOID p_astar_graph_t;		// 0x04
		PVOID p_visibility_graph_t;	// 0x08
		PVOID p_space_i;			// 0x0C
		PVOID p_surface_i;			// 0x10
		DWORD grid_size_x;			// 0x14
		DWORD grid_size_z;			// 0x18
		float total_size_x;			// 0x1C
		float total_size_z;			// 0x20
	} graph_path_finder_t, *p_graph_path_finder_t;

#pragma endregion

#pragma region GridSpace

	// Size = 0x3C
	// From Vehicle.dll
	typedef struct ao_grid_space
	{
		PVOID p_v_table;						// 0x00
		PVOID p_v_table_change_listener_i;		// 0x04
		PVOID p_v_table_detection_listener_i;	// 0x08
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC[0x14];					// 0x0C
		float total_size_x;						// 0x20
		float total_size_z;						// 0x24
		DWORD grid_size_x;						// 0x28
		DWORD grid_size_z;						// 0x2C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x30[0xC];					// 0x30
	} grid_space_t, *p_grid_space_t;

#pragma endregion

#pragma region GameData::HashSpawnPoint

	//From GameData.dll
	// Size = 0x6C
	typedef struct ao_hash_spawn_point  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;					// 0x00
		vector3_t center_pos;				// 0x04
		float radius;						// 0x10
		float rotation_mid;					// 0x14
		float rotation_width;				// 0x18
		PVOID p_v_table_reference_counted;	// 0x1C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x20[0x4];				// 0x20
		DWORD hash;							// 0x24
		DWORD min_level;					// 0x28
		DWORD max_level;					// 0x2C
		DWORD respawn_chance;				// 0x30
		float respawn_time;					// 0x34
		DWORD flags;						// 0x38
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x3C[0x4];				// 0x3C
		DWORD proximity_range;				// 0x40
		DWORD additional_points;			// 0x44
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x48[0xC];				// 0x48
		PVOID p_spells;						// 0x54
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x58[0xC];				// 0x58
		DWORD more_flags;					// 0x64
		DWORD tags;							// 0x68
	} hash_spawn_point_t, *p_hash_spawn_point_t;

#pragma endregion

#pragma region HotSpot

	// Size = 0x54
	// From GUI.dll
	typedef struct ao_hot_spot
	{
		PVOID p_v_table;				// 0x00
		DWORD sort_level;			// 0x04
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x08[0x4];		// 0x08
		DWORD tooltip_status;		// 0x0C
		PVOID tooltip_callback;		// 0x10
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x14[0x4];		// 0x14
		PCSTR p_help_text;		// 0x18
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C[0x8];		// 0x1C
		BYTE is_dragable;			// 0x24
		BYTE is_instant_drag;			// 0x25
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x26[0x2];		// 0x26
		PVOID p_render_window;		// 0x28
		DWORD hot_rect_corner1;		// 0x2C
		DWORD hot_rect_corner2;		// 0x30
		DWORD hot_rect_corner3;		// 0x34
		DWORD hot_rect_corner4;		// 0x38
		i_point_t org_pos;		// 0x3C
		i_point_t unknown_0_x44;	// 0x44
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x4C[0x4];		// 0x4C
		BYTE is_inserted;			// 0x50
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x51[0x3];		// 0x51
	} hot_spot_t, *p_hot_spot_t;

#pragma endregion

#pragma region HTMLParser

	// Size = 0x50
	// From GUI.dll
	typedef struct ao_html_parser
	{
		PVOID p_v_table;			// 0x00
		string html_text;			// 0x04;
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x1C[0x4];		// 0x1C		
		BYTE unknown_0x20[0x20];	// 0x20
		// ReSharper restore CppInconsistentNaming
		DWORD font_id;				// 0x40
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x8];		// 0x44
		DWORD feature_flags;			// 0x4C
	} html_parser_t, *p_html_parser_t;

#pragma endregion

#pragma region Indicator

	// Size = 0x3C
	// From GUI.dll
	typedef struct ao_indicator  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;			// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x4];		// 0x04
		identity_t identity;		// 0x08
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x2C];	// 0x10
	} indicator_t, *p_indicator_t;

#pragma endregion

#pragma region Hotkey

	typedef struct ao_hotkey
	{
		DWORD unknown_0x00;
		DWORD unknown_0x04;
		DWORD unknown_0x08;
		DWORD unknown_0x0C;
		DWORD unknown_0x10;
		DWORD unknown_0x14;
		DWORD unknown_0x18;
		DWORD unknown_0x1C;
		DWORD unknown_0x20;
		DWORD unknown_0x24;
	} hotkey_t, *p_hotkey_t;

#pragma endregion

#pragma region InputConfig

	// Size = 0x1D0
	// From GUI.dll
	typedef struct ao_input_config  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;									// 0x0000
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x18];							// 0x0004
		PVOID p_hotkey_list;								// 0x001C
		DWORD tooltip_time;									// 0x0020
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x24[0x80];							// 0x0024
		vector3_t mouse_world_position;						// 0x00A4
		identity_t current_id;								// 0x00B0
		identity_t current_reference;						// 0x00B8
		identity_t current_target;							// 0x00C0
		DWORD current_void_reference;						// 0x00C8
		p_hot_spot_t p_current_hotspot;						// 0x00CC
		p_hot_spot_t p_last_mouse_button_pressed_hotspot;	// 0x00D0
		DWORD character_id;									// 0x00D4
		identity_t character_identity;						// 0x00D8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xE0[0x10];							// 0x00E0
		DWORD last_hotkey_pressed;							// 0x00F0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xF4[0x24];							// 0x00F4
		i_point_t current_mouse_position;					// 0x0118
		// ReSharper disable once CppInconsistentNaming
		short last_key;										// 0x0120
		short key_modifier_flags;							// 0x0122
		unsigned char key_status_array[0x7E];				// 0x0124
		unsigned char unknown_0x1A3[0x2D];					// 0x01A3
															// 0x01D0
	} input_config_t, *p_input_config_t;

#pragma endregion

#pragma region Inventory

	// Size = 0x2C
	// From Gamecode.dll
	typedef struct ao_inventory_data  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x00[0x2];			// 0x00
		WORD count;						// 0x02
		identity_t dynel_identity;		// 0x04	
		rdb_identity_t rdb_identity;	// 0x0C
		DWORD quality_level;			// 0x14
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0x10];		// 0x1C
	} inventory_data_t, *p_inventory_data_t;

	// Size = 0x2C
	// From Gamecode.dll
	typedef struct ao_new_inventory
	{
		PVOID p_v_table;									// 0x00
		std::vector <p_inventory_data_t> p_inventory_data;	// 0x04
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x4];								// 0x10
		DWORD count;										// 0x14
		identity_t inventory_identity;						// 0x18
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x20[0xC];								// 0x20
	} new_inventory_t, *p_new_inventory_t;

	// Size = 0x1B8
	// From Gamecode.dll
	typedef struct ao_inventory_holder
	{
		struct ao_simple_char* p_client_control_dynel;	// 0x0000
		identity_t* p_client_identity;				// 0x0004
		p_new_inventory_t p_regular_inventory;		// 0x0008
		p_new_inventory_t p_unknown_inventory;		// 0x000C
		p_new_inventory_t p_overflow_inventory;		// 0x0010
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x14[0x168];					// 0x0014
		p_bank_entry_t p_bank_entry;				// 0x017C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x180[0x38];					// 0x0180
	} inventory_holder_t, *p_inventory_holder_t;

	typedef struct ao_inventory_entry  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		identity_t template_identity;	// 0x00
		DWORD unknown1;					// 0x04
		DWORD unknown2;					// 0x08
		WORD unknown3;					// 0x0C
		WORD unknown4;					// 0x0E
	} inventory_entry_t, *p_inventory_entry_t;

#pragma endregion

#pragma region LockIdMap

	// Size = 0x18
	typedef struct lock_id_node
	{
		struct lock_id_node* p_lower;	// 0x00
		struct lock_id_node* p_back;	// 0x04
		struct lock_id_node* p_higher;	// 0x08
		DWORD lock_id;					// 0x0C
		DWORD action_id;				// 0x14
	} lock_id_node_t, *p_lock_id_node_t;

	typedef struct lock_id_root
	{
		p_lock_id_node_t p_begin;		// 0x00
		p_lock_id_node_t p_node;		// 0x04
		p_lock_id_node_t p_end;			// 0x08
	} lock_id_root_t, *p_lock_id_root_t;

	typedef struct lock_id_dir
	{
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x0[0x4];		// 0x00
		p_lock_id_root_t p_root;	// 0x04
		DWORD count;				// 0x08
	} lock_id_dir_t, *p_lock_id_dir_t;

#pragma endregion

#pragma region LookAtIIR

	// Size = 0x24
	// From Gamecode.dll
	typedef struct ao_look_at_iir  // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
	{
		PVOID p_v_table;				// 0x00
		identity_t client_id;	// 0x04
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0xC;			// 0x0C		
		BYTE unknown_0xD[0x3];		// 0x0D
		// ReSharper restore CppInconsistentNaming
		DWORD map_to_key;				// 0x10
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x14;			// 0x14
		identity_t target_id;	// 0x18
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x20;			// 0x20
	} look_at_iir_t, *p_look_at_iir_t;

#pragma endregion

#pragma region LoginModule_c

	// Size = 0xB8
	// From GUI.dll
	typedef struct ao_login_module
	{
		PVOID p_v_table;			// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0xC];		// 0x04
		PVOID p_login_window;		// 0x10
		PVOID p_char_select_window;	// 0x14
		PVOID p_limbo_window;		// 0x18
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C[0x78];	// 0x1C
		PVOID p_login_world;		// 0x94
	} login_module_t, *p_login_module_t;

#pragma endregion

#pragma region StatHolder

	typedef struct ao_stat_holder
	{
		std::vector<action_lock_t>* p_skill_locks;	// 0x00
		p_stat_map_t p_stat_map_dir;			// 0x04
	} stat_holder_t, *p_stat_holder_t;

#pragma endregion

#pragma region Message

	typedef struct ao_message
	{
		PVOID p_v_table;			// 0x00
		WORD message_id;			// 0x04
		// ReSharper disable once CppInconsistentNaming
		WORD unknown_0x06;			// 0x06
		DWORD message_type;			// 0x08
		DWORD source_id;			// 0x0C
		DWORD dest_id;				// 0x10
		DWORD priority;				// 0x14
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x18;			// 0x18
	} message_t, *p_message_t;

#pragma endregion

#pragma region n3Camera

	// Size - 0x258
	// From N3.dll
	typedef struct ao_n3_camera  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;								// 0x0000
		PVOID p_v_table_n3_dynel_event_source;			// 0x0004
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x08[0x4];							// 0x0008
		PVOID p_v_table_db_object;						// 0x000C
		p_event_caster_t p_event_caster;				// 0x0010
		identity_t identity;							// 0x0014
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x1C[0x8];							// 0x001C		
		BYTE unknown_0x24[0x4];							// 0x0024
		// ReSharper restore CppInconsistentNaming
		PVOID p_v_table_n3_fsm;							// 0x0028
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x10];						// 0x002C
		BYTE is_dead;									// 0x003C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x15[0x3];							// 0x003D
		struct ao_n3_sf_base *p_first_state_function;		// 0x0040
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0xC];							// 0x0044
		p_camera_vehicle_t p_camera_vehicle;			// 0x0050
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x54[0x190];						// 0x0054
		identity_t selected_target;						// 0x01E4
		DWORD camera_type;								// 0x01EC 0 - 1st Person, 1 = 3rd Person Trail, 2 = 3rd Person Rubber, 3 = 3rd Person Lock
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1F0[0x10];						// 0x01F0
		PVOID p_visual_camera;							// 0x0200
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x204[0x54];						// 0x0204
	} n3_camera_t, *p_n3_camera_t;

#pragma endregion

#pragma region n3CellMonitor

	// Size = 0x38
	// From N3.dll
	typedef struct ao_n3_cell_monitor  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;				// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x20];		// 0x04
		PVOID p_locality_listener_i;	// 0x24
		vector3_t pos;					// 0x28
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x34[0x4];			// 0x34
	} n3_cell_monitor_t, *p_n3_cell_monitor_t;

#pragma endregion

#pragma region n3DatabaseHandler

	// Size = 0xC
	// From N3.dll
	typedef struct ao_n3_database_handler
	{
		PVOID p_v_table;								// 0x00
		struct ao_resource_database* p_resource_database;	// 0x04
		struct ao_resource_database* p_game_database;		// 0x08
	} n3_database_handler_t, *p_n3_database_handler_t;

#pragma endregion

#pragma region N3DynaMeshCollsphere

	// Size = 0x18
	// From Collision.dll
	typedef struct ao_coll_prim  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;				// 0x00
		vector3_t global_pos;			// 0x04
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x4];			// 0x10
		DWORD coll_prim_type;			// 0x14 Sphere = 2
	} coll_prim_t, *p_coll_prim;

	// Size = 0x1C
	// From Collision.dll
	typedef struct ao_coll_sphere  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		struct ao_coll_prim coll_prim;		// 0x00
		float possible_radius;			// 0x18
	} coll_sphere_t, *p_coll_sphere_t;

	// Size = 0x1C
	// From Collision.dll
	typedef struct ao_mesh_coll_sphere
	{
		struct ao_coll_sphere coll_sphere;				// 0x00
	} mesh_coll_sphere_t, p_mesh_coll_sphere_t;

	// Size = 0x30
	// From N3.dll
	typedef struct n3_dyna_mesh_coll_sphere  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		struct ao_mesh_coll_sphere mesh_coll_sphere;	// 0x00
		PVOID p_locality_listener_i;				// 0x1C
		struct ao_n3_dynel* p_owner;					// 0x20
		vector3_t original_relative_position;		// 0x24
	} n3_dyna_mesh_coll_sphere_t, *p_n3_dyna_mesh_coll_sphere_t;

#pragma endregion

#pragma region n3Dynel

	// Size = 0xA0
	// From N3.dll
	typedef struct ao_n3_dynel  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;									// 0x00
		PVOID p_v_table_n_3dynel_event_source;				// 0x04
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x08[0x4];								// 0x08
		PVOID p_v_table_db_object;							// 0x0C
		p_event_caster_t p_event_caster;					// 0x10
		identity_t identity;								// 0x14
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C[0x8];								// 0x1C
		std::vector<coll_info_t>* p_coll_info_vector;		// 0x24
		PVOID p_v_table_n3_fsm;								// 0x28
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x2C[0x10];							// 0x2C
		BYTE is_dead;										// 0x3C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x3D[0x3];								// 0x3D
		struct ao_n3_sf_base *p_first_state_function;			// 0x40
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];								// 0x44
		BYTE is_not_snoozing;								// 0x48
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x49[0x7];								// 0x49
		p_dummy_vehicle_t p_vehicle;						// 0x50
		BYTE is_ground_collision_enabled;					// 0x54
		BYTE is_coll_enabled;								// 0x55
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x56[0x2];								// 0x56
		BYTE is_stationary;									// 0x58
		BYTE is_repulsive;									// 0x59
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x5A[0x2];								// 0x5A
		p_n3_dyna_mesh_coll_sphere_t p_coll_prim;			// 0x5C
		DWORD PlayfieldInstanceId;							// 0x60
		struct ao_n3_tile_map* p_n3_tile_map;					// 0x64
		BYTE is_on_border;									// 0x68
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x69[0x2];								// 0x69
		BYTE is_in_tree;									// 0x6B	
		PVOID p_n3_info_item_remote;						// 0x6C
		std::vector<struct ao_n3_dynel*>* p_child_dynel_vector;// 0x70
		identity_t parent_identity;							// 0x74
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x7C[0x4];								// 0x7C
		BYTE is_dying;										// 0x80
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown0x81[0xB];								// 0x81
		DWORD rdb_dynel_status;								// 0x8C
		DWORD last_allowed_zone_inst;						// 0x90
		vector3_t last_allowed_global_position_in_zone;		// 0x94
	} n3_dynel_t, *p_n3_dynel_t;

#pragma endregion

#pragma region n3DynelEventSource

	// Size = 0x08
	// From N3.dll
	typedef struct ao_n3_dynel_event_source
	{
		PVOID p_v_table;			// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x4];		// 0x04
	} n3_dynel_event_source_t, *p_n3_dynel_event_source_t;

#pragma endregion

#pragma region n3EngineClientAnarchy

	// Size = 0x130
	// From Gamecode.dll
	typedef struct ao_n3_engine_client_anarchy  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;										// 0x0000
		binary_stream_t binary_stream;							// 0x0004
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x48[0x10];								// 0x0048
		BYTE teleport_status;									// 0x0058
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x59[0x7];									// 0x0059					
		struct ao_resource_database* p_resource_database;			// 0x0060
		struct ao_n3_root* p_n3_root;								// 0x0064
		float delta_time;										// 0x0068
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x6C[0x4];									// 0x006C
		float total_time;										// 0x0070
		struct ao_n3_object_factory* p_n3_object_factory;			// 0x0074
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x78[0x4];									// 0x0078
		p_n3_camera_t p_n3_camera;								// 0x007C
		DWORD client_inst_id;									// 0x0080
		struct ao_simple_char* p_client_control_char;				// 0x0084
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x88[0x8];									// 0x0088
		struct ao_n3_engine_client_anarchy** pp_client_interface;	// 0x0090
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x94[0xC];									// 0x0094
		PVOID p_grid_destination_list;							// 0x00A0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xA4[0x24];								// 0x00A4
		identity_t mouseover_target;							// 0x00C8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xD0[0x8];									// 0x00D0
		DOUBLE time_of_last_motion;								// 0x00D8
		quaternion_t last_rot;									// 0x00E0
		vector3_t last_move;									// 0x00F0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xFC[0x4];									// 0x00FC
		PVOID p_anim_calibration_control;						// 0x0100
		BYTE is_first_login;									// 0x0104
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x105[0x2B];								// 0x0105
	} n3_engine_client_anarchy_t, *p_n3_engine_client_anarchy_t;

#pragma endregion

#pragma region n3Fobj

	// Size = 0x18
	// From N3.dll
	typedef struct ao_n3_f_obj
	{
		PVOID p_v_table;			// 0x00
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x04[0x10];	// 0x04		
		BYTE unknown_0x14;			// 0x14
		BYTE unknown_0x15[0x3];		// 0x15
		// ReSharper restore CppInconsistentNaming
	} n3_f_obj_t, *p_n3_f_obj_t;

#pragma endregion
	
#pragma region n3Fsm

	// Size = 1C
	// From N3.dll
	typedef struct ao_n3_fsm
	{
		PVOID p_v_table;										// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x10];								// 0x04
		BYTE is_dead;											// 0x14
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x15[0x3];									// 0x15
		struct ao_n3_sf_base *p_first_state_function;				// 0x18
	} n3_fsm_t, *p_n3_fsm_t;	

#pragma endregion

#pragma region N3Message

	typedef struct ao_n3_message
	{
		PVOID p_v_table;			// 0x00
		WORD message_id;			// 0x04
		// ReSharper disable once CppInconsistentNaming
		WORD unknown_0x06;			// 0x06
		DWORD message_type;			// 0x08
		DWORD source_id;			// 0x0C
		DWORD dest_id;				// 0x10
		DWORD priority;				// 0x14
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x18;			// 0x18
		PVOID p_ace_data_block;		// 0x1C
		DWORD message_body_len;		// 0x20
	} n3_message_t, *p_n3_message_t;

#pragma endregion

#pragma region n3ObjectFactory

	// Size = 0x8
	// From N3.dll
	typedef struct ao_n3_object_factory
	{
		struct ao_resource_database* p_resource_database;	// 0x00
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x04;								// 0x04
	} n3_object_factory_t, *p_n3_object_factory_t;

#pragma endregion

#pragma region n3Root

	// Size = 0x3C
	// From N3.dll
	typedef struct ao_n3_root
	{
		PVOID p_v_table;			// 0x00
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x04[0x1C];	// 0x04		
		BYTE unknown_0x20;			// 0x20
		BYTE unknown_0x21[0x3];		// 0x21
		// ReSharper restore CppInconsistentNaming
		PVOID p_v_table_n3_f_obj;	// 0x24
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x28[0x10];	// 0x28
		BYTE unknown_0x38;			// 0x38
		BYTE unknown_0x39[0x3];		// 0x39
		// ReSharper restore CppInconsistentNaming
	} n3_root_t, *p_n3_root_t;

#pragma endregion

#pragma region n3SfBase

	// Size = 0x08
	// From N3.dll
	typedef struct ao_n3_sf_base
	{
		PVOID p_v_table;			// 0x00
		struct ao_n3_fsm* p_n3_fsm;	// 0x04
	} n3_sf_base_t, *p_n3_sf_base_t;

#pragma endregion

#pragma region N3Tilemap	

	// Size = 0x424
	// From N3.dll
	typedef struct ao_n3_tile_map
	{
		PVOID p_v_table;					// 0x0000
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x4];				// 0x0004
		DWORD playfield_instance_id;		// 0x0008
		struct ao_rdb_tile_map* p_rdb_tile_map;// 0x000C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x40C];			// 0x0010
		PVOID p_ground_data;				// 0x41C
		PVOID p_rdb_height_map;				// 0x0420
	} n3_tile_map_t, *p_n3_tile_map_t;

#pragma endregion

#pragma region n3TilemapSurface

	// Size = 0x18
	// From N3.dll
	typedef struct ao_n3_tile_map_surface
	{
		/*0x00*/ void* p_v_table;
		/*0x04*/ p_n3_tile_map_t p_n3_tile_map;
		/*0x08*/ struct ao_playfield_anarchy* p_playfield_anarchy;
		/*0x0C*/ struct ao_cell_surface* p_cell_surface;
		/*0x10*/ float unknown_0x10;
		/*0x14*/ float unknown_0x14;
	} n3_tile_map_surface_t, *p_n3_tile_map_surface_t;

#pragma endregion

#pragma region N3VisualDynel

	// Size = 0xE0
	// From N3.dll
	typedef struct ao_n3_visual_dynel  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;									// 0x00
		PVOID p_v_table_n3_dynel_event_source;				// 0x04
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x08[0x4];								// 0x08
		PVOID p_v_table_db_object;							// 0x0C
		p_event_caster_t p_event_caster;					// 0x10
		identity_t identity;								// 0x14
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C[0x8];								// 0x1C
		std::vector<coll_info_t>* p_coll_info_vector;		// 0x24
		PVOID p_v_table_n3_fsm;								// 0x28
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x2C[0x10];							// 0x2C
		BYTE is_dead;										// 0x3C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x3D[0x3];								// 0x3D
		p_n3_sf_base_t pFirstStateFunction;					// 0x40
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];								// 0x44
		BYTE is_not_snoozing;								// 0x48
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x49[0x7];								// 0x49
		p_dummy_vehicle_t p_vehicle;						// 0x50
		BYTE is_ground_collision_enabled;					// 0x54
		BYTE is_coll_enabled;								// 0x55
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x56[0x2];								// 0x56
		BYTE is_stationary;									// 0x58
		BYTE is_repulsive;									// 0x59
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x5A[0x2];								// 0x5A
		p_n3_dyna_mesh_coll_sphere_t p_coll_prim;			// 0x5C
		DWORD playfield_instance_id;						// 0x60
		p_n3_tile_map_t p_n3_tile_map;						// 0x64
		BYTE IsOnBorder;									// 0x68
		BYTE unknown_0x69[0x2];								// 0x69
		BYTE IsInTree;										// 0x6B	
		PVOID pn3InfoItemRemote;							// 0x6C
		std::vector<p_n3_dynel_t>* p_child_dynel_vector;	// 0x70
		identity_t parent_identity;							// 0x74
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x7C[0x4];								// 0x7C
		BYTE is_dying;										// 0x80
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x81[0xB];								// 0x81
		DWORD rdb_dynel_status;								// 0x8C
		DWORD last_allowed_zone_inst;						// 0x90
		vector3_t last_allowed_global_position_in_zone;		// 0x94
		vector3_t reconcile_position;						// 0xA0
		float body_scale;									// 0xAC
		PVOID p_texture_data_list;							// 0xB0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xB4[0xC];								// 0xB4
		PVOID p_visual_mesh_t;								// 0xC0
		PVOID p_visual_cat_mesh_t;							// 0xC4
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC8[0x4];								// 0xC8
		BYTE is_client_char;								// 0xCC
		BYTE is_visible;									// 0xCD
		BYTE use_char_dist_cap;								// 0xCE
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xCF;									// 0xCF
		vector3_t bounding_sphere_pos;						// 0xD0
		// ReSharper disable once CppInconsistentNaming
		float unknown_0xDC;									// 0xDC
	} n3_visual_dynel_t, *p_n3_visual_dynel_t;


#pragma endregion

#pragma region NanoItem

	// Size = 0xC0
	typedef struct ao_nano_item  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;					// 0x04
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x34];			// 0x04
		std::vector <LONG>* p_stats_vector;	// 0x38
		identity_t parent_identity;			// 0x3C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x2C];			// 0x44
		identity_t unknown_identity;		// 0x70
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x78[0x24];			// 0x78
		char const * name;					// 0x9C
		char const * description;			// 0xA0
		identity_t nano_identity;			// 0xA4
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xAC[0xC];				// 0xAC
		DWORD radius;						// 0xB8
		BYTE is_nano_self_only;				// 0xBC
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xBC[0x3];				// 0xBD
	} nano_item_t, *p_nano_item_t;

#pragma endregion

#pragma region NanoTemplate

	// Size = 0x1C
	// From Gamecode.dll
	typedef struct ao_nano_template  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		identity_t nano_identity;		// 0x00
		DWORD start_time;				// 0x08
		DWORD duration;					// 0x0C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x4];			// 0x10
		identity_t caster_identity;		// 0x14
	} nano_template_t, *p_nano_template_t;

#pragma endregion

#pragma region NpcHolder

	typedef struct pet_node  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		struct pet_node* p_lower;
		struct pet_node* p_node;
		struct pet_node* p_higher;
		identity_t identity;
		DWORD index;
	} pet_node_t, *p_pet_node_t;

	typedef struct pet_root
	{
		p_pet_node_t p_begin;
		p_pet_node_t p_node;
		p_pet_node_t p_end;
	} pet_root_t, *p_pet_root_t;

	typedef struct pet_dir
	{
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x0[0x4];
		p_pet_root_t p_root;
		DWORD count;
	} pet_dir_t, *p_pet_dir_t;


	// Size = 0x2C
	// From Gamecode.dll
	typedef struct ao_npc_holder
	{
		PVOID p_v_table;			// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x18];	// 0x04
		p_pet_map_t p_pet_dir;		// 0x1C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x20[0xC];		// 0x20
	} npc_holder_t, *p_npc_holder_t;

#pragma endregion

#pragma region PerkHolder

	typedef struct ao_perk_holder
	{
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x00[0x4];						// 0x00
		p_perk_map_t p_perk_dir;				// 0x04
		std::vector<action_lock_t>* p_perk_locks;	// 0x08
	} perk_holder_t, *p_perk_holder_t;

#pragma endregion

#pragma region PerkMap

	typedef struct perk_node  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		struct perk_node* p_lower;
		struct perk_node* p_back;
		struct perk_node* p_higher;
		identity_t perk_identity;
		DWORD perk_id;
	} perk_node_t, *p_perk_node_t;

	typedef struct perk_root
	{
		p_perk_node_t p_begin;
		p_perk_node_t p_node;
		p_perk_node_t p_end;
	} perk_root_t, *p_perk_root_t;

	typedef struct perk_dir
	{
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x0[0x4];
		p_perk_root_t p_root;
		DWORD count;
	} perk_dir_t, *p_perk_dir_t;

#pragma endregion

#pragma region PlayfieldAnarchy

	// Size - 0x110
	// From Gamecode.dll
	typedef struct ao_playfield_anarchy
	{
		PVOID p_v_table;						// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x4];					// 0x04
		identity_t model_id;					// 0x08
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x8];					// 0x10
		identity_t instance_id;					// 0x18
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x20[0xC];					// 0x20
		PVOID p_room_links;						// 0x2C
		std::vector<p_n3_dynel_t, std::allocator<p_n3_dynel_t>> p_dynel_children;	// 0x30
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x3C[0x10];				// 0x3C
		DWORD last_entrance_door_number;		// 0x4C
		DWORD zone_size;						// 0x50						
		p_n3_tile_map_t p_n3_tile_map;			// 0x54
		p_grid_space_t p_space_i;				// 0x58
		PVOID p_pathfinder_i;					// 0x5C
		p_n3_tile_map_surface_t p_surface_i;	// 0x60
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x64[0x8];					// 0x64
		PVOID p_shadow_list;					// 0x6C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x70[0x14];				// 0x70
		PVOID p_org_zone_id_list;				// 0x84
		DWORD num_org_zones;					// 0x88
		struct ao_rdb_playfield* p_rdb_playfield;	// 0x8C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x90[0x8];					// 0x90
		DWORD number_of_waters;					// 0x98
		PVOID p_n3_water_data;					// 0x9C
		PVOID p_environment_data;				// 0xA0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xA4[0x4];					// 0xA4				
		p_n3_cell_monitor_t p_n3_cell_monitor;	// 0xA8
		LONG pf_world_x_pos;					// 0xAC
		LONG pf_world_z_pos;					// 0xB0
		PVOID p_playfield_area_info;			// 0xB4
		PVOID p_playfield_district_info;		// 0xB8
		PVOID p_fight_mode_handler;				// 0xBC
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC0[0x4];					// 0xC0
		PVOID p_game_data_land_control_map_t;	// 0xC4
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC8[0x4];					// 0xC8
		PVOID p_id_range_remapper;				// 0xCC
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xD0[0x8];					// 0xD0
		DWORD game_run_mode;					// 0xD8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xDC[0x34];				// 0xDC
	} playfield_anarchy_t, *p_playfield_anarchy_t;

#pragma endregion

#pragma region ppj::Client_c::GroupAction_t

	// Size = 0x48
	// From GUI.dll
	typedef struct ppj_client_group_action
	{
		PVOID p_v_table;		// 0x00
		// ReSharper disable CppInconsistentNaming
		DWORD unknown_0x4;	// 0x04		
		DWORD unknown_0x8;	// 0x08
		// ReSharper restore CppInconsistentNaming
		string string1;		// 0x0C
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x24[0x4];		
		DWORD unknown_0x28;	// 0x28
		DWORD unknown_0x2C;	// 0x2C
		DWORD unknown_0x30;	// 0x30
		DWORD unknown_0x34;	// 0x34
		DWORD unknown_0x38;	// 0x38
		DWORD unknown_0x3C;	// 0x3C
		DWORD unknown_0x40;	// 0x40
		DWORD unknown_0x44;	// 0x44
		// ReSharper restore CppInconsistentNaming
	} ppj_client_group_action_t, *p_ppj_client_group_action_t;

#pragma endregion

#pragma region ppj::Client_c::GroupMessage_t

	// Size = 0x78
	// From GUI.dll
	typedef struct ppj_client_group_message
	{
		PVOID p_v_table;		// 0x00
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x04;		// 0x04
		DWORD sender_id;		// 0x08
		string sender;			// 0x0C
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x24[0x4];	// 0x24	
		DWORD unknown_0x28;		// 0x28
		DWORD unknown_0x2C;		// 0x2C
		// ReSharper restore CppInconsistentNaming
		string channel_name;	// 0x30
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x48[0x4];	// 0x48
		string message;			// 0x4C
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x64[0x4];
		DWORD unknown_0x68;	// 0x68
		DWORD unknown_0x6C;	// 0x6C
		DWORD unknown_0x70;	// 0x70
		DWORD unknown_0x74;	// 0x74
		// ReSharper restore CppInconsistentNaming
	} ppj_group_message_t, *p_ppj_group_message_t;

#pragma endregion

#pragma region ppj::Client_c::PrivateGroupAction_t

	// Size = 0x4C
	// From GUI.dll
	typedef struct ppj_client_private_group_action
	{
		PVOID p_v_table;		// 0x00
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x04;		// 0x04
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x08;		// 0x08
		string string1;			// 0x0C		
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x24[0x4];	// 0x24		
		DWORD unknown_0x28;	// 0x28
		DWORD unknown_0x2C;	// 0x2C
		// ReSharper restore CppInconsistentNaming
		string string2;		// 0x30;
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x48[0x4];
	} ppj_client_private_group_action_t, *p_ppj_client_private_group_action_t;

#pragma endregion

#pragma region ppj::Client_c::PrivateMessage_t

	// Size = 0x54
	// From GUI.dll
	typedef struct ppj_client_private_message
	{
		PVOID p_v_table;		// 0x00
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x4;		// 0x04
		DWORD sender_id;		// 0x08
		string sender;			// 0x0C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x24[0x4];
		string message;			// 0x28
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x40[0x4];		
		DWORD unknown_0x44;	// 0x44
		DWORD unknown_0x48;	// 0x48
		DWORD unknown_0x4C;	// 0x4C
		DWORD unknown_0x50;	// 0x50
		// ReSharper restore CppInconsistentNaming
	} ppj_client_private_message_t, *p_ppj_client_private_message_t;

#pragma endregion

#pragma region ppj::Client_c::SystemMessage_t

	// Size = 0x48
	// From GUI.dll
	typedef struct ppj_client_system_message
	{
		PVOID p_v_table;		// 0x00
		// ReSharper disable CppInconsistentNaming
		DWORD unknown_0x4;		// 0x04		
		DWORD unknown_0x8;		// 0x08
		// ReSharper restore CppInconsistentNaming
		string string1;			// 0x0C
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x24[0x4];		
		DWORD unknown_0x28;		// 0x28
		// ReSharper restore CppInconsistentNaming
		string string2;			// 0x2C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];
	} ppj_client_system_message_t, *p_ppj_client_system_message_t;

#pragma endregion

#pragma region ppj::Client_c::VicinityMessage_t

	// Size = 0x54
	// From GUI.dll
	typedef struct ppj_client_vicinity_message
	{
		PVOID p_v_table;		// 0x00
		// ReSharper disable once CppInconsistentNaming
		DWORD unknown_0x4;		// 0x04
		DWORD sender_id;		// 0x08
		string sender;			// 0x0C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x24[0x4];
		string message;			// 0x28
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x40[0x4];	// 0x40	
		DWORD unknown_0x44;		// 0x44
		DWORD unknown_0x48;		// 0x48
		DWORD unknown_0x4C;		// 0x4C
		DWORD unknown_0x50;		// 0x50
		// ReSharper restore CppInconsistentNaming
	} ppj_client_vicinity_message_t, *p_ppj_client_vicinity_message_t;

#pragma endregion

#pragma region RDBPlayfield

	// Size = 0x6C
	// From Gamecode.dll
	typedef struct ao_rdb_playfield  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;				// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x4];			// 0x04
		identity_t resource_id;			// 0x08
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x10];		// 0x10
		char name[0x20];				// 0x20
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x40[0x18];		// 0x40
		DWORD playfield_type_bits;		// 0x58
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x5C[0x10];		// 0x5C
	} rdb_playfield_t, *p_rdb_playfield_t;

#pragma endregion

#pragma region RDBTilemap

	// Size = 0x8270
	// From DisplaySystem.dll
	typedef struct ao_rdb_tile_map  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;						// 0x00
		p_event_caster_t p_event_caster;		// 0x04
		identity_t Identity;					// 0x08
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x8];					// 0x10
		DWORD is_dungeon;						// 0x0018
		// ReSharper disable CppInconsistentNaming
		float unknown_0x1C;						// 0x001C		
		BYTE unknown_0x20[0x8];					// 0x0020
		BYTE unknown_0x28[0x200];				// 0x0028
		BYTE unknown_0x228[0x8];				// 0x0228
		// ReSharper restore CppInconsistentNaming
		WORD num_main_tiles;					// 0x0230
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x232[2];					// 0x0232
		WORD** p_main_tile_ids;					// 0x0234
		// ReSharper disable CppInconsistentNaming
		BYTE unknown_0x238[0x4];				// 0x0238		
		BYTE unknown_0x23C[0x8000];				// 0x023C
		BYTE unknown_0x823C[0x14];				// 0x823C
		WORD unknown_0x8250;					// 0x8259
		BYTE unknown_0x8252[2];					// 0x8252		
		BYTE unknown_0x8254[0x4];				// 0x8254
		// ReSharper restore CppInconsistentNaming
		PVOID p_anarchy_ground_data_db_t;		// 0x8258
		DWORD width;							// 0x825C
		DWORD height;							// 0x8260
		float map_size_multiplier;				// 0x8264
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x8268[0x8];				// 0x8268
	} rdb_tile_map_t, *p_rdb_tile_map_t;

#pragma endregion

#pragma region Reference Counted

	// Size = 0x08
	// From DatabaseController.dll
	typedef struct ao_reference_counted
	{
		PVOID p_v_table;						// 0x00
		p_event_caster_t p_event_caster;		// 0x04
	} reference_counted_t, *p_reference_counted_t;

#pragma endregion

#pragma region ResourceDatabase

	// Size = 0x10
	// From DatabaseController.dll
	typedef struct ao_resource_database
	{
		struct ao_database_controller database_controller;		// 0x00
		PVOID p_ace_token;									// 0x08
		PVOID p_rdb_link_r_db_c;							// 0x0C
	} resource_database_t, *p_resource_database_t;

#pragma endregion 

#pragma region PlayerVehicle

	typedef struct ao_player_vehicle
	{
		PVOID p_v_table;									// 0x0000
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x10];							// 0x0004
		DWORD zone_instance_id;								// 0x0014
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0xC];								// 0x0018
		PVOID p_locality_listener_i;						// 0x0024
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x28[0x4];								// 0x0028
		struct ao_dummy_vehicle* p_parent_vehicle;			// 0x002C
		PVOID p_vehicle_body_i;								// 0x0030
		float mass;											// 0x0034
		float max_force;									// 0x0038
		float max_vel1;										// 0x003C
		float brake_distance;								// 0x0040
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];								// 0x0044
		float max_vel2;										// 0x0048
		float radius;										// 0x004C
		BYTE falling_enabled;								// 0x0050
		BYTE surface_hug_enabled;							// 0x0051
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x52[0x6];								// 0x0052
		vector3_t global_pos;								// 0x0058
		vector3_t velocity;									// 0x0064
		quaternion_t direction;								// 0x0070
		quaternion_t body_rot;								// 0x0080
		float dir;											// 0x0090
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x94[0x1C];							// 0x0094
		DWORD orientation_mode;								// 0x00B0
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xB4[0xC];								// 0x00B4
		vector3_t body_forward;								// 0x00C0
		float velocity_magnitude;							// 0x00CC
		vector3_t last_pos;									// 0x00D0
		vector3_t parent_global_pos;						// 0x00DC
		quaternion_t parent_body_rot;						// 0x00E8
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xF8[0x5C];							// 0x00F8	
		struct ao_n3_tile_map_surface* p_n3_tile_map_surface;	// 0x0154
		DWORD playfield_instance_id;						// 0x0158
		BYTE surface_collision_enabled;						// 0x015C
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x15D[0x3];							// 0x015D
		PVOID pn_3dynel_event_listener_i;					// 0x0160
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x164[0x4];							// 0x0164
		identity_t identity;								// 0x0168
		float max_speed;									// 0x0170
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x174[0x4];							// 0x0174
		p_char_movement_status_t p_char_movement_status;	// 0x0178
		struct ao_char_vehicle* p_this_vehicle;				// 0x017C
															// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x180[0x22C];							// 0x0180
	} player_vehicle_t, *p_player_vehicle_t;

#pragma endregion

#pragma region SimpleChar

	// Size = 0x2D8
	// From Gamecode.dll
	typedef struct ao_simple_char  // NOLINT(cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;										// 0x00
		p_n3_dynel_event_source_t p_n3_dynel_event_source;		// 0x04
		p_event_caster_t p_event_caster_t;						// 0x08
		db_object_t db_object;									// 0x0C
		std::vector<coll_info_t>* p_coll_info_vector;				// 0x24
		n3_fsm_t n3_fsm;										// 0x28
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];									// 0x44
		BYTE is_not_snoozing;									// 0x48
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x49[0x7];									// 0x49
		p_char_vehicle_t p_vehicle;								// 0x50
		BYTE is_ground_collision_enabled;						// 0x54
		BYTE is_coll_enabled;									// 0x55
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x56[0x2];									// 0x56
		BYTE is_stationary;										// 0x58
		BYTE is_repulsive;										// 0x59
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x5A[0x2];									// 0x5A
		p_n3_dyna_mesh_coll_sphere_t p_coll_prim;				// 0x5C
		DWORD playfield_instance_id;							// 0x60
		p_n3_tile_map_t p_n3_tile_map;							// 0x64
		BYTE is_on_border;										// 0x68
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x69[0x2];									// 0x69
		BYTE is_in_tree;										// 0x6B	
		PVOID p_n3_info_item_remote_t;							// 0x6C
		std::vector<p_n3_dynel_t>* p_child_dynel_vector;				// 0x70
		identity_t parent_identity;								// 0x74
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x7C[0x4];									// 0x7C
		BYTE is_dying;											// 0x80
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x81[0xB];									// 0x81
		DWORD rdb_dynel_status;									// 0x8C
		DWORD last_allowed_zone_inst;							// 0x90
		vector3_t last_allowed_global_position_in_zone;			// 0x94
		vector3_t reconcile_position;							// 0xA0
		float body_scale;										// 0xAC
		PVOID p_texture_data_list;								// 0xB0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xB4[0xC];									// 0xB4
		PVOID p_visual_mesh_t;									// 0xC0
		PVOID p_visual_cat_mesh_t;								// 0xC4
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC8[0x4];									// 0xC8
		BYTE is_client_char;									// 0xCC
		BYTE is_visible;										// 0xCD
		BYTE use_char_dist_cap;									// 0xCE
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xCF;										// 0xCF
		vector3_t bounding_sphere_pos;							// 0xD0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xDC[0x4];									// 0xDC
		PVOID p_v_table_trigger_holder;							// 0xE0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xE4[0x8];									// 0xE4
		PVOID p_v_table_beholder;								// 0xEC
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xF0[0x30];								// 0xF0
		PVOID p_v_table_stat_holder_t;							// 0x120
		std::vector<LONG>* p_stat_vector;							// 0x124
		identity_t identity;									// 0x128
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x130[0x28];								// 0x130
		string name;											// 0x148
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x170[0x4C];								// 0x170
		p_inventory_holder_t p_container_inventory;				// 0x1BC
		p_stat_holder_t p_map_holder;							// 0x1C0
		struct ao_spell_template_data* p_spell_template_data;	// 0x1C4
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C8[0xC];								// 0x1C8
		struct ao_special_action_holder* p_special_action_holder;	// 0x1D4
		struct ao_weapon_holder* p_weapon_holder;					// 0x1D8
		p_npc_holder_t p_npc_holder;							// 0x1DC
		PVOID p_anim_holder;									// 0x1E0
		struct ao_team_raid_holder* p_team_raid_info;				// 0x1E4
		struct ao_simple_char** pp_client_control_char;			// 0x1E8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1EC[0xC];								// 0x1EC
		p_perk_holder_t p_perk_holder;							// 0x1F8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1FC[0xC];								// 0x1FC
		DWORD state;											// 0x208
		DWORD character_body_shape;								// 0x20C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x210[0x10];								// 0x210
		BYTE is_npc;											// 0x220
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x221[0xB7];								// 0x221
	} simple_char_t, *p_simple_char_t;


#pragma endregion	

#pragma region GameData::SpawnPoint

	// From GameData.dll
	// Size = 0x14
	typedef struct ao_spawn_point  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;		// 0x00
		vector3_t center_pos;	// 0x04
		float radius;			// 0x10
	} spawn_point_t, *p_spawn_point_t;


#pragma endregion

#pragma region SpecialAction

	// Size = 0x20
	typedef struct ao_special_action  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		identity_t identity;			// 0x00
		identity_t weapon_identity;		// 0x08
		DWORD locked_skill_id1;			// 0x10
		DWORD locked_skill_id2;			// 0x14
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0x4];			// 0x18
		BYTE is_locked;					// 0x1C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1D[0x3];			// 0x1D
	} special_action_t, *p_special_action_t;

#pragma endregion

#pragma region SpecialActionHolder

	// Size = 0xFC
	// From Gamecode.dll
	typedef struct ao_special_action_holder  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		p_simple_char_t p_client_control_char;				// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x58];							// 0x04
		identity_t special_action_target;					// 0x5C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x64[0x10];							// 0x64
		PVOID p_district_data;								// 0x74
		PVOID p_area_data;									// 0x78
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x7C[0x8];								// 0x7C
		std::list<special_action_t>* p_special_actions_list;		// 0x84
		p_lock_map_t p_lock_id_dir;							// 0x88
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x8C[0x70];							// 0x8C
	} special_action_holder_t, *p_special_action_holder_t;

#pragma endregion

#pragma region Spell

	// Size = 0x4
	typedef struct ao_spell
	{
		DWORD id;
	} spell_t, *p_spell_t;

#pragma endregion

#pragma region SpellTemplateData	

	typedef struct ao_casting_data  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		p_nano_item_t p_nano_item;	// 0x00
		DWORD nano_id;				// 0x04
		// ReSharper disable once CppInconsistentNaming
		LONG unknown_0x08;			// 0x08
		DWORD cast_status;			// 0x0C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x14];	// 0x10
		identity_t target;			// 0x24
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x2C[0x4];		// 0x2C
		identity_t caster;			// 0x30
	} casting_data_t, *p_casting_data_t;

	// Size = 0x50
	// From Gamecode.dll
	typedef struct ao_spell_template_data
	{
		p_simple_char_t p_client_control_dynel;			// 0x00
		DWORD in_cooldown;								// 0x04
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x08[0x4];							// 0x08
		DWORD casting_id;								// 0x0C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x10[0x4];							// 0x10
		std::list<DWORD> spell_list;							// 0x14
		// nano_id_list_t spell_list;
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C[0x4];							// 0x1C
		std::list<nano_template_t> nano_template_list;		// 0x20
		BYTE unknown_0x28[0x4];							// 0x28
		std::list<p_casting_data_t>* p_spellcasting_info;	// 0x2C
		std::vector<p_identity_t> active_nanos;				// 0x30
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x3C[0x14];						// 0x3C
	} spell_template_data_t, *p_spell_template_data_t;

#pragma endregion
		
#pragma region Static Items

	typedef struct ao_static_item  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		identity_t static_identity;
		p_dummy_item_base_t p_dummy_item;
		DWORD unknown;
	} static_item_t, *p_static_item_p;

#pragma endregion

#pragma region StatMap

	// Size = 0x14
	typedef struct stat_node
	{
		struct stat_node* p_lower;		// 0x00
		struct stat_node* p_node;		// 0x04
		struct stat_node* p_higher;		// 0x08
		DWORD stat;						// 0x0C
		LONG modifier;					// 0x10
	} stat_node_t, *p_stat_node_t;

	// Size = 0x0C
	typedef struct stat_root
	{
		p_stat_node_t p_begin;			// 0x00
		p_stat_node_t p_node;			// 0x04
		p_stat_node_t p_end;			// 0x08
	} stat_root_t, *p_stat_root_t;

	typedef struct stat_dir
	{
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x00[0x4];			// 0x00
		p_stat_root_t p_root;			// 0x04
		DWORD count;					// 0x08
	} stat_dir_t, *p_stat_dir_t;

#pragma endregion 

#pragma region StatNameMap

	// Size = 0x14
	typedef struct stat_name_node
	{
		struct stat_name_node* p_lower;		// 0x00
		struct stat_name_node* p_back;		// 0x04
		struct stat_name_node* p_higher;	// 0x08
		stat_e stat;							// 0x0C
		PCSTR p_name;						// 0x10
	} stat_name_node_t, *p_stat_name_node_t;

	// Size = 0xC
	typedef struct stat_name_root
	{
		p_stat_name_node_t p_begin;	// 0x00
		p_stat_name_node_t p_node;		// 0x04
		p_stat_name_node_t p_end;		// 0x08
	} stat_name_root_t, *p_stat_name_root_t;

	// Size = 0xC
	typedef struct stat_name_dir
	{
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x00[0x4];			// 0x00
		p_stat_name_root_t p_root;		// 0x04
		DWORD count;					// 0x08
	} stat_name_dir_t, *p_stat_name_dir_t;


#pragma endregion

#pragma region TargetingModule

	// Size 0x10
	typedef struct ao_targeting_module
	{
		void* p_v_table;					// 0x00
		unsigned char unknown_0x04[0xC];	// 0x04

	} targeting_module_t, *p_targeting_module_t;

#pragma endregion

#pragma region TeamEntry

	typedef struct ao_team_entry
	{
		string name;
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0x4];
		identity_t identity;
	} team_entry_t, *p_team_entry_t;

#pragma endregion

#pragma region TeamRaidHolder

	// ReSharper disable once CppImplicitDefaultConstructorNotAvailable
	typedef struct ao_team_raid_info  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		const char name[0x14];		// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x14[0x8];		// 0x14
		identity_t identity;		// 0x1C
	} team_raid_info_t, *p_team_raid_info_t;

	typedef struct ao_team_raid_holder  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		identity_t team_identity;					// 0x00
		p_simple_char_t p_client_control_dynel;		// 0x08
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC[0x8];						// 0x0C
		identity_t team_leader_identity;			// 0x14
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C[0x4];						// 0x1C
		std::vector<p_team_raid_info_t*>* p_team_list[6];// 0x20
		LONG raid_team_index;							// 0x38
	} team_raid_holder_t, *p_team_raid_holder_t;

#pragma endregion

#pragma region WeaponHolder

	//typedef struct weapon_item_node
	//{
	//	struct weapon_item_node* p_lower;			// 0x00
	//	struct weapon_item_node* p_node;			// 0x04
	//	struct weapon_item_node* p_higher;			// 0x08
	//	DWORD action_id;							// 0x0C
	//	struct ao_weapon_item* p_weapon_item;			// 0x10
	//} weapon_item_node_t, *p_weapon_item_node_t;

	//// Size = 0x0C
	//typedef struct weapon_item_root
	//{
	//	p_weapon_item_node_t p_begin;			// 0x00
	//	p_weapon_item_node_t p_node;			// 0x04
	//	p_weapon_item_node_t p_end;				// 0x08
	//} weapon_item_root_t, *p_weapon_item_root_t;

	//typedef struct weapon_item_dir
	//{
	//	// ReSharper disable once CppInconsistentNaming
	//	BYTE unknown_0x00[0x4];					// 0x00
	//	p_weapon_item_root_t p_root;			// 0x04
	//	DWORD count;							// 0x08
	//} weapon_item_dir_t, *p_weapon_item_dir_t;

	// Size = 0xA4
	// From Gamecode.dll
	typedef struct ao_weapon_holder  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;						// 0x00
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x04[0x4];					// 0x04
		p_simple_char_t p_client_control_char;	// 0x08
		weapon_item_dir_t weapon_item_dir;		// 0x10
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x18[0x2C];				// 0x18
		DWORD is_attacking;						// 0x44
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x48[0x4];					// 0x48
		identity_t weapon_target_identity;		// 0x4C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x54[0x50];					// 0x54
	} weapon_holder_t, *p_weapon_holder_t;

#pragma endregion		

#pragma region WeaponItem

	// Size = 0x204
	// From Gamecode.dll
	typedef struct ao_weapon_item  // NOLINT(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
	{
		PVOID p_v_table;									// 0x0000
		PVOID p_v_table_n3_dynel_event_source;				// 0x0004
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x08[0x4];								// 0x0008
		PVOID p_v_table_db_object;							// 0x000C
		p_event_caster_t p_event_caster;					// 0x0010
		identity_t instance_identity;						// 0x0014
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1C[0x8];								// 0x001C
		std::vector<coll_info_t>* p_coll_info_vector;			// 0x0024
		PVOID p_v_table_n3_fsm;								// 0x0028
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x2C[0x10];							// 0x002C
		BYTE is_dead;										// 0x003C
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x3D[0x3];								// 0x003D
		p_n3_sf_base_t pFirstStateFunction;					// 0x0040
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x44[0x4];								// 0x0044
		BYTE is_not_snoozing;								// 0x0048
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x49[0x7];								// 0x0049
		p_dummy_vehicle_t p_vehicle;						// 0x0050
		BYTE is_ground_collision_enabled;					// 0x0054
		BYTE is_coll_enabled;								// 0x0055
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x56[0x2];								// 0x0056
		BYTE is_stationary;									// 0x0058
		BYTE is_repulsive;									// 0x0059
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x5A[0x2];								// 0x005A
		p_n3_dyna_mesh_coll_sphere_t p_coll_prim;			// 0x005C
		DWORD playfield_instance_id;						// 0x0060
		p_n3_tile_map_t p_n3_tile_map;						// 0x0064
		BYTE is_on_border;									// 0x0068
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x69[0x2];								// 0x0069
		BYTE is_in_tree;									// 0x006B	
		PVOID p_n3_info_item_remote;						// 0x006C
		std::vector<p_n3_dynel_t>* p_child_dynel_vector;			// 0x0070
		identity_t parent_identity;							// 0x0074
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x7C[0x4];								// 0x007C
		BYTE is_dying;										// 0x0080
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown0x81[0xB];								// 0x0081
		DWORD rdb_dynel_status;								// 0x008C
		DWORD last_allowed_zone_inst;						// 0x0090
		vector3_t last_allowed_global_position_in_zone;		// 0x0094
		vector3_t reconcile_position;						// 0x00A0
		float body_scale;									// 0x00AC
		PVOID p_texture_data_list;							// 0x00B0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xB4[0xC];								// 0x00B4
		PVOID p_visual_mesh;								// 0x00C0
		PVOID p_visual_cat_mesh;							// 0x00C4
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC8[0x4];								// 0x00C8
		BYTE is_client_char;								// 0x00CC
		BYTE is_visible;									// 0x00CD
		BYTE use_char_dist_cap;								// 0x00CE
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xCF;									// 0x00CF
		vector3_t bounding_sphere_pos;						// 0x00D0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xDC[0x4];								// 0x00DC
		identity_t static_identity;							// 0x00E0
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xE8[0x70];							// 0x00E8
		PVOID p_v_table_weapon_item;						// 0x0158
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x15C[0x30];							// 0x015C
		PVOID p_v_table_stat_holder;						// 0x018C
		std::vector<LONG>* p_stat_vector;						// 0x0190
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x194[0x34];							// 0x0194
		identity_t unknown_identity2;						// 0x01C8
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x1D0[0x24];							// 0x01D0
		char const * name;									// 0x01F4
		char const * description;							// 0x01F8
		PVOID p_v_table_trigger_holder;						// 0x01FC
		// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x200[0x4];								// 0x0200
	} weapon_item_t, *p_weapon_item_t;

#pragma endregion

#pragma region WindowController

	// Size = 0x198
	typedef struct ao_window_controller
	{
		PVOID p_v_table_render_sprite_base_t;

	} window_controller_t, *p_window_controller_t;


#pragma endregion


#pragma region CommandList

	typedef struct ao_command_list
	{
		BYTE unknown0x00[0x4];
		p_command_map_t p_command_map;
		BYTE unknown0x08[0x4];
	} command_list_t, *p_command_list_t;

#pragma endregion

#pragma region InputConfig_t::PIC_ID_List_s

	typedef struct ao_pic_id_list_item
	{
		PCSTR name;
		DWORD id;		
	} pic_id_list_item, *p_pic_id_list_item;

	template <size_t Size>
	struct ao_pic_id_list
	{
		BYTE unknown_0x00[0x8];
		struct ao_pic_id_list_item list[Size];

	};

	typedef ao_pic_id_list<0x7E> input_method_id_table;
	typedef ao_pic_id_list<0x3F> input_mode_id_table;

#pragma endregion

#pragma region KeyPressInfo_t

	typedef union ao_keypress_info 
	{
		DWORD info{};
		struct
		{
			WORD key;
			WORD modifier;
		};

		ao_keypress_info()
		{
			this->info = 0;
		}

		explicit ao_keypress_info(const DWORD info)
		{
			this->info = info;
		}

		explicit ao_keypress_info(const WORD key, const WORD modifier = 0)
		{
			this->key = key;
			this->modifier = modifier;
		}

		explicit ao_keypress_info(const ao::input_model_e key)
		{
			this->key = key;
			this->modifier = 0;
		}

		explicit ao_keypress_info(const DWORD info, const bool key_down, const bool use_control = false, const bool use_alt = false, const bool use_shift = false)
		{
			this->info = info;
			if (!key_down)
			{
				this->key_up();
			}
			if (use_control)
			{
				this->control_down();
			}
			if (use_alt)
			{
				this->alt_down();
			}
			if (use_shift)
			{
				this->shift_down();
			}
		}

		void key_down()
		{
			this->modifier |= 1 << 4;
		}

		void key_up()
		{
			this->modifier &= 1 << 4;
		}

		void shift_down()
		{
			this->modifier |= 1 << 1;
		}

		void shift_up()
		{
			this->modifier &= 1 << 1;
		}

		void control_down()
		{
			this->modifier |= 1 << 2;
		}

		void control_up()
		{
			this->modifier &= 1 << 2;
		}

		void alt_down()
		{
			this->modifier |= 1 << 3;
		}

		void alt_up()
		{
			this->modifier &= 1 << 3;
		}

	} keypress_info_t, *p_keypress_info_t;

#pragma endregion

#pragma region KDTreeSurface

	// Size = 0x14
	typedef struct ao_kd_tree_surface
	{
		void* p_v_table;					// 0x00
		unsigned char unknown_0x04[0x4];	// 0x04
		unsigned char unknown_0x08[0x4];	// 0x08
		unsigned char unknown_0x0C[0x4];	// 0x0C
		unsigned char unknown_0x10[0x4];	// 0x10
	} kd_tree_surface_t, *p_kd_tree_surface_t;
	
#pragma endregion

#pragma region n3SurfaceResource

	typedef struct ao_n3_surface_resource
	{
		db_object_t db_object;						// 0x00
		kd_tree_surface_t kd_tree_surface;			// 0x18
		p_kd_tree_surface_t p_invisible_surface;	// 0x2C
		p_kd_tree_surface_t p_teleportal_surface;	// 0x30
		void* p_portal_t_teleport_area;				// 0x34
		void* p_teleport_destination_playfield;		// 0x38
		unsigned long teleport_localizer_type;		// 0x3C
		void* p_teleport_localizer_instance;		// 0x40
	} n3_surface_resource_t, *p_n3_surface_resource_t;

#pragma region

#pragma region CellSurface_t

	// Size = 0x30
	typedef struct ao_cell_surface
	{
		void* p_v_table;
		std::vector<std::vector<struct ao_kd_tree_surface*>*> kd_trees;
		unsigned char unknown_0x10[0x4];	// 0x10
		unsigned long width_of_cell_array;	// 0x14
		unsigned long unknown_0x18;			// 0x18
		float unknown_0x1C;					// 0x1C
		float unknown_0x20;					// 0x20
		float cell_size_x;					// 0x24
		float cell_size_z;					// 0x28
		unsigned char surface_ownership;	// 0x2C
		unsigned char unknown_0x2D[0x3];	// 0x2D
	} cell_surface_t, *p_cell_surface_t;

#pragma endregion

//#pragma region Graphics_n::RRefFrame_t
//
//	typedef struct ao_r_ref_frame
//	{
//
//	} r_ref_frame_t, *p_r_ref_frame_t;
//
//#pragma endregion
//
//#pragma region Graphics_n::Renderable_c
//
//	typedef struct ao_renderable
//	{
//
//	} renderable_t, *p_renderable_t;
//
//#pragma endregion
//		
//
//#pragma region Graphics_n::GroundChunk_c
//
//	typedef struct ao_ground_chunk
//	{
//
//	} ground_chunk_t, *p_ground_chunk;
//
//#pragma endregion

#pragma region Serializable_c

	typedef struct ao_serializable
	{
		/*0x00*/ void* p_v_table;
		/*0x04*/ unsigned long serializable_id;
	} serializable_t, *p_serializable_t;

#pragma endregion

#pragma region RResource_t

	// Size = 0x2C
	typedef struct ao_r_resource_t  // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
	{
		/*0x00*/ void* p_v_table;
		/*0x04*/ std::string name;
		/*0x20*/ unsigned char unknown_0x20;
		/*0x21*/ unsigned char unknown_0x21[0x3];
		/*0x24*/ unsigned long num_references;
		/*0x28*/ unsigned long unknown_0x28;
	} r_resource_t, *p_r_resource_t;

#pragma endregion

#pragma region AnarchyGroundDataDB_t

	// Size = 0xF0
	typedef struct ao_anarchy_ground_data_db
	{
		/*0x00*/ void* p_v_table;
		/*0x04*/ std::string name;
		/*0x20*/ unsigned char unknown_0x20;
		/*0x21*/ unsigned char unknown_0x21[0x3];
		/*0x24*/ unsigned long num_references;
		/*0x28*/ unsigned long unknown_0x28;
		/*0x2C*/ unsigned long unknown_0x2C;
	} anarchy_ground_data_db, *p_anarchy_ground_Data_db_t;

#pragma endregion

#pragma region n3RoomSurface_t

	typedef struct ao_n3_room_surface
	{
		/*0x00*/ void* p_v_table;
		/*0x04*/ unsigned long unknown_0x04;
		/*0x08*/ unsigned long unknown_0x08;
		/*0x0C*/ float unknown_0x0C;
		/*0x10*/ float unknown_0x10;
		/*0x14*/ float unknown_0x14;
		/*0x18*/ float unknown_0x18;
	} n3_room_surface_t, *p_n3_room_surface_t;

#pragma endregion

#pragma region ACGBuildingPlayfield_t

	// Size = 0x114
	typedef struct ao_acg_building_playfield
	{
		/*0x0000*/ void* p_v_table;
		/*0x0004*/ unsigned char unknown_0x04[0x4];
		/*0x0008*/ identity_t model_id;
		/*0x0010*/ unsigned char unknown_0x10[0x8];
		/*0x0018*/ identity_t instance_id;
		/*0x0020*/ unsigned char unknown_0x20[0xC];
		PVOID p_room_links;						// 0x2C
		std::vector<p_n3_dynel_t, std::allocator<p_n3_dynel_t>> p_dynel_children;	// 0x30
																					// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x3C[0x10];				// 0x3C
		DWORD last_entrance_door_number;		// 0x4C
		DWORD zone_size;						// 0x50						
		p_n3_tile_map_t p_n3_tile_map;			// 0x54
		p_grid_space_t p_space_i;				// 0x58
		PVOID p_pathfinder_i;					// 0x5C
		p_n3_tile_map_surface_t p_surface_i;	// 0x60
												// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x64[0x8];					// 0x64
		PVOID p_shadow_list;					// 0x6C
												// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x70[0x14];				// 0x70
		PVOID p_org_zone_id_list;				// 0x84
		DWORD num_org_zones;					// 0x88
		struct ao_rdb_playfield* p_rdb_playfield;	// 0x8C
													// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0x90[0x8];					// 0x90
		DWORD number_of_waters;					// 0x98
		PVOID p_n3_water_data;					// 0x9C
		PVOID p_environment_data;				// 0xA0
												// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xA4[0x4];					// 0xA4				
		p_n3_cell_monitor_t p_n3_cell_monitor;	// 0xA8
		LONG pf_world_x_pos;					// 0xAC
		LONG pf_world_z_pos;					// 0xB0
		PVOID p_playfield_area_info;			// 0xB4
		PVOID p_playfield_district_info;		// 0xB8
		PVOID p_fight_mode_handler;				// 0xBC
												// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC0[0x4];					// 0xC0
		PVOID p_game_data_land_control_map_t;	// 0xC4
												// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xC8[0x4];					// 0xC8
		PVOID p_id_range_remapper;				// 0xCC
												// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xD0[0x8];					// 0xD0
		DWORD game_run_mode;					// 0xD8
												// ReSharper disable once CppInconsistentNaming
		BYTE unknown_0xDC[0x34];				// 0xDC
	} acg_building_playfield_t, *p_acg_building_playfield_t;

#pragma endregion


}