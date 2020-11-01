/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#ifndef USE_STEAMWORKS
///#define USE_STEAMWORKS 1
#endif

#if defined(USE_STEAMWORKS)

#pragma warning(disable:4996)
#pragma warning(disable:4101)

#include <steam/steam_api.h>
#include <steam/steam_gameserver.h>
#include <steam/steamclientpublic.h>
#include <steam/isteamcontroller.h>
#include <steam/steamencryptedappticket.h>
#include <Windows.h>

#include "LinkedList.h"
#include "ZIndexed.h"
#include "Strings.h"
#include "GLImage.h"

#define MY_STEAM_APPID ((uint32)0)  /* Just a hint */
#define STEAM_APP_GAMEDIR "Apolune"
#define MINIDUMP_CONTENT  "Minidump comment: Apolune.exe\n" 
#define STEAMWORKS_LEADERBOARD_MAX_TRANSMIT_ENTRIES 20
#define MY_SERVER_VERSION "1.0.0.0"                          // Current game server version
#define MY_AUTHENTICATION_PORT 8766                          // UDP port for the spacewar server to do authentication on (ie, talk to Steam on)
#define MASTER_SERVER_UPDATER_PORT 27016                     //  UDP port for the master server updater to listen on
#define SERVER_CONNECTION_RETRY_MILLISECONDS 350             // How long to wait for a response from the server before resending our connection attempt
#define SERVER_TIMEOUT_MILLISECONDS 5000                     // How long to wait for a client to send an update before we drop its connection server side
#define MAX_SPACEWAR_PACKET_SIZE 1024*512                    // Maximum packet size in bytes
#define MAX_PLAYERS_PER_SERVER 4                             // Maximum number of players who can join a server and play simultaneously
#define MILLISECONDS_BETWEEN_ROUNDS 4000                     // Time to pause wait after a round ends before starting a new one
#define MILLISECONDS_CONNECTION_TIMEOUT 30000                // Time to timeout a connection attempt in
#define SERVER_UPDATE_SEND_RATE 60                           // How many times a second does the server send world updates to clients
#define CLIENT_UPDATE_SEND_RATE 30                           // How many times a second do we send our updated client state to the server
#define MAX_CLIENT_AND_SERVER_FPS 86                         // How fast does the server internally run at?

extern int64 steam_app; // Actual APP ID

// Helper macros
#define STEAM_ERROR(evt)      OUTPUT("%s: Error thrown as result, %d\n",evt,SteamResultToString(result->m_eResult).c_str());
#define STEAM_LOGIN_REQUIRED  {if ( !SteamUser()->BLoggedOn() ) return false;}
// Callback for Deferred Results
#define STEAM_CR_PROTO(a,b)   void a( b*data, bool bIOFAilure )
#define STEAM_CR_FUN(a,b)     void SteamRequestCallback::a(b*result,bool io_error )
#define STEAM_CR(a,b,c)       CCallResult<SteamRequestCallback, a> b; STEAM_CR_PROTO(c,a);
#define STEAM_API(a)          SteamAPICall_t hSteamAPICall = a;
#define STEAM_CR_SET(a,b,f)   {STEAM_API(a); b.Set(hSteamAPICall,this,&SteamRequestCallback::f); return true;}
// Callbacks for Triggered Events
#define	STEAM_CBV(b,c,d)      STEAM_CALLBACK( SteamRequestCallback, b, c, d );
#define STEAM_CB(b,c)         STEAM_CALLBACK( SteamRequestCallback, b, c )
#define STEAM_CB_FUN(a,b)     void SteamRequestCallback::a(b*result)
#define STEAM_NOT(t)          {if ( SteamPendingRequests(t) > 0 ) return false;}
#define STEAM_CMD(t)          {type=t; if ( !SteamUser()->BLoggedOn() ) return false;}
#define STEAM_CMD_(t,r)       {type=t; if ( !SteamUser()->BLoggedOn() ) return r;}
#define STEAM_CMDV(t)         {type=t; if ( !SteamUser()->BLoggedOn() ) return;}
#define STEAM_SINGLETON(t)    {if ( SteamPendingRequests(t) == 0 ) {type=t; STEAM_LOGIN_REQUIRED;} else return false;}
#define STEAM_NOWHERE_TO_GO(name,req) { OUTPUT("%s: Steam couldn't return data received for `%s`\n",name,req); }

std::string SteamResultToString( EResult e );

// Contains all callbacks and acts on objects
enum SteamRequestType { // Used to detect quickly if the request is already active..
 steam_Anonymous,
 // User Stats and Achievements
 steam_GetUserStats,
 steam_SetUserStats, 
 // Leadboards
 steam_FindLeaderboard,
 steam_GetLeaderboard,
 steam_SetLeaderboard,
 // Favorites
 steam_GetFavoriteGameCount,
 // Lobbies
 steam_GetLobbyList,
 steam_CreateLobby,
 steam_JoinLobby,
 steam_LeaveLobby,
 steam_GetLobbyData,
 steam_RequestLobbyData,   	// Refreshes metadata for a lobby you're not necessarily in right now
 steam_InviteToLobby,
 steam_GetLobbyGameServer,
 steam_GetCountLobbyMembers,
 steam_GetLobbyMemberLimit,
 steam_GetLobbyMemberData,
 steam_DeleteLobbyData,
 steam_GetLobbyOwner,
 steam_GetLobbyChat,
 steam_SetLobbyData,
 steam_SetLobbyGameServer,
 steam_SetLobbyMemberLimit,
 steam_SetLobbyMemberData,
 steam_SetLobbyType,
 steam_SetLobbyJoinable,
 steam_SetLobbyOwner,
 steam_SendLobbyChat,
 steam_SetLinkedLobby
};

// Let's us determine, by accessing the global SteamWorks object, how many of a given type are still pending.
// Implemented because you cannot "set" achievements until your "get request" is finished, etc.
int SteamPendingRequests(SteamRequestType type);

// Base class
ONE(SteamNode,{id=0;})
 int64 id;
 Zbool pending;
MANY(SteamNode,SteamNodeHandle,SteamNodeHandles,"SteamNode",SteamNodes,{})
DONE(SteamNode);

// Steam user profiles
ONE(SteamProfile,{})
 CSteamID steam_id;
 Zp<GLImage> image;
 Strings metadata;
MANY(SteamProfile,SteamProfileHandle,SteamProfileHandles,"SteamProfile",SteamProfiles,{})
DONE(SteamProfile);

ONE(SteamLobbyChat,{})
MANY(SteamLobbyChat,SteamLobbyChatHandle,SteamLobbyChatHandles,"SteamLobbyChat",SteamLobbyChats,{})
DONE(SteamLobbyChat);


// Valve's messaging functions for debugging and error reporting.
void MiniDumpFunction( unsigned int nExceptionCode, EXCEPTION_POINTERS *pException );
int Alert( const char *lpCaption, const char *lpText );
extern "C" void __cdecl SteamAPIDebugTextHook( int nSeverity, const char *pchDebugText );

enum SteamLobbySearchCriteriaTypes {
 steam_LobbySearchString=0,
 steam_LobbySearchNumber=1,
 steam_LobbySearchNear=2,
 steam_LobbySearchDistance=3,
 steam_LobbySearchSlots=4,
 steam_CompatibleMembers=5
};

ONE(SteamLobbySearchCriteria,{
 this->type=steam_LobbySearchString;
 this->comparison=k_ELobbyComparisonEqual;
 this->distance=k_ELobbyDistanceFilterDefault;
})
 SteamLobbySearchCriteriaTypes type;
 ELobbyComparison comparison;
 ELobbyDistanceFilter distance;
 Zstring k,v;
 Zint n;
 CSteamID lobby_id;
MANY(SteamLobbySearchCriteria,SteamLobbySearchCriteriaHandle,SteamLobbySearchCriteriaHandles,"SteamLobbySearchCriteria",SteamLobbySearchCriterias,{})
 Zint results;
 void CompatibleMembers( CSteamID lobby_id ) {
  SteamLobbySearchCriteria *c=new SteamLobbySearchCriteria;
  c->type=steam_CompatibleMembers;
  c->lobby_id=lobby_id;
  Append(c);
 }
 void KV( const char *k, const char *v, ELobbyComparison comparison=k_ELobbyComparisonEqual ) {
  SteamLobbySearchCriteria *c=new SteamLobbySearchCriteria;
  c->k=k;
  c->v=v;
  c->comparison=comparison;
  c->type=steam_LobbySearchString;
  Append(c);
 }
 void KV( const char *k, int v, ELobbyComparison comparison=k_ELobbyComparisonEqual ) {
  SteamLobbySearchCriteria *c=new SteamLobbySearchCriteria;
  c->k=k;
  c->n=v;
  c->comparison=comparison;
  c->type=steam_LobbySearchNumber;
  Append(c);
 }
 void Near( const char *k, int n ) {
  SteamLobbySearchCriteria *c=new SteamLobbySearchCriteria;
  c->k=k;
  c->n=n;
  c->type=steam_LobbySearchNear;
  Append(c);
 }
 void Distance( ELobbyDistanceFilter d=k_ELobbyDistanceFilterDefault ) {
  SteamLobbySearchCriteria *c=new SteamLobbySearchCriteria;
  c->type=steam_LobbySearchDistance;
  c->distance=d;
  Append(c);
 }
 void Slots( int n ) {
  SteamLobbySearchCriteria *c=new SteamLobbySearchCriteria;
  c->n=n;
  c->type=steam_LobbySearchSlots;
  Append(c);
 }
 // Called when the API needs to set criteria, these are cleared on each call to RequestLobbyList()
 void Execute() {
  FOREACH(SteamLobbySearchCriteria,s) {
   switch ( s->type ) {
    case steam_LobbySearchString:
      SteamMatchmaking()->AddRequestLobbyListStringFilter(s->k.c_str(),s->v.c_str(),s->comparison);
     break;
    case steam_LobbySearchNumber:	// numerical comparison
      SteamMatchmaking()->AddRequestLobbyListNumericalFilter(s->k.c_str(),s->n,s->comparison);
     break;
    case steam_LobbySearchNear: // results closest to the specified value, early filters taking precedence
      SteamMatchmaking()->AddRequestLobbyListNearValueFilter(s->k.c_str(),s->n);
     break;
    case steam_LobbySearchDistance:	// based on users IP address to location map on the Steam backed
      SteamMatchmaking()->AddRequestLobbyListDistanceFilter(s->distance);
     break;
    case steam_LobbySearchSlots:
      SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(s->n);
     break;
    case steam_CompatibleMembers:
      SteamMatchmaking()->AddRequestLobbyListCompatibleMembersFilter(s->lobby_id);
     break;
   }
  }
  if ( results > 0 ) {
   SteamMatchmaking()->AddRequestLobbyListResultCountFilter(results.value);
  }
 }
DONE(SteamLobbySearchCriteria);

C_ONE(SteamLobby,SteamNode,{type=ELobbyType::k_ELobbyTypeFriendsOnly;})
 Zint index;
 CSteamID lobby_id,owner;
 Zint dataCount,membershipCount;
 Zbool joinable;
 SteamProfiles members;
 Zint memberLimit;
 ELobbyType type;
 Strings kv;
 SteamLobbyChats chats;
 CSteamID server_id;
 uint32 ip;
 uint16 port;
 Zp<SteamLobby> child;
 bool SetMyData( const char *key, const char *value ) {
  STEAM_LOGIN_REQUIRED;
  SteamMatchmaking()->SetLobbyMemberData(lobby_id,key,value);
 }
 bool SendChat( const char *text ) {
  STEAM_LOGIN_REQUIRED;
  return SteamMatchmaking()->SendLobbyChatMsg(lobby_id,(const void*)text,strlen(text)+1);
 }
 bool Joinable( bool value ) {
  STEAM_LOGIN_REQUIRED;
  if ( SteamMatchmaking()->SetLobbyJoinable(lobby_id, value ) ) {
   this->joinable=value;
   return true;
  } else return false;
 }
 bool LeaveLobby() {
  STEAM_LOGIN_REQUIRED;
  SteamMatchmaking()->LeaveLobby(lobby_id);
  return true;
 }
 bool SetLobbyData(const char *key,const char *value) {
  STEAM_LOGIN_REQUIRED;
  return SteamMatchmaking()->SetLobbyData(lobby_id,key,value);
 }
 bool DeleteLobbyData( const char *key ) {
  STEAM_LOGIN_REQUIRED;
  return SteamMatchmaking()->DeleteLobbyData(lobby_id,key);
 }
 const char *GetLobbyData(const char *key) {
  STEAM_LOGIN_REQUIRED;
  return SteamMatchmaking()->GetLobbyData(lobby_id,key);
 }
 bool SetLobbyMemberLimit( int limit ) {
  STEAM_LOGIN_REQUIRED;
  return SteamMatchmaking()->SetLobbyMemberLimit(lobby_id,limit);
 }
 int GetLobbyMemberLimit() {
  STEAM_LOGIN_REQUIRED;
  return SteamMatchmaking()->GetLobbyMemberLimit(lobby_id);
 }
 bool SetLobbyType( ELobbyType eLobbyType ) {
  STEAM_LOGIN_REQUIRED;
  if ( SteamMatchmaking()->SetLobbyType(lobby_id, eLobbyType) ) {
   this->type=eLobbyType;
   return true;
  }
  return false;
 }
 bool GetLobbyOwner() {
  STEAM_LOGIN_REQUIRED;
  this->owner=SteamMatchmaking()->GetLobbyOwner(lobby_id);
  return true;
 }
 int GetLobbyDataCount() {
  STEAM_NOT(steam_GetLobbyList);
  return dataCount=SteamMatchmaking()->GetLobbyDataCount(lobby_id);
 }
 const char *GetLobbyMemberData( SteamProfile *user, const char *key ) {
  if ( !SteamUser()->BLoggedOn() || !user ) return "";
  return SteamMatchmaking()->GetLobbyMemberData(lobby_id,user->steam_id,key);
 }
 bool SendLobbyChat(const char *text) {
  STEAM_LOGIN_REQUIRED;
  return SteamMatchmaking()->SendLobbyChatMsg(lobby_id,(const void *)text,strlen(text)+1);
 }
 bool SetLobbyGameServer( uint32 ip, uint32 port ) {
  STEAM_LOGIN_REQUIRED;
  this->ip=ip;
  this->port=port;
  SteamMatchmaking()->SetLobbyGameServer(lobby_id,ip,port,server_id);
  return true;
 }
 bool GetLobbyGameServer() {
  STEAM_LOGIN_REQUIRED;
  return SteamMatchmaking()->GetLobbyGameServer(lobby_id,&ip,&port,&server_id);
 }
 bool SetLobbyOwner( SteamProfile *user ) {
  STEAM_LOGIN_REQUIRED;
  if ( SteamMatchmaking()->SetLobbyOwner( lobby_id,user->steam_id) ) {
   this->owner=user->steam_id;
   return true;
  }
  return false;
 }
 bool SetLinkedLobby( SteamLobby *child ) {
  STEAM_LOGIN_REQUIRED;
  if ( SteamMatchmaking()->SetLinkedLobby( lobby_id,child->lobby_id ) ) {
   this->child=child;
   return true;
  }
  return false;
 }
 bool InviteToLobby( SteamProfile *user ) {
  STEAM_LOGIN_REQUIRED;
  return SteamMatchmaking()->InviteUserToLobby(lobby_id,user->steam_id);
 }
 int GetCountLobbyMembers() {
  STEAM_LOGIN_REQUIRED;
  return membershipCount=SteamMatchmaking()->GetNumLobbyMembers(lobby_id);
 }
MANY(SteamLobby,SteamLobbyHandle,SteamLobbyHandles,"SteamLobby",SteamLobbies,{})
 SteamLobby *find( CSteamID lobby_id ) {
  FOREACH(SteamLobby,L) if ( L->lobby_id == lobby_id ) return L;
  return null;
 }
 bool Drop( CSteamID lobby_id ) {
  SteamLobby *L=find(lobby_id);
  bool result=false;
  if ( L ) {
   Remove(L);
   delete L;
   result=true;
  }
  return result;
 }
DONE(SteamLobby);

C_ONE(SteamLeaderboard,SteamNode,{})
MANY(SteamLeaderboard,SteamLeaderboardHandle,SteamLeaderboardHandles,"SteamLeaderboard",SteamLeaderboards,{})
void Define(const char *n) {
 FOREACH(SteamLeaderboard, sl) if (sl->name == n) return;
 SteamLeaderboard *sl = new SteamLeaderboard;
 sl->name = n;
 Append(sl);
}
DONE(SteamLeaderboard);

ONE(SteamAchievementDefinition,{})
MANY(SteamAchievementDefinition, SteamAchievementDefinitionHandle, SteamAchievementDefinitionHandles, "SteamAchievementDefinition", SteamAchievementDefinitions, { Init(); })
void Init() {
 Define("APOLUNE_1");
 Define("APOLUNE_2");
 Define("APOLUNE_3");
 Define("APOLUNE_4");
 Define("APOLUNE_5");
 Define("APOLUNE_6");
 Define("APOLUNE_7");
 Define("APOLUNE_8");
 Define("APOLUNE_9");
 Define("APOLUNE_10");
 Define("APOLUNE_11");
 Define("APOLUNE_12");
 Define("APOLUNE_13");
 Define("APOLUNE_14");
 Define("APOLUNE_15");
 Define("APOLUNE_16");
 Define("APOLUNE_17");
 Define("APOLUNE_18");
 Define("APOLUNE_19");
 Define("APOLUNE_20");
 Define("APOLUNE_21");
 Define("APOLUNE_22");
}
 void Define( const char *sw_id ) {
  SteamAchievementDefinition *sad=new SteamAchievementDefinition;
  sad->name=sw_id;
  Append(sad);
 }
DONE(SteamAchievementDefinition);
extern SteamAchievementDefinitions achievementDefinitions;

ONE(SteamAchievement,{})
 Zstring id;
 Zbool achieved;
 Zstring description;
 Zp<GLImage> image;
MANY(SteamAchievement,SteamAchievementHandle,SteamAchievementHandles,"SteamAchievement",SteamAchievements,{})
DONE(SteamAchievement);


ONE(SteamStatDefinition,{})
 Zstring displayName;
MANY(SteamStatDefinition,SteamStatDefinitionHandle,SteamStatDefinitionHandles,"SteamStatDefinition",SteamStatDefinitions,{})
 void Define( const char *sw_id, const char *displayName ) {
  SteamStatDefinition *ssd=new SteamStatDefinition;
  ssd->name=sw_id;
  ssd->displayName=displayName;
  Append(ssd);
 }
DONE(SteamStatDefinition);
extern SteamStatDefinitions statDefinitions;

ONE(SteamStat,{})
 Zstring id;
 Zfloat achieved;
MANY(SteamStat,SteamStatHandle,SteamStatHandles,"SteamStat",SteamStats,{})
DONE(SteamStat);

// Favorite Game
ONE(SteamFavoriteGame,{})
 AppId_t pnAppID;
 uint32 pnIP; // *pnIP, 
 uint16 pnConnPort; // *pnConnPort are filled in the with IP:port of the game server
 uint16 pnQueryPort; // *pnQueryport are filled in the with IP:port of the game server
 uint32 punFlags;	// *punFlags specify whether the game server was stored as an explicit favorite or in the history of connections
 uint32 pRTime32LastPlayedOnServer;	// *pRTime32LastPlayedOnServer is filled in the with the Unix time the favorite was added
 Zbool out;
 bool GetFavoriteGame( int index ) {
  return SteamMatchmaking()->GetFavoriteGame( index, &pnAppID, &pnIP, &pnConnPort, &pnQueryPort, &punFlags, &pRTime32LastPlayedOnServer );
 }
 int AddFavoriteGame() {	// adds the game server to the local list; updates the time played of the server if it already exists in the list
  return SteamMatchmaking()->AddFavoriteGame( pnAppID, pnIP, pnConnPort, pnQueryPort, punFlags, pRTime32LastPlayedOnServer );
 }
 bool RemoveFavoriteGame() {	// removes the game server from the local storage; returns true if one was removed
  return SteamMatchmaking()->RemoveFavoriteGame( pnAppID, pnIP, pnConnPort, pnQueryPort, punFlags );
 }
MANY(SteamFavoriteGame,SteamFavoriteGameHandle,SteamFavoriteGameHandles,"SteamFavoriteGame",SteamFavoriteGames,{})
 bool Get() {
  STEAM_LOGIN_REQUIRED;
  int total=SteamMatchmaking()->GetFavoriteGameCount();
  bool worked=true;
  Clear();
  for ( int i=0; i < total; i++ ) {
   SteamFavoriteGame *fav=new SteamFavoriteGame;
   worked = worked && fav->GetFavoriteGame(i);
   Append(fav);
  }
  return worked;
 }
DONE(SteamFavoriteGame);

class SteamServices;
class SteamServerListResponse : public ISteamMatchmakingServerListResponse {
public:
 Zbool active,LAN;
	int m_nServers;	// Track the number of servers we know about
	bool m_bRequestingServers;	// Track whether we are in the middle of a refresh or not
	HServerListRequest m_hServerListRequest;	// Track what server list request is currently running
 Zp<SteamServices> services; // List
	void ServerResponded( HServerListRequest hReq, int iServer );
	void ServerFailedToRespond( HServerListRequest hReq, int iServer );
	void RefreshComplete( HServerListRequest hReq, EMatchMakingServerResponse response );
};

class SteamService;
class SteamServerPingResponse : public ISteamMatchmakingPingResponse {
public:
 Zp<SteamService> service;
 void ServerResponded( gameserveritem_t &server );	// Server has responded successfully and has updated data
 void ServerFailedToRespond();	// Server failed to respond to the ping request
};

class SteamServerPlayersResponse : public ISteamMatchmakingPlayersResponse {
public:
 Zp<SteamService> service;
	void AddPlayerToList( const char *pchName, int nScore, float flTimePlayed );	// Got data on a new player on the server -- you'll get this callback once per player on the server which you have requested player data on.
	void PlayersFailedToRespond();	// The server failed to respond to the request for player details
	void PlayersRefreshComplete();	// The server has finished responding to the player details request (ie, you won't get anymore AddPlayerToList callbacks)
};

class SteamServerRulesResponse : public ISteamMatchmakingRulesResponse {
public:
 Zp<SteamService> service;
	void RulesResponded( const char *pchRule, const char *pchValue ); // Got data on a rule on the server -- you'll get one of these per rule defined on the server you are querying
	void RulesFailedToRespond(); // The server failed to respond to the request for rule details
	void RulesRefreshComplete();	// The server has finished responding to the rule details request (ie, you won't get anymore RulesResponded callbacks)
};

ONE(SteamServerPlayer,{})
 Zint playtime;
 Zfloat score;
MANY(SteamServerPlayer,SteamServerPlayerHandle,SteamServerPlayerHandles,"SteamServerPlayer",SteamServerPlayers,{})
 void Add( const char *name, float playtime, int score ) {
  SteamServerPlayer *p=new SteamServerPlayer;
  p->name=name;
  p->playtime=playtime;
  p->score=score;
  Append(p);
 }
DONE(SteamServerPlayer);

// Game server
ONE(SteamService,{
 m_unIPAddress=0;
 m_nConnectionPort=0;
 __ping.service=this;
 __players.service=this;
 __rules.service=this;
})
	CSteamID steam_id;
	uint32 m_unIPAddress;		   	// IP address for the server
	int32 m_nConnectionPort;		 // Port for game clients to connect to for this server
	Zint ping;				            	// current ping time in milliseconds
 Zstring ip,map,description,serverString;
	Zint humans;		             // current number of players on the server
	Zint max;		  	             // Maximum players that can join this server
	Zint bots;				             // Number of bots (i.e simulated players) on this server
	Zbool passwordRequired;				// true if this server needs a password to join
	Zbool VACSecured;		     			// Is this server protected by VAC
	Zint	version;	           		// server version as reported to Steam
 SteamServerPlayers players; // Lightweight player list
 Strings rules;              // Contains KeyValuePair ruleset for server
 HServerQuery _ping,_players,_rules;
 Zbool could_not_ping,could_not_players,could_not_rules,players_loading,rules_loading;
 SteamServerPingResponse __ping;
 SteamServerPlayersResponse __players;
 SteamServerRulesResponse __rules;
 void Ping() { _ping=SteamMatchmakingServers()->PingServer( m_unIPAddress, m_nConnectionPort, &__ping); }
 void CancelPing() { SteamMatchmakingServers()->CancelServerQuery(_ping); }
 void Players() { if ( players_loading ) return; players.Clear(); could_not_players=false; players_loading=true; _players=SteamMatchmakingServers()->PlayerDetails( m_unIPAddress, m_nConnectionPort, &__players); }
 void CancelPlayers() { SteamMatchmakingServers()->CancelServerQuery(_players); }
 void Rules() { if ( rules_loading ) return; rules.Clear(); could_not_rules=false; rules_loading=true; _rules=SteamMatchmakingServers()->ServerRules( m_unIPAddress, m_nConnectionPort, &__rules); }
 void CancelRules() { SteamMatchmakingServers()->CancelServerQuery(_rules); }
 void Set( gameserveritem_t &pGameServerItem ) {
 	char m_szMap[32];				// current map
 	char m_szGameDescription[64];	// game description
 	strncpy( m_szMap, pGameServerItem.m_szMap, ARRAYSIZE( m_szMap ) );
 	strncpy( m_szGameDescription, pGameServerItem.m_szGameDescription, ARRAYSIZE( m_szGameDescription ) );
  this->name=pGameServerItem.GetName();
  this->map=m_szMap;
  this->description=m_szGameDescription;
  this->serverString = FORMAT("%s (%i/%i) at %s ping(%d)",
   pGameServerItem.GetName(), pGameServerItem.m_nPlayers,
   pGameServerItem.m_nMaxPlayers, pGameServerItem.m_NetAdr.GetConnectionAddressString(),
   pGameServerItem.m_nPing
  );
 	this->m_unIPAddress = pGameServerItem.m_NetAdr.GetIP();
 	this->m_nConnectionPort = pGameServerItem.m_NetAdr.GetConnectionPort();
 	this->ping = pGameServerItem.m_nPing;
 	this->humans = pGameServerItem.m_nPlayers;
 	this->max = pGameServerItem.m_nMaxPlayers;
 	this->bots = pGameServerItem.m_nBotPlayers;
 	this->passwordRequired = pGameServerItem.m_bPassword;
 	this->VACSecured = pGameServerItem.m_bSecure;
 	this->version = pGameServerItem.m_nServerVersion;
 	this->steam_id = pGameServerItem.m_steamID;
 }
MANY(SteamService,SteamServiceHandle,SteamServiceHandles,"SteamService",SteamServices,{
 response.services=this;
 filters.Append(new KeyValuePair("gamedir",STEAM_APP_GAMEDIR)); // Filter entire steam server list by this game/app
// filters.Append(new KeyValuePair("secure","1")); // Look for secure servers only
// filters.Append(new KeyValuePair("gametype","coop")); // Look for games only of this type
})
 SteamServerListResponse response;
 Zbool loading; 
 Strings filters;
	HServerListRequest handle;	// Track what server list request is currently running
	bool Internet();	// Initiate a refresh of internet servers
 bool LAN();	// Initiate a refresh of LAN servers
 void Refresh() {
  if ( handle && !SteamMatchmakingServers()->IsRefreshing(handle) ) SteamMatchmakingServers()->RefreshQuery(handle);
 }
 bool Cancel() { // Cancels a previously executed request
  if ( handle ) { SteamMatchmakingServers()->ReleaseRequest(handle); handle=NULL; return true; }
  return false;
 }
DONE(SteamService);

// A game instance
ONE(SteamGame,{})
MANY(SteamGame,SteamGameHandle,SteamGameHandles,"SteamGame",SteamGames,{})
DONE(SteamGame);

// Network message types
#pragma pack( push, 1 )
enum EMessage
{
	// Server messages                                     	// Client messages
	k_EMsgServerBegin = 0,                                 	k_EMsgClientBegin = 500,
	k_EMsgServerSendInfo = k_EMsgServerBegin+1,            	k_EMsgClientInitiateConnection = k_EMsgClientBegin+1,
	k_EMsgServerFailAuthentication = k_EMsgServerBegin+2,  	k_EMsgClientBeginAuthentication = k_EMsgClientBegin+2,
	k_EMsgServerPassAuthentication = k_EMsgServerBegin+3,  	k_EMsgClientSendLocalUpdate = k_EMsgClientBegin+3,
	k_EMsgServerUpdateWorld = k_EMsgServerBegin+4,         	k_EMsgClientLeavingServer = k_EMsgClientBegin+4,
	k_EMsgServerExiting = k_EMsgServerBegin+5,             	k_EMsgClientPing = k_EMsgClientBegin+5,
	k_EMsgServerPingResponse = k_EMsgServerBegin+6,

	// P2P authentication messages
	k_EMsgP2PBegin = 600, 
	k_EMsgP2PSendingTicket = k_EMsgP2PBegin+1,

	// voice chat messages
	k_EMsgVoiceChatBegin = 700, 
	k_EMsgVoiceChatPing = k_EMsgVoiceChatBegin+1,	// just a keep alive message
	k_EMsgVoiceChatData = k_EMsgVoiceChatBegin+2,	// voice data from another player

	// force 32-bit size enum so the wire protocol doesn't get outgrown later
	k_EForceDWORD  = 0x7fffffff, 
};

// The Steamworks API's are modular, you can use some subsystems without using others
// When USE_GS_AUTH_API is defined you get the following Steam features:
// - Strong user authentication and authorization
// - Game server matchmaking
// - VAC cheat protection
// - Access to achievement/community API's
// - P2P networking capability
// Remove this define to disable using the native Steam authentication and matchmaking system
// You can use this as a sample of how to integrate your game without replacing an existing matchmaking system
// When you un-define USE_GS_AUTH_API you get:
// - Access to achievement/community API's
// - P2P networking capability
// You CANNOT use:
// - VAC cheat protection
// - Game server matchmaking
// as these function depend on using Steam authentication
#define USE_GS_AUTH_API 

// Stuff Valve needs for P2P network packets and PS3 cross-compatibility
#define LittleInt16( val )	( val )
#define LittleWord( val )	( val )
#define LittleInt32( val )	( val )
#define LittleDWord( val )	( val )
#define LittleQWord( val )	( val )
#define LittleFloat( val )	( val )

template <typename T> inline T WordSwap( T w ) {
	uint16 temp;
	temp  = ((*((uint16 *)&w) & 0xff00) >> 8);
	temp |= ((*((uint16 *)&w) & 0x00ff) << 8);
	return *((T*)&temp);
}

template <typename T> inline T DWordSwap( T dw ) {
	uint32 temp;
	temp  =   *((uint32 *)&dw) 				>> 24;
	temp |= ((*((uint32 *)&dw) & 0x00FF0000) >> 8);
	temp |= ((*((uint32 *)&dw) & 0x0000FF00) << 8);
	temp |= ((*((uint32 *)&dw) & 0x000000FF) << 24);
	return *((T*)&temp);
}

template <typename T> inline T QWordSwap( T dw ) {
	uint64 temp;
	temp  =   *((uint64 *)&dw) 				         >> 56;
	temp |= ((*((uint64 *)&dw) & 0x00FF000000000000ull) >> 40);
	temp |= ((*((uint64 *)&dw) & 0x0000FF0000000000ull) >> 24);
	temp |= ((*((uint64 *)&dw) & 0x000000FF00000000ull) >> 8);
	temp |= ((*((uint64 *)&dw) & 0x00000000FF000000ull) << 8);
	temp |= ((*((uint64 *)&dw) & 0x0000000000FF0000ull) << 24);
	temp |= ((*((uint64 *)&dw) & 0x000000000000FF00ull) << 40);
	temp |= ((*((uint64 *)&dw) & 0x00000000000000FFull) << 56);
	return *((T*)&temp);
}

// Msg from the server to the client which is sent right after communications are established
// and tells the client what SteamID the game server is using as well as whether the server is secure
struct SteamServerSendInfo_t {
	SteamServerSendInfo_t() : m_dwMessageType( LittleDWord( k_EMsgServerSendInfo ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }

	void SetSteamIDServer( uint64 SteamID ) { m_ulSteamIDServer = LittleQWord( SteamID ); }
	uint64 GetSteamIDServer() { return LittleQWord( m_ulSteamIDServer ); }

	void SetSecure( bool bSecure ) { m_bIsVACSecure = bSecure; }
	bool GetSecure() { return m_bIsVACSecure; }

	void SetServerName( const char *pchName ) { strncpy( m_rgchServerName, pchName, sizeof( m_rgchServerName ) ); }
	const char *GetServerName() { return m_rgchServerName; }

private:
	const DWORD m_dwMessageType;
	uint64 m_ulSteamIDServer;
	bool m_bIsVACSecure;
	char m_rgchServerName[128];
};

// Msg from the server to the client when refusing a connection
struct SteamServerFailAuthentication_t
{
	SteamServerFailAuthentication_t() : m_dwMessageType( LittleDWord( k_EMsgServerFailAuthentication ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
private:
	const DWORD m_dwMessageType;
};

// Msg from the server to client when accepting a pending connection
struct SteamServerPassAuthentication_t {
	SteamServerPassAuthentication_t() : m_dwMessageType( LittleDWord( k_EMsgServerPassAuthentication ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
	void SetPlayerPosition ( uint32 pos ) { m_uPlayerPosition = LittleDWord( pos ); }
	uint32 GetPlayerPosition() { return LittleDWord( m_uPlayerPosition ); }
private:
	const DWORD m_dwMessageType;
	uint32 m_uPlayerPosition;
};

// Msg from the server to clients when updating the world state
struct SteamServerUpdateWorld_t {
	SteamServerUpdateWorld_t() : m_dwMessageType( LittleDWord( k_EMsgServerUpdateWorld ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
//	ServerSpaceWarUpdateData_t *AccessUpdateData() { return &m_ServerUpdateData; }
private:
	const DWORD m_dwMessageType; //	ServerSpaceWarUpdateData_t m_ServerUpdateData;
};

// Msg from server to clients when it is exiting
struct SteamServerExiting_t {
	SteamServerExiting_t() : m_dwMessageType( LittleDWord( k_EMsgServerExiting ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
private:
	const DWORD m_dwMessageType;
};

// Msg from server to clients when it is exiting
struct SteamServerPingResponse_t {
	SteamServerPingResponse_t() : m_dwMessageType( LittleDWord( k_EMsgServerPingResponse ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
private:
	const DWORD m_dwMessageType;
};

// Msg from client to server when trying to connect
struct SteamClientInitiateConnection_t {
	SteamClientInitiateConnection_t() : m_dwMessageType( LittleDWord( k_EMsgClientInitiateConnection ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
private:
	const DWORD m_dwMessageType;
};

// Msg from client to server when initiating authentication
struct SteamClientBeginAuthentication_t {
	SteamClientBeginAuthentication_t() : m_dwMessageType( LittleDWord( k_EMsgClientBeginAuthentication ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
	uint32 GetTokenLen() { return LittleDWord( m_uTokenLen ); }
#ifdef USE_GS_AUTH_API
	void SetToken( const char *pchToken, uint32 unLen ) { m_uTokenLen = LittleDWord( unLen ); memcpy( m_rgchToken, pchToken, UMIN( unLen, sizeof( m_rgchToken ) ) ); }
	const char *GetTokenPtr() { return m_rgchToken; }
#endif
	void SetSteamID( uint64 ulSteamID ) { m_ulSteamID = LittleQWord( ulSteamID ); }
	uint64 GetSteamID() { return LittleQWord( m_ulSteamID ); }
private:
	const DWORD m_dwMessageType;	
	uint32 m_uTokenLen;
#ifdef USE_GS_AUTH_API
	char m_rgchToken[1024];
#endif
	uint64 m_ulSteamID;
};

// Msg from client to server when sending state update
struct SteamClientSendLocalUpdate_t {
	SteamClientSendLocalUpdate_t() : m_dwMessageType( LittleDWord( k_EMsgClientSendLocalUpdate ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
	 void SetShipPosition( uint32 uPos ) { m_uShipPosition = LittleDWord( uPos ); }
//	ClientSpaceWarUpdateData_t *AccessUpdateData() { return &m_ClientUpdateData; }
private:
	const DWORD m_dwMessageType;
	uint32 m_uShipPosition;
//	ClientSpaceWarUpdateData_t m_ClientUpdateData;
};

// Msg from the client telling the server it is about to leave
struct SteamClientLeavingServer_t {
	SteamClientLeavingServer_t() : m_dwMessageType( LittleDWord( k_EMsgClientLeavingServer ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
private:
	const DWORD m_dwMessageType;
};

// server ping
struct SteamClientPing_t {
	SteamClientPing_t() : m_dwMessageType( LittleDWord( k_EMsgClientPing ) ) {}
 DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
private:
	const DWORD m_dwMessageType;
};


class SteamP2PSendingTicket_t {
public:
	const DWORD m_dwMessageType;
	uint32 m_uTokenLen;
	char m_rgchToken[1024];
	uint64 m_ulSteamID;
	SteamP2PSendingTicket_t() : m_dwMessageType( LittleDWord( k_EMsgP2PSendingTicket ) ) {}
	DWORD GetMessageType() { return LittleDWord( m_dwMessageType ); }
	void SetToken( const char *pchToken, uint32 unLen ) { m_uTokenLen = LittleDWord( unLen ); memcpy( m_rgchToken, pchToken, UMIN( unLen, sizeof( m_rgchToken ) ) ); }
	uint32 GetTokenLen() { return LittleDWord( m_uTokenLen ); }
	const char *GetTokenPtr() { return m_rgchToken; }
	void SetSteamID( uint64 ulSteamID ) { m_ulSteamID = LittleQWord( ulSteamID ); }
	uint64 GetSteamID() { return LittleQWord( m_ulSteamID ); }
};
#pragma pack(pop)

// A peer-to-peer connection
C_ONE(SteamP2P,SteamNode,{
 connectTime=ticketTime=answerTime=0;
 m_cubTicketIGaveThisUser=0;
 m_cubTicketHeGaveMe=0;
})
 CSteamID steam_id;
 Zint port,timeout;
 Zbool relays;
	Zbool called,submitted,answered;
 uint64 connectTime,ticketTime,answerTime;
	uint32 m_cubTicketIGaveThisUser;
	uint8 m_rgubTicketIGaveThisUser[1024];
	uint32 m_cubTicketHeGaveMe;
	uint8 m_rgubTicketHeGaveMe[1024];
	HAuthTicket m_hAuthTicketIGaveThisUser;
	EBeginAuthSessionResult m_eBeginAuthSessionResult;
	EAuthSessionResponse m_eAuthSessionResponse;
 void Connect( CSteamID player, int port, int timeout, bool allowRelay ) {
  this->relays=allowRelay;
  this->timeout=timeout;
  this->port=port;
  steam_id=player;
  called=false;
  submitted=false;
  answered=false;
  connectTime=TICKSECS;
 	m_cubTicketIGaveThisUser = 0;
 	m_cubTicketHeGaveMe = 0;
  SteamNetworking()->CreateP2PConnectionSocket(player,port,timeout,allowRelay);
 }
 bool SendNetMessage( CSteamID steamIDTo, void *pMessage, int cubMessage ) {
 	return SteamNetworking()->SendP2PPacket( steamIDTo, pMessage, cubMessage, k_EP2PSendReliable );
 }
 bool SendTicket( CSteamID steamIDFrom, CSteamID steamIDTo, uint32 cubTicket, uint8 *pubTicket ) {
 	SteamP2PSendingTicket_t msg;
	 msg.SetToken( (char *)pubTicket, cubTicket );
	 msg.SetSteamID( steamIDFrom.ConvertToUint64() );
	 return SendNetMessage( steamIDTo, (void*)&msg, sizeof( msg ) );
 }
 void Authenticate() { // Initiate the Authentication Process
  // send the ticket if we havent yet
  if ( m_cubTicketIGaveThisUser == 0 ) {
  	m_hAuthTicketIGaveThisUser = SteamUser()->GetAuthSessionTicket( m_rgubTicketIGaveThisUser, sizeof( m_rgubTicketIGaveThisUser ), &m_cubTicketIGaveThisUser );
  }
  // sample has no retry here
  called = SendTicket( SteamUser()->GetSteamID(), steam_id, m_cubTicketIGaveThisUser, m_rgubTicketIGaveThisUser );
  // start a timer on this, if we dont get a ticket back within reasonable time, mark him timed out
  ticketTime = TICKSECS;
 }
 bool Authenticated() { // Purpose: is this auth ok?
 	if ( steam_id.IsValid() ) {
 		// Timeout if we fail to establish communication with this player
 		if ( !called && !submitted )	{
 			if ( TICKSECS - connectTime > 30  )	{
 				OUTPUT( "SteamP2P::Authenticated() Nothing received for account=%d\n", steam_id.GetAccountID() );
 				return false;
 			}
 		}
 		// first ticket check: if i submitted his ticket - was it good?
 		if ( submitted && m_eBeginAuthSessionResult != k_EBeginAuthSessionResultOK )	{
 			OUTPUT( "SteamP2P::Authenticated() Ticket from account=%d was bad\n", steam_id.GetAccountID() );
 			return false;
 		}
 		// second ticket check: if the steam backend replied, was that good?
 		if ( answered && m_eAuthSessionResponse != k_EAuthSessionResponseOK )	{
 			OUTPUT( "SteamP2P::Authenticated() Steam response for account=%d was bad\n", steam_id.GetAccountID() );
 			return false;
 		}
 		// last: if i sent him a ticket and he has not reciprocated, time out after 30 sec
 		if ( called && !submitted )	{
 			if ( TICKSECS - ticketTime > 30  )	{
 				OUTPUT( "SteamP2P::Authenticated() No ticket received for account=%d\n", steam_id.GetAccountID() );
 				return false;
 			}
 		}
 	}
 	return true;
 }
 void Disconnect() {
	 if ( called )	{
	 	SteamUser()->CancelAuthTicket( m_hAuthTicketIGaveThisUser );
	 	called = false;
	 }
	 if ( submitted )
	 {
	 	SteamUser()->EndAuthSession( steam_id );
	 	submitted = false;
	 }
	 if ( steam_id.IsValid() ) {
  	SteamNetworking()->CloseP2PSessionWithUser( steam_id );
  }
 }
MANY(SteamP2P,SteamP2PHandle,SteamP2PHandles,"SteamP2P",SteamP2Ps,{})
DONE(SteamP2P);

#pragma pack(push, 1)
struct SteamVoiceChatPing_t {
 SteamVoiceChatPing_t() : m_dwMessageType( LittleDWord( k_EMsgVoiceChatPing ) ) {}
	DWORD GetMessageType() const { return LittleDWord( m_dwMessageType ); }
private:
	const DWORD m_dwMessageType;
};

// voice chat data
struct SteamVoiceChatData_t {
	SteamVoiceChatData_t() : m_dwMessageType( LittleDWord( k_EMsgVoiceChatData ) ) {}
	DWORD GetMessageType() const { return LittleDWord( m_dwMessageType ); }
	void SetDataLength( uint32 unLength ) { m_uDataLength = LittleDWord( unLength ); }
	uint32 GetDataLength() const { return LittleDWord( m_uDataLength ); }
private:
	const DWORD m_dwMessageType;
	uint32 m_uDataLength;
};
#pragma pack(pop)

// Voice stream (voice chat)
C_ONE(SteamVoice,SteamNode,{})
MANY(SteamVoice,SteamVoiceHandle,SteamVoiceHandles,"SteamVoice",SteamVoices,{})
DONE(SteamVoice);

// A chat room ("clan chat")
C_ONE(SteamChatRoom,SteamNode,{})
MANY(SteamChatRoom,SteamChatRoomHandle,SteamChatRoomHandles,"SteamChatRoom",SteamChatRooms,{})
DONE(SteamChatRoom);

// A remote storage object (cloud service)
C_ONE(SteamStorage,SteamNode,{})
MANY(SteamStorage,SteamStorageHandle,SteamStorageHandles,"SteamStorage",SteamStorages,{})
DONE(SteamStorage);

// Steam workshop items
C_ONE(SteamWorkshopItem,SteamNode,{})
MANY(SteamWorkshopItem,SteamWorkshopItemHandle,SteamWorkshopItemHandles,"SteamStorage",SteamWorkshopItems,{})
DONE(SteamWorkshopItem);

// DLC package
C_ONE(SteamDLCDefinition,SteamNode,{dlc_id=0;})
 int64 dlc_id;
MANY(SteamDLCDefinition,SteamDLCDefinitionHandle,SteamDLCDefinitionHandles,"SteamDLCDefinition",SteamDLCDefinitions,{})
 SteamDLCDefinition *Define( const char *name, int64 dlc_id ) { 
  SteamDLCDefinition *def=new SteamDLCDefinition;
  def->name=name;
  def->dlc_id=dlc_id;
  Append(def);
 }
DONE(SteamDLCDefinition);

// DLC package
C_ONE(SteamDLC,SteamNode,{})
 Zbool installed;
MANY(SteamDLC,SteamDLCHandle,SteamDLCHandles,"SteamDLC",SteamDLCs,{})
DONE(SteamDLC);

class SteamRequestCallback {
public:
 SteamRequestCallback();
 SteamRequestType type;
 Zbool plurality; // When a request may be repeated once started.
 Zbool complete;
 Zint integer;
 Zbool boolean;
 Zstring string;
 Zp<SteamProfile> user;
 Zp<SteamProfiles> users;
 Zp<SteamLobby> lobby;
 Zp<SteamLobbies> lobbies;
 Zp<SteamService> service;
 Zp<SteamServices> services;
 Zp<SteamAchievement> achievement;
 Zp<SteamAchievements> achievements;
 Zp<SteamStat> stat;
 Zp<SteamStats> stats;
 Zp<SteamLeaderboard> leaderboard;
 Zp<SteamLeaderboards> leaderboards;
 Zp<SteamGame> game;
 Zp<SteamGames> games;
 Zp<SteamP2P> peer;
 Zp<SteamP2Ps> peers;
 Zp<SteamVoice> voice;
 Zp<SteamVoices> voices;
 Zp<SteamChatRoom> room;
 Zp<SteamChatRooms> rooms;
 Zp<SteamStorage> storage;
 Zp<SteamStorages> storages;
 Zp<SteamWorkshopItem> item;
 Zp<SteamWorkshopItems> items;
 Zp<SteamDLC> dlc;
 Zp<SteamDLCs> dlcs;
	STEAM_CB( OnAvatarImageLoaded,         AvatarImageLoaded_t             );
	STEAM_CB( OnGameJoinRequested,         GameRichPresenceJoinRequested_t );
	STEAM_CB( OnIPCFailure,                IPCFailure_t                    );
	STEAM_CB( OnSteamServersConnected,     SteamServersConnected_t         );
	STEAM_CB( OnSteamServersDisconnected,  SteamServersDisconnected_t      );
	STEAM_CB( OnSteamServerConnectFailure, SteamServerConnectFailure_t     );
	STEAM_CB( OnGameOverlayActivated,      GameOverlayActivated_t          );
	STEAM_CB( OnGameWebCallback,           GameWebCallback_t               );
	STEAM_CB( OnWorkshopItemInstalled,     ItemInstalled_t                 );
	STEAM_CB( OnSteamShutdown,             SteamShutdown_t                 );
	STEAM_CB( OnLobbyGameCreated,          LobbyGameCreated_t              );
 STEAM_CB( OnLobbyKicked,               LobbyKicked_t                   );
	STEAM_CBV( OnLobbyDataUpdate,          LobbyDataUpdate_t,              cb_LobbyDataUpdate );
	STEAM_CBV( OnLobbyChatUpdate,          LobbyChatUpdate_t,              cb_LobbyChatUpdate );
	STEAM_CBV( OnBeginAuthResponse,        ValidateAuthTicketResponse_t,   cb_BeginAuthResponse );
	STEAM_CBV( OnP2PSessionRequest,        P2PSessionRequest_t,            cb_P2PSessionRequest );
	STEAM_CBV( OnP2PSessionConnectFail,    P2PSessionConnectFail_t,        cb_P2PSessionConnectFail );
	STEAM_CBV( OnPersonaStateChange,       PersonaStateChange_t,           cb_PersonaStateChange );
	STEAM_CBV( OnSteamInventoryResult,     SteamInventoryResultReady_t,    cb_SteamInventoryResult );
	STEAM_CBV( OnSteamInventoryFullUpdate, SteamInventoryFullUpdate_t,     cb_SteamInventoryFullUpdate );
	STEAM_CBV( OnPlaybackStatusHasChanged, PlaybackStatusHasChanged_t,     cb_PlaybackStatusHasChanged );
	STEAM_CBV( OnVolumeChanged,            VolumeHasChanged_t,             cb_VolumeChanged );
	STEAM_CBV( OnUserStatsReceived,        UserStatsReceived_t,            cb_UserStatsReceived );
	STEAM_CBV( OnUserStatsStored,          UserStatsStored_t,              cb_UserStatsStored );
	STEAM_CBV( OnAchievementStored,        UserAchievementStored_t,        cb_AchievementStored );
	STEAM_CBV( OnCloseBrowser,             HTML_CloseBrowser_t,            cb_CloseBrowser );
	STEAM_CBV( OnNeedsPaint,               HTML_NeedsPaint_t,              cb_NeedsPaint );
	STEAM_CBV( OnStartRequest,             HTML_StartRequest_t,            cb_StartRequest );
	STEAM_CBV( OnFinishedRequest,          HTML_FinishedRequest_t,         cb_FinishedRequest );
 STEAM_CR(LeaderboardFindResult_t,cr_FindLeaderboard,OnFindLeaderboard); 
	STEAM_CR(LeaderboardScoreUploaded_t,cr_SetLeaderboard,OnSetLeaderboard);
	STEAM_CR(LeaderboardScoresDownloaded_t,cr_GetLeaderboard,OnGetLeaderboard); 
 STEAM_CR(FavoritesListChanged_t,cr_FavoritesListChanged,OnFavoritesListChanged);
 STEAM_CR(LobbyInvite_t,cr_LobbyInvite,OnLobbyInvite);
 STEAM_CR(LobbyEnter_t,cr_LobbyEnter,OnLobbyEnter);
 STEAM_CR(LobbyCreated_t,cr_LobbyCreated,OnLobbyCreated);
 STEAM_CR(LobbyChatMsg_t,cr_LobbyChatMsg,OnLobbyChatMsg);
 STEAM_CR(LobbyMatchList_t,cr_LobbyMatchList,OnLobbyMatchList);
 STEAM_CR(EncryptedAppTicketResponse_t,cr_EncryptedAppTicket,OnEncryptedAppTicket);
	STEAM_CR(JoinClanChatRoomCompletionResult_t,cr_JoinChatRoom,OnJoinChatRoom);
	STEAM_CR(HTML_BrowserReady_t,cr_BrowserReady,OnBrowserReady);
	STEAM_CR(SteamUGCRequestUGCDetailsResult_t,cr_WorkshopDetails,OnWorkshopDetails);
 bool GetUserStats() {
  if (!SteamUser()) return false;
  try {
   STEAM_NOT(steam_GetUserStats);
   STEAM_CMD(steam_GetUserStats);
  } catch (exception &e) { return false; }
  return SteamUserStats()->RequestCurrentStats();
 }
 bool GetAchievements() {
  if (!SteamUser()) return false;
  try {
   STEAM_NOT(steam_GetUserStats);
   STEAM_CMD(steam_GetUserStats);
  } catch (exception &e) { return false; }
  return SteamUserStats()->RequestCurrentStats();
 }
 bool SetAchievement( SteamAchievementDefinition *def ) {
  if (!SteamUser()) return false;
  try {
   STEAM_CMD(steam_SetUserStats);
   STEAM_NOT(steam_GetUserStats);
  } catch (exception &e) { return false; }
 	SteamUserStats()->SetAchievement(def->name.c_str());
		return SteamUserStats()->StoreStats();
 }
 bool GetStats() {
  if (!SteamUser()) return false;
  try {
   STEAM_NOT(steam_GetUserStats);
   STEAM_CMD(steam_GetUserStats);
  } catch (exception e) {}
  return SteamUserStats()->RequestCurrentStats();
 }
 bool SetStat( SteamStatDefinition *def, float value ) {
  if (!SteamUser()) return false;
  try {
   STEAM_NOT(steam_GetUserStats);
   STEAM_CMD(steam_SetUserStats);
  } catch (exception &e) { return false; }
 	SteamUserStats()->SetStat(def->name.c_str(),value);
		return SteamUserStats()->StoreStats();
 }
 bool FindLeaderboard( const char *ID ) {
  if (!SteamUser()) return false;
  try {
   STEAM_NOT(steam_FindLeaderboard);
   STEAM_CMD(steam_FindLeaderboard);
   STEAM_CR_SET(SteamUserStats()->FindLeaderboard(ID),cr_FindLeaderboard,OnFindLeaderboard);
  } catch (exception &e) { return false; }
 }
 bool GetLeaderboard( int offset, int limit ) {
  if (!SteamUser()) return false;
  try {
   STEAM_NOT(steam_GetLeaderboard);
   STEAM_CMD(steam_GetLeaderboard);
   STEAM_CR_SET(
    SteamUserStats()->DownloadLeaderboardEntries( leaderboard->id, k_ELeaderboardDataRequestGlobalAroundUser, offset, offset+limit ),
    cr_GetLeaderboard,OnGetLeaderboard
   );
  } catch (exception &e) { return false; }
 }
 bool SetLeaderboard( SteamLeaderboard *def, int score ) {
  if (!SteamUser()) return false;
  try {
   STEAM_CMD(steam_SetLeaderboard);
   STEAM_CR_SET(
    SteamUserStats()->UploadLeaderboardScore( def->id, k_ELeaderboardUploadScoreMethodKeepBest, score, NULL, 0 ),
    cr_SetLeaderboard,OnSetLeaderboard
   );
  }
  catch (exception &e) { return false; }
 }
 bool LobbyMatchList() {
  if (!SteamUser()) return false;
  try {
   STEAM_NOT(steam_GetLobbyList);
   STEAM_NOT(steam_GetLobbyData);
   STEAM_NOT(steam_SetLobbyGameServer);
   STEAM_NOT(steam_GetLobbyGameServer);
   STEAM_NOT(steam_SetLobbyMemberLimit);
   STEAM_NOT(steam_GetLobbyMemberLimit);
   STEAM_NOT(steam_SetLobbyType);
   STEAM_NOT(steam_SetLobbyJoinable);
   STEAM_NOT(steam_GetLobbyOwner);
   STEAM_NOT(steam_SetLobbyOwner);
   STEAM_NOT(steam_SetLinkedLobby);
   STEAM_CMD(steam_GetLobbyList);
   STEAM_CR_SET(SteamMatchmaking()->RequestLobbyList(),cr_LobbyMatchList,OnLobbyMatchList);
  } catch (exception &e) { return false; }
 }
 bool CreateLobby( ELobbyType eLobbyType, int membersMax ) {
  if (!SteamUser()) return false;
  try {
   integer=membersMax;
   STEAM_CMD(steam_CreateLobby);
   STEAM_CR_SET(
    SteamMatchmaking()->CreateLobby(eLobbyType,membersMax),
    cr_LobbyCreated,OnLobbyCreated
   );
  } catch (exception &e) { return false; }
 }
 bool JoinLobby() {
  if (!SteamUser()) return false;
  try {
   STEAM_NOT(steam_GetLobbyList);
   STEAM_CMD(steam_JoinLobby);
   STEAM_CR_SET(SteamMatchmaking()->JoinLobby(lobby->lobby_id),cr_LobbyEnter,OnLobbyEnter);
  } catch (exception &e) { return false; }
 }
 bool RequestLobbyData() {
  if (!SteamUser()) return false;
  try {
   STEAM_CMD(steam_RequestLobbyData);
   return SteamMatchmaking()->RequestLobbyData(lobby->lobby_id);
  } catch (exception &e) { return false; }
 }

/// bool AcceptP2P();
/// bool P2PPacketAvailable();
/// bool ReadP2P();
/// bool SendP2P();
/// bool CloseP2P();
/// 
/// // bool GetEncryptedAppTicket( AppTicket *out );
/// bool JoinClanChat();
/// bool GetWebPage();
/// bool WorkshopQuery();
/// bool InventoryUpdate();
/// bool ItemDrop();
/// bool ConsumeItems();
/// bool AddPromoItem();
/// bool AddPromoItems();
/// bool GrantPromoItems();
/// bool ExchangeItems();
};

ONE(SteamRequest,{}) 
 SteamRequest( SteamLobby *lobby ) : ListItem() { callback.lobby=lobby; }
 SteamRequest( SteamLobby *lobby, SteamProfile *member ) : ListItem() { callback.lobby=lobby; callback.user=member; }
 SteamRequest( SteamLobbies* get ) : ListItem() { callback.lobbies=get; }
 SteamRequest( SteamServices* get ) : ListItem() { callback.services=get; }
 SteamRequest( SteamStats* get ) : ListItem() { callback.stats=get; }
 SteamRequest( SteamAchievements* get ) : ListItem() { callback.achievements=get; }
 SteamRequest( SteamLeaderboard *get ) : ListItem() { callback.leaderboard=get; }
 SteamRequest( SteamLeaderboards* get ) : ListItem() { callback.leaderboards=get; }
 SteamRequest( SteamP2Ps* get ) : ListItem() { callback.peers=get; }
 SteamRequest( SteamVoices* get ) : ListItem() { callback.voices=get; }
 SteamRequest( SteamChatRooms* get ) : ListItem() { callback.rooms=get; }
 SteamRequest( SteamStorages* get ) : ListItem() { callback.storages=get; }
 SteamRequestCallback callback;
MANY(SteamRequest,SteamRequestHandle,SteamRequestHandles,"SteamRequest",SteamRequests,{})
 void Between() {
  SteamAPI_RunCallbacks();
  FOREACHN(SteamRequest,r,{
   if ( r->callback.complete ) {
    Remove(r);
    delete r;
   }
  });
 }
 int CountPending( SteamRequestType t ) {
  int total=0;
  FOREACH(SteamRequest,r) if ( !r->callback.complete && r->callback.type == t ) total++;
  return total;
 }
 SteamRequestCallback *Request(SteamRequest *r) { Append(r); return &r->callback; }
 bool GetAchievements( SteamAchievements *out ) { return Request(new SteamRequest(out))->GetAchievements(); }
 bool GetStats( SteamStats *out ) { return Request(new SteamRequest(out))->GetStats(); }
 bool SetAchievement( SteamAchievementDefinition *def ) { return Request(new SteamRequest)->SetAchievement(def); }
 bool SetStat( SteamStatDefinition *def, float value ) { return Request(new SteamRequest)->SetStat(def,value); }
// bool GetEncryptedAppTicket( AppTicket *out );
 bool FindLeaderboard( SteamLeaderboards *out, const char *ID ) { SteamLeaderboard *LB=out->firstNamed(ID); if ( LB ) return true; return Request(new SteamRequest(out))->FindLeaderboard(ID); }
 bool SetLeaderboard( SteamLeaderboard *def, int score ) { return Request(new SteamRequest)->SetLeaderboard(def,score); }
 bool GetLeaderboard( SteamLeaderboard *in, int offset, int limit ) { return Request(new SteamRequest(in))->GetLeaderboard(offset,limit); }
 bool GetLobbyList( SteamLobbySearchCriterias *search, SteamLobbies *out ) { search->Execute(); return Request(new SteamRequest(out))->LobbyMatchList(); }
 bool JoinLobby( SteamLobby *lobby ) { return Request(new SteamRequest(lobby))->JoinLobby(); }
 bool CreateLobby( SteamLobby *lobby,  ELobbyType eLobbyType, int membersMax ) { return Request(new SteamRequest(lobby))->CreateLobby(eLobbyType,membersMax); }
DONE(SteamRequest);

class SteamWorks {
public:
 Zuint lostPackets;
 SteamRequests requests;
 Zbool inited;
 Zbool error;
 Zbool online;
 SteamProfile self;
 SteamService service;
 ///
 SteamFavoriteGames favorites;
 SteamLobbies lobbies;
 SteamAchievements achievements;
 SteamStats stats;
 SteamLeaderboards leaderboards;
 SteamP2Ps peers;
 SteamVoices voices;
 SteamChatRooms rooms;
 SteamStorages storages;
 SteamLobbySearchCriterias lobbySearch;
 bool Init();
 void GoFriends() { if ( !SteamUtils()->IsOverlayEnabled() ) SteamFriends()->ActivateGameOverlay("Friends"); }
 void GoCommunity() { if ( !SteamUtils()->IsOverlayEnabled() ) SteamFriends()->ActivateGameOverlay("Community"); }
 void GoPlayers() { if ( !SteamUtils()->IsOverlayEnabled() ) SteamFriends()->ActivateGameOverlay("Players"); }
 void GoSettings() { if ( !SteamUtils()->IsOverlayEnabled() ) SteamFriends()->ActivateGameOverlay("Settings"); }
 void GoOfficialGameGroup() { if ( !SteamUtils()->IsOverlayEnabled() ) SteamFriends()->ActivateGameOverlay("OfficialGameGroup"); }
 void GoAchievements() { if ( !SteamUtils()->IsOverlayEnabled() ) SteamFriends()->ActivateGameOverlay("Achievements"); }
 void GoURL( const char *url ) { if ( !SteamUtils()->IsOverlayEnabled() ) SteamFriends()->ActivateGameOverlayToWebPage(url); }
 void GoStore( AppId_t app_id, EOverlayToStoreFlag e=EOverlayToStoreFlag::k_EOverlayToStoreFlag_None ) { if ( !SteamUtils()->IsOverlayEnabled() ) SteamFriends()->ActivateGameOverlayToStore(app_id,e); }
 void GoLobbyInvite() { if ( !SteamUtils()->IsOverlayEnabled() ) SteamFriends()->ActivateGameOverlay( "LobbyInvite" ); }
 void NotePosition( ENotificationPosition desired ) { SteamUtils()->SetOverlayNotificationPosition(desired); }
 void NoteInset( int x, int y ) { SteamUtils()->SetOverlayNotificationInset(x,y); } // Moves the notification in from the corner
 bool isOnline() { return (online=SteamUser()->BLoggedOn()); }
 bool GetAchievements() { return requests.GetAchievements(&achievements); }
 bool SetAchievement( const char *ID ) { SteamAchievementDefinition *sad=achievementDefinitions.firstNamed(ID); return sad ? requests.SetAchievement(sad) : false; }
 bool GetStats() { return requests.GetStats(&stats); }
 bool SetStats( const char *ID, float value ) { SteamStatDefinition *ssd=statDefinitions.firstNamed(ID); return ssd ? requests.SetStat(ssd,value) : false; }
 bool FindLeaderboard( const char *ID ) { return requests.FindLeaderboard(&leaderboards,ID); }
 bool GetLeaderboard( const char *ID, int offset, int limit ) { SteamLeaderboard *LB=leaderboards.firstNamed(ID); return LB ? requests.GetLeaderboard(LB,offset,limit) : false; }
 bool SetLeaderboard( const char *ID, int score ) { SteamLeaderboard *LB=leaderboards.firstNamed(ID); return LB ? requests.SetLeaderboard(LB,score) : false; }
 bool GetLobbyList() { return ((requests.CountPending(steam_GetLobbyList) == 0) ? requests.GetLobbyList(&lobbySearch,&lobbies) : false); }
 bool JoinLobby( SteamLobby *L ) { return requests.JoinLobby(L); }
 bool CreateLobby( SteamLobby *L, ELobbyType eLobbyType, int membersMax ) { return requests.CreateLobby(L,eLobbyType,membersMax); }
 void Between() {
  if ( inited ) {
   SteamAPI_RunCallbacks();
   requests.Between();
  }
 }
 void Shutdown() { if ( inited ) SteamAPI_Shutdown(); }
};

extern SteamWorks steamworks;

#endif