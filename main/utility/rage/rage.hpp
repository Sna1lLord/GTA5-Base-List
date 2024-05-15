#ifndef _RAGE_HPP
#define _RAGE_HPP

#include <common.hpp>
#include "../definitions.hpp"

class scrNativeCallContext;
class scrNativeRegistration;

using scrNativeHash = uint64_t;
using scrNativeMapping = std::pair<scrNativeHash, scrNativeHash>;
using scrNativeHandler = void(*)(scrNativeCallContext*);

class scrNativeCallContext {
public:
    void Reset() {
        argumentCount = 0;
        dataCount = 0;
    }

    template <typename T>
    void Push(T&& value) {
        static_assert(sizeof(T) <= sizeof(uint64_t));
        *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<uint64_t*>(arguments) + argumentCount++) = std::forward<T>(value);
    }

    template <typename T>
    T& Get(size_t index) {
        static_assert(sizeof(T) <= sizeof(uint64_t));
        return *reinterpret_cast<T*>(reinterpret_cast<uint64_t*>(arguments) + index);
    }

    template <typename T>
    void Set(size_t index, T&& value) {
        static_assert(sizeof(T) <= sizeof(uint64_t));
        *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<uint64_t*>(arguments) + index) = std::forward<T>(value);
    }

    template <typename T>
    T* GetReturn() {
        return reinterpret_cast<T*>(returnValue);
    }

    template <typename T>
    void SetReturn(T&& value) {
        *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(returnValue) = std::forward<T>(value);
    }

    template <typename T>
    void SetReturn(T& value) {
        *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(returnValue) = std::forward<T>(value);
    }

protected:
    void* returnValue;
    uint32_t argumentCount;
    void* arguments;
    int32_t dataCount;
    uint32_t data[48];
};

class scrNativeRegistrationTable {
    scrNativeRegistration* entries[0xFF];
    uint32_t unk;
    bool initialized;
};

class CPed;

class CPlayerInfo {
public:
    void SetSwimSpeed(float value) {
        *(float*)(this + 0x1C8) = value;
    }
    int GetGameState() {
        return *(int*)(this + 0x230);
    }
    CPed* GetPed() {
        return *(CPed**)(this + 0x240);
    }
    void SetFrameFlags(uint32_t value) {
        *(uint32_t*)(this + 0x270) = value;
    }
    uint32_t GetFrameFlags() {
        return *(uint32_t*)(this + 0x270);
    }
    uint32_t GetNpcIgnore() {
        return *(uint32_t*)(this + 0x8C0);
    }
    int GetWantedLevel() {
        return *(int*)(this + 0x8D8);
    }
    void SetWantedLevel(int value) {
        *(int*)(this + 0x8D8) = value;
    }
    void SetRunSpeed(float value) {
        *(float*)(this + 0xD40) = value;
    }
    void SetStamina(float value) {
        *(float*)(this + 0xD44) = value;
    }
    void SetStaminaRegen(float value) {
        *(float*)(this + 0xD48) = value;
    }
};

class CPed {
public:
    bool GetInvisibility() {
        return (*(uint8_t*)(this + 0x2C) == 1);
    }

    Vector3 GetCoordinates() {
		return *(Vector3*)(this + 0x90);
	}

    uint32_t GetDamageBits() {
        return *(uint32_t*)(this + 0x188);
    }

    bool GetInvincibility() {
        return *(bool*)(this + 0x189);
    }
    void SetInvincibility(bool enabled) {
        *(bool*)(this + 0x189) = enabled;
    }

    float GetHealth() {
        return *(float*)(this + 0x280);
    }
    void SetHealth(float value) {
        *(float*)(this + 0x280) = value;
    }

    float GetMaxHealth() {
        return *(float*)(this + 0x284);
    }
    void SetMaxHealth(float value) {
        *(float*)(this + 0x284) = value;
    }

    CPlayerInfo* GetPlayerInfo() {
        return *(CPlayerInfo**)(this + 0x10A8);
    }

    float GetArmour() {
        return *(float*)(this + 0x150C);
    }
    void SetArmour(float value) {
        *(float*)(this + 0x150C) = value;
    }
};

class CPedFactory {
public:
    char pad_0000[8]; //0x0000
    CPed* m_local_ped;
};

union netAddress {
    uint32_t m_packed; //0x0000
    struct {
        uint8_t m_field4; //0x0000
        uint8_t m_field3; //0x0001
        uint8_t m_field2; //0x0002
        uint8_t m_field1; //0x0003
    };
};

class rlGamerHandle {
public:
    uint64_t m_rockstar_id; //0x0000
    uint8_t m_platform; //0x0008
    uint8_t unk_0009; //0x0009

    inline rlGamerHandle() = default;

    inline rlGamerHandle(uint64_t rockstar_id) :
        m_rockstar_id(rockstar_id),
        m_platform(3),
        unk_0009(0)
    {
    }
};

class rlGamerInfoBase {
public:
    char pad_0000[0x8];              //0x0000
    uint64_t m_peer_id;              //0x0008
    rlGamerHandle m_gamer_handle;    //0x0010
    char m_aes_key[0x20];            //0x0020
    char pad_0040[0x8];              //0x0040
    netAddress m_unk_ip;             //0x0048
    uint16_t m_unk_port;             //0x004C
    netAddress m_relay_ip;           //0x0050
    uint16_t m_relay_port;           //0x0054
    char pad_0058[0x12];             //0x0056
    char pad_0068[0x40];             //0x0068
    netAddress m_external_ip;        //0x00A8
    uint16_t m_external_port;        //0x00AC
    netAddress m_internal_ip;        //0x00B0
    uint16_t m_internal_port;        //0x00B4
    char pad_00B6[0x2];              //0x00B6
    uint32_t unk_00B8;               //0x00B8
    char pad_00BC[0x4];              //0x00BC
};

class rlGamerInfo : public rlGamerInfoBase {
public:
    uint64_t m_host_token;
    union
    {
        rlGamerHandle m_gamer_handle_2;
        uint32_t m_peer_id_2; // not found in all instances
    };
    uint32_t m_ros_privilege;
    char m_name[17];
};

class CNetGamePlayer;

class netPlayer {
public:
    virtual void* _0x00();
    virtual void* _0x08();
    virtual uint32_t _0x10();
    virtual void* _0x18(void*);
    virtual bool _0x20(void*);
    virtual bool _0x28(void**);
    virtual void destructor();
    virtual void reset();
    virtual bool is_valid();
    virtual const char* get_name();
    virtual void _0x50();
    virtual bool is_host();
    virtual rlGamerInfo* get_net_data();
    virtual void _0x68();

    char pad_0008[8]; //0x0008
    void* m_non_physical_player; //0x0010
    uint32_t m_msg_id; //0x0018
    char pad_001C[4]; //0x001C
    uint8_t m_active_id; //0x0020
    uint8_t m_player_id; //0x0021
    char pad_0022[3]; //0x0022
    uint16_t m_complaints; //0x0026
    char pad_0027[17]; //0x0028
    class CNetGamePlayer* m_unk_net_player_list[10]; //0x0040
    char pad_0090[4]; //0x0090
    uint64_t pad_0098; //0x0098
};

class CNetGamePlayer : public netPlayer {
public:
    class CPlayerInfo* m_player_info; //0x00A0
};

#endif