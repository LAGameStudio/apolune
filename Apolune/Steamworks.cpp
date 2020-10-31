#include "Steamworks.h"

#if defined(USE_STEAMWORKS)

SteamWorks steamworks;
SteamAchievementDefinitions achievementDefinitions;
SteamStatDefinitions statDefinitions;

int64 steam_app = MY_STEAM_APPID;

std::string SteamResultToString( EResult e ) {
 switch ( e ) {
 case	k_EResultOK: return string("k_EResultOK: success");
 case	k_EResultFail: return string("k_EResultFail: generic failure");
 case	k_EResultNoConnection: return string("k_EResultNoConnection: no/failed network connection");
// case	k_EResultNoConnectionRetry: return string("connection retry (OBSOLETE)");
 case	k_EResultInvalidPassword: return string("k_EResultInvalidPassword: password/ticket is invalid");
 case	k_EResultLoggedInElsewhere: return string("k_EResultLoggedInElsewhere: same user logged in elsewhere");
 case	k_EResultInvalidProtocolVer: return string("k_EResultInvalidProtocolVer: protocol version is incorrect");
 case	k_EResultInvalidParam: return string("k_EResultInvalidParam: a parameter is incorrect");
 case	k_EResultFileNotFound: return string("k_EResultFileNotFound: file was not found");
 case	k_EResultBusy: return string("k_EResultBusy: called method busy - action not taken");
 case	k_EResultInvalidState: return string("k_EResultInvalidState: called object was in an invalid state");
 case	k_EResultInvalidName: return string("k_EResultInvalidName: name is invalid");
 case	k_EResultInvalidEmail: return string("k_EResultInvalidEmail: email is invalid");
 case	k_EResultDuplicateName: return string("k_EResultDuplicateName: name is not unique");
 case	k_EResultAccessDenied: return string("k_EResultAccessDenied: access is denied");
 case	k_EResultTimeout: return string("k_EResultTimeout: operation timed out");
 case	k_EResultBanned: return string("k_EResultBanned: VAC2 banned");
 case	k_EResultAccountNotFound: return string("k_EResultAccountNotFound: account not found");
 case	k_EResultInvalidSteamID: return string("k_EResultInvalidSteamID: steamID is invalid");
 case	k_EResultServiceUnavailable: return string("k_EResultServiceUnavailable: The requested service is currently unavailable");
 case	k_EResultNotLoggedOn: return string("k_EResultNotLoggedOn: The user is not logged on");
 case	k_EResultPending: return string("k_EResultPending: Request is pending (may be in process, or waiting on third party)");
 case	k_EResultEncryptionFailure: return string("k_EResultEncryptionFailure: Encryption or Decryption failed");
 case	k_EResultInsufficientPrivilege: return string("k_EResultInsufficientPrivilege: Insufficient privilege");
 case	k_EResultLimitExceeded: return string("k_EResultLimitExceeded: Too much of a good thing");
 case	k_EResultRevoked: return string("k_EResultRevoked: Access has been revoked (used for revoked guest passes)");
 case	k_EResultExpired: return string("k_EResultExpired: License/Guest pass the user is trying to access is expired");
 case	k_EResultAlreadyRedeemed: return string("k_EResultAlreadyRedeemed: Guest pass has already been redeemed by account, cannot be acked again");
 case	k_EResultDuplicateRequest: return string("k_EResultDuplicateRequest: The request is a duplicate and the action has already occurred in the past, ignored this time");
 case	k_EResultAlreadyOwned: return string("k_EResultAlreadyOwned: All the games in this guest pass redemption request are already owned by the user");
 case	k_EResultIPNotFound: return string("k_EResultIPNotFound: IP address not found");
 case	k_EResultPersistFailed: return string("k_EResultPersistFailed: failed to write change to the data store");
 case	k_EResultLockingFailed: return string("k_EResultLockingFailed: failed to acquire access lock for this operation");
 case	k_EResultLogonSessionReplaced: return string("k_EResultLogonSessionReplaced: failed to acquire access lock for this operation");
 case	k_EResultConnectFailed: return string("k_EResultConnectFailed: connection failed");
 case	k_EResultHandshakeFailed: return string("k_EResultHandshakeFailed: handshake failed");
 case	k_EResultIOFailure: return string("k_EResultIOFailure: io failure");
 case	k_EResultRemoteDisconnect: return string("k_EResultRemoteDisconnect: remote disconnect");
 case	k_EResultShoppingCartNotFound: return string("k_EResultShoppingCartNotFound: failed to find the shopping cart requested");
 case	k_EResultBlocked: return string("k_EResultBlocked: a user didn't allow it");
 case	k_EResultIgnored: return string("k_EResultIgnored: target is ignoring sender");
 case	k_EResultNoMatch: return string("k_EResultNoMatch: nothing matching the request found");
 case	k_EResultAccountDisabled: return string("k_EResultAccountDisabled");
 case	k_EResultServiceReadOnly: return string("k_EResultServiceReadOnly");
 case	k_EResultAccountNotFeatured: return string("k_EResultAccountNotFeatured: account doesn't have value, so this feature isn't available");
 case	k_EResultAdministratorOK: return string("k_EResultAdministratorOK: allowed to take this action, but only because requester is admin");
 case	k_EResultContentVersion: return string("k_EResultContentVersion: A Version mismatch in content transmitted within the Steam protocol.");
 case	k_EResultTryAnotherCM: return string("k_EResultTryAnotherCM: The current CM can't service the user making a request, user should try another.");
 case	k_EResultPasswordRequiredToKickSession: return string("k_EResultPasswordRequiredToKickSession: You are already logged in elsewhere, this cached credential login has failed.");
 case	k_EResultAlreadyLoggedInElsewhere: return string("k_EResultAlreadyLoggedInElsewhere: You are already logged in elsewhere, you must wait");
 case	k_EResultSuspended: return string("k_EResultSuspended: Long running operation (content download) suspended/paused");
 case	k_EResultCancelled: return string("k_EResultCancelled: Operation canceled (typically by user: content download)");
 case	k_EResultDataCorruption: return string("k_EResultDataCorruption: Operation canceled because data is ill formed or unrecoverable");
 case	k_EResultDiskFull: return string("k_EResultDiskFull: Operation canceled - not enough disk space.");
 case	k_EResultRemoteCallFailed: return string("k_EResultRemoteCallFailed: a remote call or an IPC call failed");
 case	k_EResultPasswordUnset: return string("k_EResultPasswordUnset: Password could not be verified as it's unset server side");
 case	k_EResultExternalAccountUnlinked: return string("k_EResultExternalAccountUnlinked: External account (PSN, Facebook...) is not linked to a Steam account");
 case	k_EResultPSNTicketInvalid: return string("k_EResultPSNTicketInvalid: PSN ticket was invalid");
 case	k_EResultExternalAccountAlreadyLinked: return string("k_EResultExternalAccountAlreadyLinked: External account (PSN, Facebook...) is already linked to some other account, must explicitly request to replace/delete the link first");
 case	k_EResultRemoteFileConflict: return string("k_EResultRemoteFileConflict: The sync cannot resume due to a conflict between the local and remote files");
 case	k_EResultIllegalPassword: return string("k_EResultIllegalPassword: The requested new password is not legal");
 case	k_EResultSameAsPreviousValue: return string("k_EResultSameAsPreviousValue: new value is the same as the old one ( secret question and answer )");
 case	k_EResultAccountLogonDenied: return string("k_EResultAccountLogonDenied: account login denied due to 2nd factor authentication failure");
 case	k_EResultCannotUseOldPassword: return string("k_EResultCannotUseOldPassword: The requested new password is not legal");
 case	k_EResultInvalidLoginAuthCode: return string("k_EResultInvalidLoginAuthCode: account login denied due to auth code invalid");
 case	k_EResultAccountLogonDeniedNoMail: return string("k_EResultAccountLogonDeniedNoMail: account login denied due to 2nd factor auth failure - and no mail has been sent");
 case	k_EResultHardwareNotCapableOfIPT: return string("k_EResultHardwareNotCapableOfIPT");
 case	k_EResultIPTInitError: return string("k_EResultIPTInitError");
 case	k_EResultParentalControlRestricted: return string("k_EResultParentalControlRestricted: operation failed due to parental control restrictions for current user");
 case	k_EResultFacebookQueryError: return string("k_EResultFacebookQueryError: Facebook query returned an error");
 case	k_EResultExpiredLoginAuthCode: return string("k_EResultExpiredLoginAuthCode: account login denied due to auth code expired");
 case	k_EResultIPLoginRestrictionFailed: return string("k_EResultIPLoginRestrictionFailed");
 case	k_EResultAccountLockedDown: return string("k_EResultAccountLockedDown");
 case	k_EResultAccountLogonDeniedVerifiedEmailRequired: return string("k_EResultAccountLogonDeniedVerifiedEmailRequired");
 case	k_EResultNoMatchingURL: return string("k_EResultNoMatchingURL");
 case	k_EResultBadResponse: return string("k_EResultBadResponse: parse failure, missing field, etc.");
 case	k_EResultRequirePasswordReEntry: return string("k_EResultRequirePasswordReEntry: The user cannot complete the action until they re-enter their password");
 case	k_EResultValueOutOfRange: return string("k_EResultValueOutOfRange: the value entered is outside the acceptable range");
 case	k_EResultUnexpectedError: return string("k_EResultUnexpectedError: something happened that we didn't expect to ever happen");
 case	k_EResultDisabled: return string("k_EResultDisabled: The requested service has been configured to be unavailable");
 case	k_EResultInvalidCEGSubmission: return string("k_EResultInvalidCEGSubmission: The set of files submitted to the CEG server are not valid !");
 case	k_EResultRestrictedDevice: return string("k_EResultRestrictedDevice: The device being used is not allowed to perform this action");
 case	k_EResultRegionLocked: return string("k_EResultRegionLocked: The action could not be complete because it is region restricted");
 case	k_EResultRateLimitExceeded: return string("k_EResultRateLimitExceeded: Temporary rate limit exceeded, try again later, different from k_EResultLimitExceeded which may be permanent");
 case	k_EResultAccountLoginDeniedNeedTwoFactor: return string("k_EResultAccountLoginDeniedNeedTwoFactor: Need two-factor code to login");
 case	k_EResultItemDeleted: return string("k_EResultItemDeleted: The thing we're trying to access has been deleted");
 case	k_EResultAccountLoginDeniedThrottle: return string("k_EResultAccountLoginDeniedThrottle: login attempt failed, try to throttle response to possible attacker");
 case	k_EResultTwoFactorCodeMismatch: return string("k_EResultTwoFactorCodeMismatch: two factor code mismatch");
 case	k_EResultTwoFactorActivationCodeMismatch: return string("k_EResultTwoFactorActivationCodeMismatch: activation code for two-factor didn't match");
 case	k_EResultAccountAssociatedToMultiplePartners: return string("k_EResultAccountAssociatedToMultiplePartners: account has been associated with multiple partners");
 case	k_EResultNotModified: return string("k_EResultNotModified: data not modified");
 case	k_EResultNoMobileDevice: return string("k_EResultNoMobileDevice: the account does not have a mobile device associated with it");
 case	k_EResultTimeNotSynced: return string("k_EResultTimeNotSynced: the time presented is out of range or tolerance");
 case	k_EResultSmsCodeFailed: return string("k_EResultSmsCodeFailed: SMS code failure (no match, none pending, etc.)");
 case	k_EResultAccountLimitExceeded: return string("k_EResultAccountLimitExceeded: Too many accounts access this resource");
 case	k_EResultAccountActivityLimitExceeded: return string("k_EResultAccountActivityLimitExceeded: Too many changes to this account");
 case	k_EResultPhoneActivityLimitExceeded: return string("k_EResultPhoneActivityLimitExceeded: Too many changes to this phone");
 case	k_EResultRefundToWallet: return string("k_EResultRefundToWallet: Cannot refund to payment method, must use wallet");
 case	k_EResultEmailSendFailure: return string("k_EResultEmailSendFailure: Cannot send an email");
 case	k_EResultNotSettled: return string("k_EResultNotSettled: Can't perform operation till payment has settled");
 case	k_EResultNeedCaptcha: return string("k_EResultNeedCaptcha: Needs to provide a valid captcha");
 default: return FORMAT("Unknown error: %d",(int)e);
 }
}

//-----------------------------------------------------------------------------
// Purpose: Wrapper around SteamAPI_WriteMiniDump which can be used directly 
// as a se translator
//-----------------------------------------------------------------------------
#ifdef _WIN32
void MiniDumpFunction( unsigned int nExceptionCode, EXCEPTION_POINTERS *pException )
{
	// You can build and set an arbitrary comment to embed in the minidump here,
	// maybe you want to put what level the user was playing, how many players on the server,
	// how much memory is free, etc...
	SteamAPI_SetMiniDumpComment( MINIDUMP_CONTENT );

	// The 0 here is a build ID, we don't set it
	SteamAPI_WriteMiniDump( nExceptionCode, pException, 0 );
}
#endif

//-----------------------------------------------------------------------------
// Purpose: Helper to display critical errors
//-----------------------------------------------------------------------------
int Alert( const char *lpCaption, const char *lpText ) {
#ifndef _WIN32
    fprintf( stderr, "Message: '%s', Detail: '%s'\n", lpCaption, lpText );
	return 0;
#else
    return ::MessageBox( NULL, lpText, lpCaption, MB_OK | MB_TOPMOST | MB_SYSTEMMODAL | MB_SETFOREGROUND );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: callback hook for debug text emitted from the Steam API
//-----------------------------------------------------------------------------
extern "C" void __cdecl SteamAPIDebugTextHook( int nSeverity, const char *pchDebugText ) {
	// if you're running in the debugger, only warnings (nSeverity >= 1) will be sent
	// if you add -debug_steamapi to the command-line, a lot of extra informational messages will also be sent
	OUTPUT( pchDebugText );
	if ( nSeverity >= 1 )	{	// place to set a breakpoint for catching API errors
		int x = 3;
		x = x;
	}
}

/////////////////
// API Wrapper //
/////////////////

void SteamServerListResponse::ServerResponded( HServerListRequest hReq, int iServer ) {
 gameserveritem_t *pGameServerItem = SteamMatchmakingServers()->GetServerDetails( hReq, iServer );
 if ( pGameServerItem ) {
 	// Filter out servers that don't match our appid here (might get these in LAN calls since we can't put more filters on it)
 	if ( pGameServerItem->m_nAppID == SteamUtils()->GetAppID() )	{
  	char m_szMap[32];				// current map
  	char m_szGameDescription[64];	// game description
   strncpy( m_szMap, pGameServerItem->m_szMap, ARRAYSIZE( m_szMap ) );
   strncpy( m_szGameDescription, pGameServerItem->m_szGameDescription, ARRAYSIZE( m_szGameDescription ) );
   SteamService *s=new SteamService;
   s->m_unIPAddress = pGameServerItem->m_NetAdr.GetIP();
   s->m_nConnectionPort = pGameServerItem->m_NetAdr.GetConnectionPort();
   s->ping = pGameServerItem->m_nPing;
   s->serverString=FORMAT( "%s (%i/%i) at %s ping(%d)", pGameServerItem->GetName(), pGameServerItem->m_nPlayers, pGameServerItem->m_nMaxPlayers, pGameServerItem->m_NetAdr.GetConnectionAddressString(), pGameServerItem->m_nPing );
   s->ip=pGameServerItem->m_NetAdr.GetConnectionAddressString();
   s->map = m_szMap;
   s->name = pGameServerItem->GetName();
   s->description = m_szGameDescription;
   s->humans = pGameServerItem->m_nPlayers;
   s->max = pGameServerItem->m_nMaxPlayers;
   s->bots = pGameServerItem->m_nBotPlayers;
   s->passwordRequired = pGameServerItem->m_bPassword;
   s->VACSecured = pGameServerItem->m_bSecure;
   s->version = pGameServerItem->m_nServerVersion;	   
   s->steam_id = pGameServerItem->m_steamID;
   services->Append(s);
 	}
 }
}

void SteamServerListResponse::ServerFailedToRespond( HServerListRequest hReq, int iServer ) {
// Assert( hReq == m_hServerListRequest );
// bugbug jmccaskey - why would we ever need this?  Remove servers from our list I guess?
 services->loading=false;
}

void SteamServerListResponse::RefreshComplete( HServerListRequest hReq, EMatchMakingServerResponse response ) {
// Assert( hReq == m_hServerListRequest );
// Doesn't really matter to us whether the response tells us the refresh succeeded or failed,
// we just track whether we are done refreshing or not
 services->loading=false;
}

// Server has responded successfully and has updated data
void SteamServerPingResponse::ServerResponded( gameserveritem_t &server ) {
 service->Set(server);
 service->could_not_ping=false;
}
// Server failed to respond to the ping request
void SteamServerPingResponse::ServerFailedToRespond() {
 service->could_not_ping=true;
}	
 
// Got data on a new player on the server -- you'll get this callback once per player on the server which you have requested player data on.
void SteamServerPlayersResponse::AddPlayerToList( const char *pchName, int nScore, float flTimePlayed ) {
 service->players.Add(pchName,flTimePlayed,nScore);
 service->could_not_players=false;
}
// The server failed to respond to the request for player details
void SteamServerPlayersResponse::PlayersFailedToRespond() {
 service->could_not_players=true;
 service->players_loading=false;
}
// The server has finished responding to the player details request (ie, you won't get anymore AddPlayerToList callbacks)
void SteamServerPlayersResponse::PlayersRefreshComplete() {
 service->players_loading=false;
}

// Got data on a rule on the server -- you'll get one of these per rule defined on the server you are querying
void SteamServerRulesResponse::RulesResponded( const char *pchRule, const char *pchValue ) {
 service->rules.addKeyValue(pchRule,pchValue);
}
// The server failed to respond to the request for rule details
void SteamServerRulesResponse::RulesFailedToRespond() {
 service->could_not_rules=true;
 service->rules_loading=false;
}
// The server has finished responding to the rule details request (ie, you won't get anymore RulesResponded callbacks)
void SteamServerRulesResponse::RulesRefreshComplete() {
 service->rules_loading=false;
}

//-----------------------------------------------------------------------------
// Purpose: Initiate a refresh of internet servers
//-----------------------------------------------------------------------------
bool SteamServices::Internet() {
 STEAM_LOGIN_REQUIRED;
 if ( loading ) return false;	// If we are still finishing the previous refresh, then ignore this new request
 if ( handle )	{	// If another request is outstanding, make sure we release it properly
  SteamMatchmakingServers()->CancelQuery( handle );
 	SteamMatchmakingServers()->ReleaseRequest( handle );
 	handle = NULL;
 }
 OUTPUT( "SteamServices::Internet(): Refreshing internet servers\n" );
 // Track that we are now in a refresh, what type of refresh, and reset our server count
 loading = true;
 Clear();
 //	Steamworks_TestSecret();
 // Allocate some filters, there are some common pre-defined values that can be used:
 //
 // "gamedir" -- this is used to specify mods inside or a single product/appid
 // "secure" -- this is used to specify whether anti-cheat is enabled for a server
 // "gametype" -- this is used to specify game type and is set to whatever your game server code sets
 ZIndexed<MatchMakingKeyValuePair_t> pFilters;
 pFilters.Size((size_t)filters.count.value);
 unsigned int i=0;
 EACH(filters.first,KeyValuePair,s) {
  strncpy( pFilters.list[i].m_szKey, s->s.c_str(), sizeof(pFilters[i].m_szKey) );
  strncpy( pFilters.list[i].m_szValue, s->s.c_str(), sizeof(pFilters[i].m_szValue) );
  i++;
 }
 response.LAN=false;
	MatchMakingKeyValuePair_t *pFilter = pFilters.list;
	// bugbug jmccaskey - passing just the appid without filters results in getting all servers rather than
	// servers filtered by appid alone.  So, we'll use the filters to filter the results better.
	handle = SteamMatchmakingServers()->RequestInternetServerList( SteamUtils()->GetAppID(), &pFilter, pFilters.length, &response );
 return true;
}

//-----------------------------------------------------------------------------
// Purpose: Initiate a refresh of LAN servers
//-----------------------------------------------------------------------------
 bool SteamServices::LAN() {
  STEAM_LOGIN_REQUIRED;
 	if ( loading )	return false; 	// If we are still finishing the previous refresh, then ignore this new request
 	if ( handle )	{	// If another request is outstanding, make sure we release it properly
 		SteamMatchmakingServers()->ReleaseRequest( handle );
 		handle = NULL;
 	}
 	OUTPUT( "Refreshing LAN servers\n" );
 	loading=true;
  Clear();
  // LAN refresh doesn't accept filters like internet above does
  response.LAN=true;
 	handle = SteamMatchmakingServers()->RequestLANServerList( SteamUtils()->GetAppID(), &response );
  return true;
 }

int SteamPendingRequests( SteamRequestType t ) {
 return steamworks.requests.CountPending(t);
}

SteamRequestCallback::SteamRequestCallback() :
 cb_LobbyDataUpdate( this, &SteamRequestCallback::OnLobbyDataUpdate ),
 cb_LobbyChatUpdate( this, &SteamRequestCallback::OnLobbyChatUpdate ),
 cb_BeginAuthResponse( this, &SteamRequestCallback::OnBeginAuthResponse ),
 cb_P2PSessionRequest( this, &SteamRequestCallback::OnP2PSessionRequest ),
 cb_P2PSessionConnectFail( this, &SteamRequestCallback::OnP2PSessionConnectFail ),
 cb_PersonaStateChange( this, &SteamRequestCallback::OnPersonaStateChange ),                         
 cb_SteamInventoryResult( this, &SteamRequestCallback::OnSteamInventoryResult ),
 cb_SteamInventoryFullUpdate( this, &SteamRequestCallback::OnSteamInventoryFullUpdate ),
 cb_PlaybackStatusHasChanged( this, &SteamRequestCallback::OnPlaybackStatusHasChanged ),
 cb_VolumeChanged( this, &SteamRequestCallback::OnVolumeChanged ),
 cb_UserStatsReceived( this, &SteamRequestCallback::OnUserStatsReceived ),
 cb_UserStatsStored( this, &SteamRequestCallback::OnUserStatsStored ),
 cb_AchievementStored( this, &SteamRequestCallback::OnAchievementStored ),
 cb_CloseBrowser( this, &SteamRequestCallback::OnCloseBrowser ),
 cb_NeedsPaint( this, &SteamRequestCallback::OnNeedsPaint ),
 cb_StartRequest( this, &SteamRequestCallback::OnStartRequest ),
 cb_FinishedRequest( this, &SteamRequestCallback::OnFinishedRequest ) {
 type=steam_Anonymous;
}

STEAM_CB_FUN(OnAvatarImageLoaded,AvatarImageLoaded_t) {
 complete=true;
}

STEAM_CB_FUN(OnGameJoinRequested,GameRichPresenceJoinRequested_t) {
 complete=true;
}

STEAM_CB_FUN(OnIPCFailure,IPCFailure_t) {
 complete=true;
}

STEAM_CB_FUN(OnSteamServersConnected,SteamServersConnected_t) {
 complete=true;
}

STEAM_CB_FUN(OnSteamServersDisconnected,SteamServersDisconnected_t) {
 complete=true;
}

STEAM_CB_FUN(OnSteamServerConnectFailure,SteamServerConnectFailure_t) {
 complete=true;
}

STEAM_CB_FUN(OnGameOverlayActivated,GameOverlayActivated_t) {
 complete=true;
}

STEAM_CB_FUN(OnGameWebCallback,GameWebCallback_t) {
 complete=true;
}

STEAM_CB_FUN(OnWorkshopItemInstalled,ItemInstalled_t) {
 complete=true;
}

STEAM_CB_FUN(OnSteamShutdown,SteamShutdown_t) {
 complete=true;
}

STEAM_CB_FUN(OnLobbyGameCreated,LobbyGameCreated_t) {
 complete=true;
}

STEAM_CB_FUN(OnLobbyKicked,LobbyKicked_t) {
 steamworks.lobbies.Drop(result->m_ulSteamIDLobby);
 if ( !result->m_bKickedDueToDisconnect ) {
//  steamworks.console.Send(steam_KickedDueToDisconnect,result->m_ulSteamIDAdmin);
 }
 complete=true;
}

STEAM_CB_FUN(OnLobbyDataUpdate,LobbyDataUpdate_t) {
 complete=true;
}

STEAM_CB_FUN(OnLobbyChatUpdate,LobbyChatUpdate_t) {
 complete=true;
}

STEAM_CB_FUN(OnBeginAuthResponse,ValidateAuthTicketResponse_t) {
	if ( peer && peer->steam_id == result->m_SteamID )	{
		OUTPUT( "P2P:: Received steam response for account=%d\n", peer->steam_id.GetAccountID() );
  peer->answerTime=TICKSECS;
		peer->answered=true;
		peer->m_eAuthSessionResponse = result->m_eAuthSessionResponse;
	}
 complete=true;
}

STEAM_CB_FUN(OnP2PSessionRequest,P2PSessionRequest_t) {
	// always accept packets, the packet itself will come through when you call SteamNetworking()->ReadP2PPacket()
	SteamNetworking()->AcceptP2PSessionWithUser( result->m_steamIDRemote );
 complete=true;
}

STEAM_CB_FUN(OnP2PSessionConnectFail,P2PSessionConnectFail_t) {
	// we've sent a packet to the user, but it never got through
	// we can just use the normal timeout
 steamworks.lostPackets++;
 complete=true;
}

STEAM_CB_FUN(OnPersonaStateChange,PersonaStateChange_t) {
 complete=true;
}

STEAM_CB_FUN(OnSteamInventoryResult,SteamInventoryResultReady_t) {
 complete=true;
}

STEAM_CB_FUN(OnSteamInventoryFullUpdate,SteamInventoryFullUpdate_t) {
 complete=true;
}

STEAM_CB_FUN(OnPlaybackStatusHasChanged,PlaybackStatusHasChanged_t) {
 complete=true;
}

STEAM_CB_FUN(OnVolumeChanged,VolumeHasChanged_t) {
 complete=true;
}

STEAM_CB_FUN(OnUserStatsReceived,UserStatsReceived_t) {
 // we may get callbacks for other games' stats arriving, ignore them
 if ( steam_app == result->m_nGameID ) {
  if ( k_EResultOK == result->m_eResult ) {
   OUTPUT("Steamworks::OnUserStatsReceived: Received stats and achievements from Steam\n");
   if ( achievements ) { // Load achievements
    achievements->Clear();
    EACH(achievementDefinitions.first,SteamAchievementDefinition,ad) {
     SteamAchievement *a=new SteamAchievement;
     a->id=ad->name;
     SteamUserStats()->GetAchievement(ad->name.c_str(),&a->achieved.value);  
     a->name=SteamUserStats()->GetAchievementDisplayAttribute(ad->name.c_str(),"name");
     a->name=SteamUserStats()->GetAchievementDisplayAttribute(ad->name.c_str(),"desc");
     achievements->Append(a);
    }
    complete=true;
   } else if ( stats ) { // Load stats
    stats->Clear();
    EACH(statDefinitions.first,SteamStatDefinition,sd) {
     SteamStat *s=new SteamStat;
     s->id=sd->name;
     s->name=sd->displayName;
     SteamUserStats()->GetStat(sd->name.c_str(),&s->achieved.value);  
     stats->Append(s);
    }
    complete=true;
   } else STEAM_NOWHERE_TO_GO("OnUserStatsReceived","stats");
  } else {
   complete=true;
   STEAM_ERROR("OnUserStatsReceived");
   OUTPUT("Steamworks::OnUserStatsReceived: failed, %d\n", (int) result->m_eResult);
  }
 }
}

STEAM_CB_FUN(OnUserStatsStored,UserStatsStored_t) {
 // we may get callbacks for other games' stats arriving, ignore them
 if ( steam_app == result->m_nGameID ) {
   if ( k_EResultOK == result->m_eResult ) {
    OUTPUT( "Steamworks::StoreUserStats: success\n" );
    complete=true;
   } else if ( k_EResultInvalidParam == result->m_eResult ) {
     // One or more stats we set broke a constraint. They've been reverted,
     // and we should re-iterate the values now to keep in sync.
     OUTPUT( "Steamworks::StoreUserStats: some failed to validate\n" );
     // Fake up a callback here so that we re-load the values.
     UserStatsReceived_t callback;
     callback.m_eResult = k_EResultOK;
     callback.m_nGameID = steam_app;
     OnUserStatsReceived( &callback );
    complete=true;
   } else {
   OUTPUT( "Steamworks::StoreUserStats: failed, %d\n", result->m_eResult );
   complete=true;
  }
 }
}

STEAM_CB_FUN(OnAchievementStored,UserAchievementStored_t) {
 complete=true;
}

STEAM_CB_FUN(OnCloseBrowser,HTML_CloseBrowser_t) {
 complete=true;
}

STEAM_CB_FUN(OnNeedsPaint,HTML_NeedsPaint_t) {
 complete=true;
}

STEAM_CB_FUN(OnStartRequest,HTML_StartRequest_t) {
 complete=true;
}

STEAM_CB_FUN(OnFinishedRequest,HTML_FinishedRequest_t) {
 complete=true;
}

STEAM_CR_FUN(OnFindLeaderboard,LeaderboardFindResult_t) {
 if ( !result->m_bLeaderboardFound || io_error || result->m_hSteamLeaderboard == 0 )	{	// see if we encountered an error during the call
		OUTPUT( "SteamRequestCallback::OnFindLeaderboard: Leaderboard requested could not be found\n" );
	}
 if ( leaderboards ) { // Store locally
  SteamLeaderboard *LB=new SteamLeaderboard;
  LB->id=(int64) result->m_hSteamLeaderboard;
  leaderboards->Append(LB);
 }
 complete=true;
}

STEAM_CR_FUN(OnSetLeaderboard,LeaderboardScoreUploaded_t) {
 complete=true;
}

STEAM_CR_FUN(OnGetLeaderboard,LeaderboardScoresDownloaded_t) {
 complete=true;
}

STEAM_CR_FUN(OnLobbyMatchList,LobbyMatchList_t) {
 if ( io_error ) return;
 if ( lobbies ) {
  lobbies->Clear();
  // lobbies are returned in order of closeness to the user, so add them to the list in that order
 	for ( uint32 iLobby = 0; iLobby < result->m_nLobbiesMatching; iLobby++ )	{
   SteamLobby *L=new SteamLobby;
 		L->lobby_id = SteamMatchmaking()->GetLobbyByIndex( iLobby );
   L->index = (int) iLobby;
   const char *pchLobbyName = SteamMatchmaking()->GetLobbyData( L->lobby_id, "name" );
  	if ( pchLobbyName && pchLobbyName[0] ) L->name=pchLobbyName;
   else {
 			SteamMatchmaking()->RequestLobbyData( L->lobby_id );
 			L->name = FORMAT("Lobby %d", L->lobby_id.GetAccountID() );
   }
   lobbies->Append(L);
  }
	}
 complete=true;
}

STEAM_CR_FUN(OnLobbyCreated,LobbyCreated_t) {
 if ( io_error || result->m_eResult != k_EResultOK ) {
  STEAM_ERROR("OnLobbyCreated");
  complete=true;
  return;
 }
 if ( lobbies ) {
  lobby=new SteamLobby;
  lobby->memberLimit=integer;
  lobby->lobby_id=result->m_ulSteamIDLobby;
  lobbies->Append(lobby);
 } else STEAM_NOWHERE_TO_GO("OnLobbyCreated","lobbies");
 complete=true;
}

STEAM_CR_FUN(OnEncryptedAppTicket,EncryptedAppTicketResponse_t) {
 complete=true;
}

STEAM_CR_FUN(OnJoinChatRoom,JoinClanChatRoomCompletionResult_t) {
 complete=true;
}

STEAM_CR_FUN(OnBrowserReady,HTML_BrowserReady_t) {
 complete=true;
}

STEAM_CR_FUN(OnWorkshopDetails,SteamUGCRequestUGCDetailsResult_t) {
 complete=true;
}



bool SteamWorks::Init() {
 /*
  * This is in a !DEBUG block because it will cause the "installed" version in the "Steam client" to run instead
  * of what you want (the debugger executable).
  */
#if !defined(DEBUG)

 if ( SteamAPI_RestartAppIfNecessary( MY_STEAM_APPID ) ) {
 	// if Steam is not running or the game wasn't started through Steam, SteamAPI_RestartAppIfNecessary starts the 
 	// local Steam client and also launches this game again.
 	// Once you get a public Steam AppID assigned for this game, you need to replace k_uAppIdInvalid with it and
 	// removed steam_appid.txt from the game depot.
 	return false;
 }
 /*
 // Init Steam CEG
 if ( !Steamworks_InitCEGLibrary() ) {
  OUTPUT( "Steamworks_InitCEGLibrary() failed\n" );
 	Alert( "Fatal Error", "Steam must be running to play this game (InitDrmLibrary() failed).\n" );
 	return false;
 }
 */
	// Initialize SteamAPI, if this fails we bail out since we depend on Steam for lots of stuff.
	// You don't necessarily have to though if you write your code to check whether all the Steam
	// interfaces are NULL before using them and provide alternate paths when they are unavailable.
	// This will also load the in-game steam overlay dll into your process.  That dll is normally
	// injected by steam when it launches games, but by calling this you cause it to always load,
	// even when not launched via steam.
	if ( !SteamAPI_Init() )
	{
		OUTPUT( "SteamAPI_Init() failed\n" );
		Alert( "Fatal Error", "Steam must be running to play this game (SteamAPI_Init() failed).\n" );
		return false;
	}
 // set our debug handler
	SteamClient()->SetWarningMessageHook( &SteamAPIDebugTextHook );

 // get our real app id
 steam_app=SteamUtils()->GetAppID();


 leaderboards.Define("Apolune High Scores");
 leaderboards.Define("Apolune Kill Counts");
 leaderboards.Define("Apolune 2 Player High Scores");
 leaderboards.Define("Apolune 2 Player Kill Counts");
 // then try it this way:
 FindLeaderboard("Apolune High Scores");
 FindLeaderboard("Apolune Kill Counts");
 FindLeaderboard("Apolune 2 Player High Scores");
 FindLeaderboard("Apolune 2 Player Kill Counts");
#endif



 // success
 return (inited=true).value;
}

#endif