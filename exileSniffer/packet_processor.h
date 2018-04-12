#pragma once
#include "stdafx.h"
#include "packet_capture_thread.h"
#include "key_grabber_thread.h"

enum eDecodingErr{ eNoErr, eErrUnderflow, eBadPacketID, ePktIDUnimplemented};


class STREAMDATA {
public:
	CryptoPP::Salsa20::Encryption toLoginSalsa, fromLoginSalsa;
	CryptoPP::Salsa20::Encryption fromGameSalsa, toGameSalsa;
	unsigned long packetCount = 0;
	KEYDATA *workingRecvKey = NULL;
	KEYDATA *workingSendKey = NULL;
	unsigned short lastPktID = 0;
};

class packet_processor :
	public base_thread
{
public:

	packet_processor(key_grabber_thread *keyGrabPtr, SafeQueue<UI_MESSAGE>* uiq)
	{		keyGrabber = keyGrabPtr; uiMsgQueue = uiq; 	}
	~packet_processor() {};

private:
	UIDecodedPkt * testpk;

	void main_loop();

	void init_packetDeserialisers();

	bool process_packet_loop();
	void handle_patch_data(std::vector<byte> pkt);
	void handle_login_data(std::vector<byte> pkt);
	void handle_game_data(std::vector<byte> pkt);

	void handle_packet_from_loginserver(networkStreamID streamID, byte* data, unsigned int dataLen);
	void handle_packet_to_loginserver(networkStreamID streamID, byte* data, unsigned int dataLen);
	void handle_packet_from_patchserver(byte* data, unsigned int dataLen);
	void handle_packet_to_patchserver(byte* data, unsigned int dataLen);
	void handle_packet_from_gameserver(networkStreamID streamID, byte* data, unsigned int dataLen, long long timems);
	void handle_packet_to_gameserver(networkStreamID streamID, byte* data, unsigned int dataLen, long long timems);

	inline void consume_add_byte(std::wstring name, UIDecodedPkt *uipkt) {	uipkt->add_byte(name, consume_Byte());}
	inline void consume_add_word(std::wstring name, UIDecodedPkt *uipkt) { uipkt->add_word(name, consumeUShort()); }
	inline void consume_add_dword(std::wstring name, UIDecodedPkt *uipkt) { uipkt->add_dword(name, consume_DWORD()); }

	void deserialise_SRV_PKT_ENCAPSULATED(UIDecodedPkt *);
	void deserialise_CLI_CHAT_MSG_ITEMS(UIDecodedPkt *);
	void deserialise_CLI_CHAT_MSG(UIDecodedPkt *);
	void deserialise_CLI_CHAT_COMMAND(UIDecodedPkt *);
	void deserialise_SRV_CHAT_MESSAGE(UIDecodedPkt *);
	void deserialise_SRV_SERVER_MESSAGE(UIDecodedPkt *);
	void deserialise_CLI_LOGGED_OUT(UIDecodedPkt *);
	void deserialise_CLI_PING_CHALLENGE(UIDecodedPkt *);
	void deserialise_SRV_PING_RESPONSE(UIDecodedPkt *);		
	void deserialise_SRV_AREA_INFO(UIDecodedPkt*);
	void deserialise_SRV_PRELOAD_MONSTER_LIST(UIDecodedPkt*);
	void deserialise_SRV_UNK_0x13(UIDecodedPkt *);
	void deserialise_SRV_PLAYER_ITEMS(UIDecodedPkt*);
	void deserialise_CLI_CLICKED_GROUND_ITEM(UIDecodedPkt *);
	void deserialise_CLI_ACTION_PREDICTIVE(UIDecodedPkt *);
	void deserialise_SRV_INSTANCE_SERVER_DATA(UIDecodedPkt *);
	void deserialise_CLI_PICKUP_ITEM(UIDecodedPkt *);
	void deserialise_CLI_PLACE_ITEM(UIDecodedPkt *);
	void deserialise_CLI_REMOVE_SOCKET(UIDecodedPkt *);
	void deserialise_CLI_INSERT_SOCKET(UIDecodedPkt *);

	void deserialise_CLI_LEVEL_SKILLGEM(UIDecodedPkt *);
	void deserialise_CLI_SKILLPOINT_CHANGE(UIDecodedPkt *);
	void deserialise_CLI_CANCEL_BUF(UIDecodedPkt *);
	void deserialise_CLI_SET_HOTBARSKILL(UIDecodedPkt *);
	void deserialise_SRV_SKILL_SLOTS_LIST(UIDecodedPkt *);
	
	void deserialise_CLI_USE_BELT_SLOT(UIDecodedPkt *);
	void deserialise_CLI_USE_ITEM(UIDecodedPkt *);
	
	void deserialise_CLI_UNK_x56(UIDecodedPkt *);
	void deserialise_SRV_UNK_ACCT_SOCIAL_NAME(UIDecodedPkt *);
	void deserialise_CLI_REQUEST_PUBLICPARTIES(UIDecodedPkt *);
	void deserialise_SRV_RESPOND_PUBLICPARTIES(UIDecodedPkt *);
	void deserialise_SRV_CREATE_ITEM(UIDecodedPkt *);
	void deserialise_SRV_SLOT_ITEMSLIST(UIDecodedPkt *);

	void deserialise_CLI_SKILLPANE_ACTION(UIDecodedPkt *);
	void deserialise_CLI_MICROSTRANSACTIONPANE_ACTION(UIDecodedPkt *);
	void deserialise_CLI_PACKET_EXIT(UIDecodedPkt *);
	void deserialise_CLI_USED_SKILL(UIDecodedPkt *);
	void deserialise_CLI_CLICK_OBJ(UIDecodedPkt *);
	void deserialise_CLI_MOUSE_HELD(UIDecodedPkt *);
	void deserialise_CLI_MOUSE_RELEASE(UIDecodedPkt *);
	void deserialise_SRV_DISPLAY_BUILTIN_MSG(UIDecodedPkt *);
	void deserialise_CLI_GUILD_CREATE(UIDecodedPkt *);
	void deserialise_SRV_MOBILE_USED_SKILL(UIDecodedPkt *);
	void deserialise_SRV_MOBILE_UNK_0xeb(UIDecodedPkt*);
	void deserialise_SRV_MOBILE_UPDATE_HMS(UIDecodedPkt *);
	void deserialise_SRV_STAT_CHANGED(UIDecodedPkt *);
	void deserialise_CLI_REQUEST_PLAYERID(UIDecodedPkt *);
	void deserialise_SRV_NOTIFY_PLAYERID(UIDecodedPkt *);
	void deserialise_SRV_UNKNOWN_0x111(UIDecodedPkt *);
	void deserialise_CLI_OPTOUT_TUTORIALS(UIDecodedPkt *);
	void deserialise_SRV_HEARTBEAT(UIDecodedPkt *uipkt);
	void deserialise_SRV_ADD_OBJECT(UIDecodedPkt *uipkt);

	byte consume_Byte();
	unsigned short consumeUShort();
	unsigned long consume_DWORD();
	std::wstring consumeWString(size_t bytesLength);
	void discard_data(ushort byteCount);
	DWORD customSizeByteGet();
	DWORD customSizeByteGet_signed();

	bool sanityCheckPacketID(unsigned short pktID);
	void emit_decoding_err_msg(unsigned short msgID, unsigned short lastMsgID);



private:
	vector<UIDecodedPkt *> pktVec;

	networkStreamID lastActiveConnectionID;
	key_grabber_thread * keyGrabber;
	std::map<networkStreamID, STREAMDATA> streamDatas;
	map<unsigned long, std::pair<KEYDATA *, KEYDATA *> > pendingGameserverKeys;
	map<networkStreamID, unsigned long> connectionIDStreamIDmapping;
	SafeQueue<UI_MESSAGE>* uiMsgQueue;

	typedef void (packet_processor::*deserialiser)(UIDecodedPkt *);
	map<unsigned short, deserialiser> packetDeserialisers;

	HANDLE patchpipe = NULL;
	HANDLE loginpipe = NULL;
	HANDLE gamepipe = NULL;

	DWORD activeClientPID = 0;
	byte *decryptedBuffer = NULL;
	size_t remainingDecrypted = 0, decryptedIndex = 0;
	eDecodingErr errorFlag = eDecodingErr::eNoErr;
	unsigned long errorCount = 0;

	bool unfinishedPacket = false;

	std::map <unsigned long, std::string> gameObjHashes;
	std::map <unsigned long, std::string> monsterHashes;
	std::map <unsigned long, std::string> chestHashes;
	std::map <unsigned long, std::string> characterHashes;
	std::map <unsigned long, std::string> NPCHashes;
	std::map <unsigned long, std::string> petHashes;
	std::map <unsigned long, std::string> itemHashes;

	std::map <unsigned long, std::string> areaCodes;
	std::vector <std::string> monsterVarieties;
};
