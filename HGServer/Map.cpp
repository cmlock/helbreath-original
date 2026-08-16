// Map.cpp: implementation of the CMap class.
//
//////////////////////////////////////////////////////////////////////

#include "Map.h"

extern void PutLogFileList(char * cStr);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMap::CMap(class CGame * pGame)
: m_bIsSnowEnabled(FALSE)
{
 int i, ix, iy;

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		m_pTeleportLoc[i] = NULL;

	for (i = 0; i < DEF_MAXWAYPOINTCFG; i++) {
		m_WaypointList[i].x = -1;
		m_WaypointList[i].y = -1;
	}

	for (i = 0; i < DEF_MAXMGAR; i++) {
		m_rcMobGenAvoidRect[i].top  = -1;
		m_rcMobGenAvoidRect[i].left = -1;
	}

	for (i = 0; i < DEF_MAXNMR; i++) {
		m_rcNoAttackRect[i].top  = -1;
		m_rcNoAttackRect[i].left = -1;
	}

	for (i = 0; i < DEF_MAXSPOTMOBGENERATOR; i++) {
		m_stSpotMobGenerator[i].bDefined = FALSE;
		m_stSpotMobGenerator[i].iTotalActiveMob = 0;
	}

	for (i = 0; i < DEF_MAXFISHPOINT; i++) {
		m_FishPointList[i].x = -1;
		m_FishPointList[i].y = -1;
	}

	for (i = 0; i < DEF_MAXMINERALPOINT; i++) {
		m_MineralPointList[i].x = -1;
		m_MineralPointList[i].y = -1;
	}

	for (i = 0; i < DEF_MAXINITIALPOINT; i++) {
		m_pInitialPoint[i].x = -1;
		m_pInitialPoint[i].y = -1;
	}
 
	for (i = 0; i < 1000; i++)
		m_bNamingValueUsingStatus[i] = FALSE;

	for (i = 0; i < DEF_MAXOCCUPYFLAG; i++)
		m_pOccupyFlag[i] = NULL;

	for (i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
		m_pStrategicPointList[i] = NULL;

	for (i = 0; i < DEF_MAXENERGYSPHERES; i++) {
		m_stEnergySphereCreationList[i].cType = NULL;
		m_stEnergySphereGoalList[i].cResult   = NULL;
	}

	ZeroMemory(m_stItemEventList, sizeof(m_stItemEventList));

	m_bIsHeldenianMap = FALSE;
	m_iTotalActiveObject = 0;
	m_iTotalAliveObject  = 0;
	m_iTotalItemEvents = 0;
	sMobEventAmount = 15;
	//m_sInitialPointX = 0;
	//m_sInitialPointY = 0;

	m_bIsFixedDayMode = FALSE;

	m_iTotalFishPoint = 0;
	m_iMaxFish = 0;
	m_iCurFish = 0;

	m_iTotalMineralPoint = 0;
	m_iMaxMineral = 0;
	m_iCurMineral = 0;

	m_pTile = NULL;

	m_cWhetherStatus = NULL;
	m_cType          = DEF_MAPTYPE_NORMAL;

	m_pGame = pGame;

	m_iLevelLimit = 0;
	m_iUpperLevelLimit = 0; // v1.4
	m_bMineralGenerator = FALSE;

	m_iTotalOccupyFlags = 0;
	
	m_bIsAttackEnabled = TRUE;
	m_cRandomMobGeneratorLevel = 0;

	m_bIsFightZone = FALSE;
	
	m_iTotalEnergySphereCreationPoint = 0;
	m_iTotalEnergySphereGoalPoint = 0;

	m_bIsEnergySphereGoalEnabled = FALSE;
	m_iCurEnergySphereGoalPointIndex = -1;

	for (ix = 0; ix < DEF_MAXSECTORS; ix++)
	for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
		m_stSectorInfo[ix][iy].iNeutralActivity = 0;
		m_stSectorInfo[ix][iy].iAresdenActivity = 0;
		m_stSectorInfo[ix][iy].iElvineActivity  = 0;
		m_stSectorInfo[ix][iy].iMonsterActivity = 0;
		m_stSectorInfo[ix][iy].iPlayerActivity  = 0;

		m_stTempSectorInfo[ix][iy].iNeutralActivity = 0;
		m_stTempSectorInfo[ix][iy].iAresdenActivity = 0;
		m_stTempSectorInfo[ix][iy].iElvineActivity  = 0;
		m_stTempSectorInfo[ix][iy].iMonsterActivity = 0;
		m_stTempSectorInfo[ix][iy].iPlayerActivity  = 0;
	} 
	
	m_iMaxNx = m_iMaxNy = m_iMaxAx = m_iMaxAy = m_iMaxEx = m_iMaxEy = m_iMaxMx = m_iMaxMy = m_iMaxPx = m_iMaxPy = 0;

	for (i = 0; i < DEF_MAXHELDENIANDOOR; i++) {
		m_stHeldenianGateDoor[i].cDir = 0;
		m_stHeldenianGateDoor[i].dX = 0;
		m_stHeldenianGateDoor[i].dY = 0;
	}

	for (i = 0; i < DEF_MAXHELDENIANTOWER; i++) {
		m_stHeldenianTower[i].sTypeID = 0;
		m_stHeldenianTower[i].dX = 0;
		m_stHeldenianTower[i].dY = 0;
		m_stHeldenianTower[i].cSide = 0;
	}
	
	for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) {
		m_stStrikePoint[i].dX  = 0;
		m_stStrikePoint[i].dY  = 0;
		m_stStrikePoint[i].iHP = 0;
		m_stStrikePoint[i].iMapIndex = -1;
		ZeroMemory(m_stStrikePoint[i].cRelatedMapName, sizeof(m_stStrikePoint[i].cRelatedMapName));
	}
	m_iTotalStrikePoints = 0;
	m_bIsDisabled = FALSE;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
	}
	m_iTotalCrusadeStructures = 0;
	m_iTotalAgriculture = 0;
}

CMap::~CMap()
{
	
 int i;
	
	if (m_pTile != NULL) 
		delete []m_pTile;	
 	
	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		if (m_pTeleportLoc[i] != NULL) delete m_pTeleportLoc[i];

	for (i = 0; i < DEF_MAXOCCUPYFLAG; i++)
		if (m_pOccupyFlag[i] != NULL) delete m_pOccupyFlag[i];

	for (i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
		if (m_pStrategicPointList[i] != NULL) delete m_pStrategicPointList[i];
}

void CMap::SetOwner(short sOwner, char cOwnerClass, short sX, short sY)
{
 class CTile * pTile;	
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;
	
	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->m_sOwner      = sOwner;
	pTile->m_cOwnerClass = cOwnerClass;
}

char _tmp_cMoveDirX[9] = { 0,0,1,1,1,0,-1,-1,-1 };
char _tmp_cMoveDirY[9] = { 0,-1,-1,0,1,1,1,0,-1 };
BOOL CMap::bCheckFlySpaceAvailable(short sX, char sY, char cDir, short sOwner)
{
 class CTile * pTile;
 short dX, dY;

	if ((cDir <= 0) || (cDir > 8)) return 0;
	dX = _tmp_cMoveDirX[cDir] + sX;
	dY = _tmp_cMoveDirY[cDir] + sY;
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return 0;
	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	if (pTile->m_sOwner != NULL) return 0;
	pTile->m_sOwner = sOwner;
	return 1;
}

void CMap::SetDeadOwner(short sOwner, char cOwnerClass, short sX, short sY)
{
 class CTile * pTile;	
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;
 
	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->m_sDeadOwner      = sOwner;
	pTile->m_cDeadOwnerClass = cOwnerClass;
}


/*********************************************************************************************************************
**  void CMap::GetOwner(short * pOwner, char * pOwnerClass, short sX, short sY)										**
**  description			:: check if the tile contains a player														**
**  last updated		:: November 17, 2004; 10:48 PM; Hypnotoad													**
**	return value		:: void																						**
**  commentary			::	-	added check to see if owner is class 1 or if is greater than max clients 			**
**********************************************************************************************************************/
void CMap::GetOwner(short * pOwner, char * pOwnerClass, short sX, short sY)
{
 class CTile * pTile;	

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) {
		*pOwner      = NULL;	
		*pOwnerClass = NULL;
		return;	
	}

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	*pOwner      = pTile->m_sOwner;
	*pOwnerClass = pTile->m_cOwnerClass;
	
	if ((*pOwnerClass == 1) && (*pOwner > DEF_MAXCLIENTS)) {
		*pOwner      = NULL;	
		*pOwnerClass = NULL;
		return;	
	}
	
	if (pTile->m_sOwner == 0) *pOwnerClass = 0;
}

/*********************************************************************************************************************
**  void CMap::GetDeadOwner(short * pOwner, char * pOwnerClass, short sX, short sY)									**
**  description			:: check if the tile contains a dead player													**
**  last updated		:: November 20, 2004; 9:13 PM; Hypnotoad													**
**	return value		:: void																						**
**********************************************************************************************************************/
void CMap::GetDeadOwner(short * pOwner, char * pOwnerClass, short sX, short sY)
{
 class CTile * pTile;	

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) {
		*pOwner      = NULL;	
		*pOwnerClass = NULL;
		return;	
	}

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	*pOwner      = pTile->m_sDeadOwner;
	*pOwnerClass = pTile->m_cDeadOwnerClass;
}

 								  
BOOL CMap::bGetMoveable(short dX, short dY, short * pDOtype, short * pTopItem)
{
 class CTile * pTile;	
	
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return FALSE;
	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if (pDOtype != NULL) *pDOtype = pTile->m_sDynamicObjectType;
	if (pTopItem != NULL) *pTopItem = pTile->m_cTotalItem;

	if (pTile->m_sOwner != NULL) return FALSE;
	if (pTile->m_bIsMoveAllowed == FALSE) return FALSE;
	if (pTile->m_bIsTempMoveAllowed == FALSE) return FALSE;
	
	return TRUE;
}

// 8-direction compass codes, matching the convention used by Game.cpp's _tmp_cTmpDirX/Y
// (1=N, 2=NE, 3=E, 4=SE, 5=S, 6=SW, 7=W, 8=NW). Duplicated locally rather than shared
// because Game.cpp's table is a file-local global, not exposed via a header.
static char  s_cPathDirX[9]    = {  0,  0, 1, 1, 1, 0,-1,-1,-1 };
static char  s_cPathDirY[9]    = {  0, -1,-1, 0, 1, 1, 1, 0,-1 };
static short s_cPathDirCost[9] = {  0, 10,14,10,14,10,14,10,14 }; // orthogonal=10, diagonal=14 (~10*sqrt2)

BOOL CMap::bGetMoveableDir(short sX, short sY, char cDir)
{
 char cFlankA, cFlankB;
 short dX, dY;

	dX = sX + s_cPathDirX[cDir];
	dY = sY + s_cPathDirY[cDir];

	if (bGetMoveable(dX, dY) == FALSE) return FALSE;

	if ((cDir % 2) == 0) {
		// Diagonal step: also require both tiles flanking the corner to be open, otherwise
		// the mover is allowed to clip through a solid wall corner between two blocked tiles.
		cFlankA = cDir - 1; if (cFlankA < 1) cFlankA += 8;
		cFlankB = cDir + 1; if (cFlankB > 8) cFlankB -= 8;

		if (bGetMoveable(sX + s_cPathDirX[cFlankA], sY + s_cPathDirY[cFlankA]) == FALSE) return FALSE;
		if (bGetMoveable(sX + s_cPathDirX[cFlankB], sY + s_cPathDirY[cFlankB]) == FALSE) return FALSE;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------------------
// Bounded-radius A*, used for NPC target chase/flee only (short, obstacle-heavy routes
// where the old greedy "step toward target, sidestep if blocked" logic gets stuck on
// concave obstacles). Waypoint wandering keeps using the simpler stepper - those routes
// are long and mostly open, so the greedy approach is cheap and good enough there.
//
// The search area is a small fixed box centered on the mover, so cost is bounded
// regardless of overall map size. NPC TargetSearchRange in NPC.cfg tops out at 9 tiles,
// so a 20-tile radius leaves generous room to route around obstacles without ever needing
// a map-spanning search.
// ---------------------------------------------------------------------------------------

#define DEF_PATHFIND_RADIUS  20
#define DEF_PATHFIND_DIM     (DEF_PATHFIND_RADIUS*2 + 1)
#define DEF_PATHFIND_CELLS   (DEF_PATHFIND_DIM*DEF_PATHFIND_DIM)
#define DEF_PATHFIND_HEAPCAP (DEF_PATHFIND_CELLS*4)

struct SPathNode
{
	BOOL  bVisited;
	BOOL  bClosed;
	short sG;
	char  cFromDir;   // direction taken from the parent to reach this cell; 0 at the start node
};

// Single-threaded server (the game loop itself is not multithreaded - only the debug UI
// runs on its own thread), so static reuse across calls is safe and avoids re-allocating
// ~1700 cells' worth of scratch state on the stack every time an NPC takes a chase step.
static SPathNode s_stPathNode[DEF_PATHFIND_CELLS];
static int       s_iPathHeapNode[DEF_PATHFIND_HEAPCAP];
static int       s_iPathHeapF[DEF_PATHFIND_HEAPCAP];
static int       s_iPathHeapSize;

static void _PathHeapPush(int iNode, int iF)
{
 int i, iParent, iTmp;

	if (s_iPathHeapSize >= DEF_PATHFIND_HEAPCAP) return; // search area is bounded, this can't overflow in practice

	i = s_iPathHeapSize++;
	s_iPathHeapNode[i] = iNode;
	s_iPathHeapF[i]    = iF;

	while (i > 0) {
		iParent = (i - 1) / 2;
		if (s_iPathHeapF[iParent] <= s_iPathHeapF[i]) break;

		iTmp = s_iPathHeapNode[iParent]; s_iPathHeapNode[iParent] = s_iPathHeapNode[i]; s_iPathHeapNode[i] = iTmp;
		iTmp = s_iPathHeapF[iParent];    s_iPathHeapF[iParent]    = s_iPathHeapF[i];    s_iPathHeapF[i]    = iTmp;
		i = iParent;
	}
}

static BOOL _PathHeapPop(int * piNode)
{
 int i, iLeft, iRight, iSmallest, iTmp;

	if (s_iPathHeapSize <= 0) return FALSE;

	*piNode = s_iPathHeapNode[0];

	s_iPathHeapSize--;
	s_iPathHeapNode[0] = s_iPathHeapNode[s_iPathHeapSize];
	s_iPathHeapF[0]    = s_iPathHeapF[s_iPathHeapSize];

	i = 0;
	for (;;) {
		iLeft     = i*2 + 1;
		iRight    = i*2 + 2;
		iSmallest = i;
		if ((iLeft  < s_iPathHeapSize) && (s_iPathHeapF[iLeft]  < s_iPathHeapF[iSmallest])) iSmallest = iLeft;
		if ((iRight < s_iPathHeapSize) && (s_iPathHeapF[iRight] < s_iPathHeapF[iSmallest])) iSmallest = iRight;
		if (iSmallest == i) break;

		iTmp = s_iPathHeapNode[iSmallest]; s_iPathHeapNode[iSmallest] = s_iPathHeapNode[i]; s_iPathHeapNode[i] = iTmp;
		iTmp = s_iPathHeapF[iSmallest];    s_iPathHeapF[iSmallest]    = s_iPathHeapF[i];    s_iPathHeapF[i]    = iTmp;
		i = iSmallest;
	}

	return TRUE;
}

// Octile-distance heuristic consistent with the 10/14 step costs above.
static int _iPathHeuristic(int ox, int oy, int iGoalOx, int iGoalOy)
{
 int dx, dy;

	dx = abs(ox - iGoalOx);
	dy = abs(oy - iGoalOy);

	if (dx > dy) return dx*10 + dy*4;
	else         return dy*10 + dx*4;
}

// Finds the first step of a short obstacle-avoiding route from (sX,sY) toward (dX,dY) and
// returns it as a compass direction (1-8) in *pcDir. The search stops as soon as it reaches
// a tile adjacent to the goal rather than the goal tile itself, since the goal is usually
// occupied (the chased/fleeing target standing on it) and callers only ever need to get
// next to it. Returns FALSE - caller should fall back to the plain greedy stepper - if the
// target is outside the bounded search radius or genuinely unreachable within it.
BOOL CMap::bFindPathStep(short sX, short sY, short dX, short dY, char * pcDir)
{
 int  iOriginX, iOriginY;
 int  ox, oy, oxN, oyN;
 int  iCur, iNext, iIdx;
 int  iG, iF;
 int  iGoalOx, iGoalOy;
 char cDir;
 int  i;

	if ((abs((int)sX - (int)dX) > DEF_PATHFIND_RADIUS) || (abs((int)sY - (int)dY) > DEF_PATHFIND_RADIUS)) return FALSE;
	if ((abs((int)sX - (int)dX) <= 1) && (abs((int)sY - (int)dY) <= 1)) return FALSE; // already adjacent

	iOriginX = sX - DEF_PATHFIND_RADIUS;
	iOriginY = sY - DEF_PATHFIND_RADIUS;

	iGoalOx = dX - iOriginX;
	iGoalOy = dY - iOriginY;

	ZeroMemory(s_stPathNode, sizeof(s_stPathNode));
	s_iPathHeapSize = 0;

	ox = sX - iOriginX;
	oy = sY - iOriginY;
	iCur = oy*DEF_PATHFIND_DIM + ox;
	s_stPathNode[iCur].bVisited = TRUE;
	s_stPathNode[iCur].sG       = 0;
	s_stPathNode[iCur].cFromDir = 0;
	_PathHeapPush(iCur, _iPathHeuristic(ox, oy, iGoalOx, iGoalOy));

	iIdx = -1; // will hold the closed node adjacent to the goal, once found

	while (_PathHeapPop(&iCur)) {

		if (s_stPathNode[iCur].bClosed == TRUE) continue;
		s_stPathNode[iCur].bClosed = TRUE;

		ox = iCur % DEF_PATHFIND_DIM;
		oy = iCur / DEF_PATHFIND_DIM;

		if ((abs(ox - iGoalOx) <= 1) && (abs(oy - iGoalOy) <= 1)) {
			iIdx = iCur;
			break;
		}

		for (cDir = 1; cDir <= 8; cDir++) {
			oxN = ox + s_cPathDirX[cDir];
			oyN = oy + s_cPathDirY[cDir];

			if ((oxN < 0) || (oxN >= DEF_PATHFIND_DIM) || (oyN < 0) || (oyN >= DEF_PATHFIND_DIM)) continue;

			iNext = oyN*DEF_PATHFIND_DIM + oxN;
			if (s_stPathNode[iNext].bClosed == TRUE) continue;

			if (bGetMoveableDir((short)(iOriginX + ox), (short)(iOriginY + oy), cDir) == FALSE) continue;

			iG = s_stPathNode[iCur].sG + s_cPathDirCost[cDir];

			if ((s_stPathNode[iNext].bVisited == FALSE) || (iG < s_stPathNode[iNext].sG)) {
				s_stPathNode[iNext].bVisited = TRUE;
				s_stPathNode[iNext].sG       = (short)iG;
				s_stPathNode[iNext].cFromDir = cDir;
				iF = iG + _iPathHeuristic(oxN, oyN, iGoalOx, iGoalOy);
				_PathHeapPush(iNext, iF);
			}
		}
	}

	if (iIdx < 0) return FALSE; // nothing adjacent to the goal is reachable inside the box

	// Walk the parent chain back from the arrival node to the start; the last direction
	// visited (i.e. the step out of the start node) is the one to actually take this tick.
	cDir = 0;
	for (i = 0; i < DEF_PATHFIND_CELLS; i++) {
		if (s_stPathNode[iIdx].cFromDir == 0) break; // reached the start node

		cDir = s_stPathNode[iIdx].cFromDir;
		ox   = (iIdx % DEF_PATHFIND_DIM) - s_cPathDirX[cDir];
		oy   = (iIdx / DEF_PATHFIND_DIM) - s_cPathDirY[cDir];
		iIdx = oy*DEF_PATHFIND_DIM + ox;
	}

	if (cDir == 0) return FALSE; // arrival node WAS the start (already adjacent - shouldn't happen, checked above)

	*pcDir = cDir;
	return TRUE;
}

BOOL CMap::bGetIsMoveAllowedTile(short dX, short dY)
{
 class CTile * pTile;	
	
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if (pTile->m_bIsMoveAllowed == FALSE) return FALSE;
	if (pTile->m_bIsTempMoveAllowed == FALSE) return FALSE;
	
	return TRUE;
}

/*BOOL CMap::sub_4C0F20(short dX, short dY)
{
 class CTile * pTile;	
	
	3CA18h = 0;

	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);

}*/

BOOL CMap::bGetIsTeleport(short dX, short dY)
{
 class CTile * pTile;	
	
	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if (pTile->m_bIsTeleport == FALSE) return FALSE;
	
	return TRUE;
}

void CMap::ClearOwner(int iDebugCode, short sOwnerH, char cOwnerType, short sX, short sY)
{
 class CTile * pTile;	
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;
 
	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);

	// 현 위치에 핸들이 일치하면 모두 클리어한다. 
	if ((pTile->m_sOwner == sOwnerH) && (pTile->m_cOwnerClass == cOwnerType)) {
		pTile->m_sOwner      = NULL;
		pTile->m_cOwnerClass = NULL;
	}

	// 
	if ((pTile->m_sDeadOwner == sOwnerH) && (pTile->m_cDeadOwnerClass == cOwnerType)) {
		pTile->m_sDeadOwner      = NULL;
		pTile->m_cDeadOwnerClass = NULL;
	}
}

void CMap::ClearDeadOwner(short sX, short sY)
{
 class CTile * pTile;	

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return;	

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	pTile->m_sDeadOwner      = NULL;
	pTile->m_cDeadOwnerClass = NULL;
}

BOOL CMap::bSetItem(short sX, short sY, class CItem * pItem)
{
 class CTile * pTile;
 register int i, iPos;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return NULL;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);

	// If Gold is already on the ground here, merge onto the existing pile instead of taking another slot.
	if (pItem->m_sIDnum == 90) { // Gold
		for (i = 0; i < DEF_TILE_PER_ITEMS; i++) {
			if ((pTile->m_pItem[i] != NULL) && (pTile->m_pItem[i]->m_sIDnum == 90)) { // Gold
				pTile->m_pItem[i]->m_dwCount += pItem->m_dwCount;
				delete pItem;
				return TRUE;
			}
		}
	}

	// Rank the new item by value (m_pItem[0] is the only slot the client renders),
	// so pricier items stay visible on top and common/low-value loot sinks to the bottom.
	for (iPos = 0; iPos < DEF_TILE_PER_ITEMS; iPos++) {
		if ((pTile->m_pItem[iPos] == NULL) || (pTile->m_pItem[iPos]->m_wPrice < pItem->m_wPrice))
			break;
	}

	// Stack is full and every slot already holds an item of equal or greater
	// value - the new item is the least valuable of the bunch, so it doesn't fit.
	if (iPos == DEF_TILE_PER_ITEMS) {
		delete pItem;
		return TRUE;
	}

	// v1.4 마지막 타일에 아이템이 있다면 삭제시키고 이동한다.
	if (pTile->m_pItem[DEF_TILE_PER_ITEMS-1] != NULL)
		delete pTile->m_pItem[DEF_TILE_PER_ITEMS-1];
	else pTile->m_cTotalItem++;

	for (i = DEF_TILE_PER_ITEMS-2; i >= iPos; i--)
		pTile->m_pItem[i+1] = pTile->m_pItem[i];

	pTile->m_pItem[iPos] = pItem;
	return TRUE;
}


class CItem * CMap::pGetItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, char * pRemainItemColor, int iIndex) //v1.4 color
{
 class CTile * pTile;
 class CItem * pItem;
 register int i;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return NULL;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	if (pTile->m_cTotalItem == 0) return NULL;

	// Ground-item UI lets a player pick a specific stack slot (not just the top item);
	// a stale or out-of-range index falls back to the top rather than corrupting the tile.
	if ((iIndex < 0) || (iIndex >= pTile->m_cTotalItem)) iIndex = 0;

	pItem = pTile->m_pItem[iIndex];

	for (i = iIndex; i <= DEF_TILE_PER_ITEMS-2; i++)
		pTile->m_pItem[i] = pTile->m_pItem[i+1];
	pTile->m_cTotalItem--;
	pTile->m_pItem[pTile->m_cTotalItem] = NULL;

	if (pTile->m_pItem[0] == NULL) {
		*pRemainItemSprite      = 0;
		*pRemainItemSpriteFrame = 0;
		*pRemainItemColor       = 0;
	}
	else
	{
		*pRemainItemSprite      = pTile->m_pItem[0]->m_sSprite;
		*pRemainItemSpriteFrame = pTile->m_pItem[0]->m_sSpriteFrame;
		*pRemainItemColor       = pTile->m_pItem[0]->m_cItemColor;
	}

	return pItem;
}


// Read-only snapshot of a tile's item stack for the ground-item info response; does not
// pop or mutate anything, unlike pGetItem/pGetGoldItem above.
int CMap::iGetItemStack(short sX, short sY, class CItem ** ppOut, int iMax)
{
 class CTile * pTile;
 register int i, iCount;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return 0;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);

	iCount = pTile->m_cTotalItem;
	if (iCount > iMax) iCount = iMax;

	// Items occupy a contiguous prefix, so m_cTotalItem should never outrun the array - but
	// stop at the first hole anyway: the caller dereferences every entry to build a network
	// reply, and a count/array desync here would take the whole zone down.
	for (i = 0; i < iCount; i++) {
		if (pTile->m_pItem[i] == NULL) break;
		ppOut[i] = pTile->m_pItem[i];
	}

	return i;
}


class CItem * CMap::pGetGoldItem(short sX, short sY, short * pRemainItemSprite, short * pRemainItemSpriteFrame, char * pRemainItemColor)
{
 class CTile * pTile;
 class CItem * pItem;
 register int i, iFoundIndex;

	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return NULL;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	if (pTile->m_cTotalItem == 0) return NULL;

	// Find Gold in the stack regardless of its position (later drops can bury it).
	iFoundIndex = -1;
	for (i = 0; i < pTile->m_cTotalItem; i++) {
		if ((pTile->m_pItem[i] != NULL) && (pTile->m_pItem[i]->m_sIDnum == 90)) { // Gold
			iFoundIndex = i;
			break;
		}
	}
	if (iFoundIndex == -1) return NULL;

	pItem = pTile->m_pItem[iFoundIndex];

	for (i = iFoundIndex; i <= DEF_TILE_PER_ITEMS-2; i++)
		pTile->m_pItem[i] = pTile->m_pItem[i+1];
	pTile->m_cTotalItem--;
	pTile->m_pItem[pTile->m_cTotalItem] = NULL;

	if (pTile->m_pItem[0] == NULL) {
		*pRemainItemSprite      = 0;
		*pRemainItemSpriteFrame = 0;
		*pRemainItemColor       = 0;
	}
	else
	{
		*pRemainItemSprite      = pTile->m_pItem[0]->m_sSprite;
		*pRemainItemSpriteFrame = pTile->m_pItem[0]->m_sSpriteFrame;
		*pRemainItemColor       = pTile->m_pItem[0]->m_cItemColor;
	}

	return pItem;
}


int CMap::iCheckItem(short sX, short sY)
{
 class CTile * pTile;	
 class CItem * pItem;
	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return NULL;

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);
	pItem =  pTile->m_pItem[0];
	if (pTile->m_cTotalItem == 0) return NULL;

	return pItem->m_sIDnum;
}


BOOL CMap::bIsValidLoc(short sX, short sY)
{
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return FALSE;
	return TRUE;
}

BOOL CMap::bInit(char * pName)
{
 int i;
	// 맵파일을 로드한다.
	ZeroMemory(m_cName, sizeof(m_cName));
	strcpy(m_cName, pName);

	ZeroMemory(m_cLocationName, sizeof(m_cLocationName));

	if (_bDecodeMapDataFileContents() == FALSE) 
		return FALSE;	

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
		m_pTeleportLoc[i] = NULL;

	return TRUE;
}

BOOL CMap::_bDecodeMapDataFileContents()
{
 HANDLE hFile;
 char  cMapFileName[256], cHeader[260], cTemp[100];
 DWORD dwFileSize, nRead;
 register int i, ix, iy;
 char * token, cReadMode;
 char seps[] = "= \t\n";
 class CStrTok * pStrTok = NULL;
 class CTile * pTile;
 short * sp;

	ZeroMemory(cMapFileName, sizeof(cMapFileName));
	strcat(cMapFileName, "mapdata\\");
	strcat(cMapFileName, m_cName);
	strcat(cMapFileName, ".amd");
	
	hFile = CreateFile(cMapFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;
	dwFileSize = GetFileSize(hFile, NULL);
	
	ZeroMemory(cHeader, sizeof(cHeader));
	ReadFile(hFile, (char *)cHeader, 256, &nRead, NULL);

	// 헤더정보를 분석한다.

	// 먼저 널문자를 없앤다.
	for (i = 0; i < 256; i++) 
		if (cHeader[i] == NULL) cHeader[i] = ' ';

	cReadMode = 0;

	pStrTok = new class CStrTok(cHeader, seps);
	token = pStrTok->pGet();
	//token = strtok( cHeader, seps );   
	while( token != NULL )   {
			
		if (cReadMode != 0) {
			switch (cReadMode) {
			case 1:
				m_sSizeX = atoi(token);
				cReadMode = 0;
				break;
			case 2:
				m_sSizeY = atoi(token);
				cReadMode = 0;
				break;
			case 3:
				m_sTileDataSize = atoi(token);
				cReadMode = 0;
				break;
			}
		}	
		else {
			if (memcmp(token, "MAPSIZEX",8) == 0) cReadMode = 1;
			if (memcmp(token, "MAPSIZEY",8) == 0) cReadMode = 2;
			if (memcmp(token, "TILESIZE",8) == 0) cReadMode = 3; 
		}
			
		//token = strtok( NULL, seps );
		token = pStrTok->pGet();
	}

	// 크기에 맞게 타일을 생성한다.
	m_pTile = (class CTile *)new class CTile[m_sSizeX * m_sSizeY];

	// 타일의 정보를 읽어들인다.
	for (iy = 0; iy < m_sSizeY; iy++)
	for (ix = 0; ix < m_sSizeX; ix++) {
		ReadFile(hFile, (char *)cTemp, m_sTileDataSize, &nRead, NULL);	
		pTile = (class CTile *)(m_pTile + ix + iy*m_sSizeY);
		if ((cTemp[8] & 0x80) != 0) {
			// 갈수없는 속성이 세트되어 있다.
			 pTile->m_bIsMoveAllowed = FALSE;
		}
		else pTile->m_bIsMoveAllowed = TRUE;

		if ((cTemp[8] & 0x40) != 0) {
			// 텔레포트 속성이 세트되어 있다.
			 pTile->m_bIsTeleport = TRUE;
		}
		else pTile->m_bIsTeleport = FALSE;

		if ((cTemp[8] & 0x20) != 0) {
			 pTile->m_bIsFarm = TRUE;
		}
		else pTile->m_bIsFarm = FALSE;

		sp = (short *)&cTemp[0];
		if (*sp == 19) {
			// 물 타일이다. 
			 pTile->m_bIsWater = TRUE;
		}
		else pTile->m_bIsWater = FALSE;


	}

	CloseHandle(hFile);

	if (pStrTok != NULL) delete pStrTok;
	return TRUE;
}


BOOL CMap::bSearchTeleportDest(int sX, int sY, char * pMapName, int * pDx, int * pDy, char * pDir)
{
 register int i;

	for (i = 0; i < DEF_MAXTELEPORTLOC; i++)
	if ((m_pTeleportLoc[i] != NULL) && (m_pTeleportLoc[i]->m_sSrcX == sX) && (m_pTeleportLoc[i]->m_sSrcY == sY)) {
		// 같은 좌표를 갖는 텔레포트 위치를 찾았다. 	
		memcpy(pMapName, m_pTeleportLoc[i]->m_cDestMapName, 10);
		*pDx  = m_pTeleportLoc[i]->m_sDestX;
		*pDy  = m_pTeleportLoc[i]->m_sDestY;
		*pDir = m_pTeleportLoc[i]->m_cDir;
		return TRUE;
	}

	return FALSE;
}

void CMap::SetDynamicObject(WORD wID, short sType, short sX, short sY, DWORD dwRegisterTime)
{
 class CTile * pTile;	

	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return; 

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);

	pTile->m_wDynamicObjectID   = wID;
	pTile->m_sDynamicObjectType = sType;
	pTile->m_dwDynamicObjectRegisterTime = dwRegisterTime;
}

BOOL CMap::bGetDynamicObject(short sX, short sY, short *pType, DWORD *pRegisterTime, int * pIndex)
{
 class CTile * pTile;	

	
	if ((sX < 0) || (sX >= m_sSizeX) || (sY < 0) || (sY >= m_sSizeY)) return FALSE; 

	pTile = (class CTile *)(m_pTile + sX + sY*m_sSizeY);

	*pType = pTile->m_sDynamicObjectType;
	*pRegisterTime = pTile->m_dwDynamicObjectRegisterTime;
	if (pIndex != NULL) *pIndex = pTile->m_wDynamicObjectID;

	return TRUE;
}

int CMap::iGetEmptyNamingValue()
{
 register int i;

	for (i = 0; i < 1000; i++)
	if (m_bNamingValueUsingStatus[i] == FALSE) {

		m_bNamingValueUsingStatus[i] = TRUE;
		return i;
	}

	return -1;
}

void CMap::SetNamingValueEmpty(int iValue)
{
	m_bNamingValueUsingStatus[iValue] = FALSE;
}

BOOL CMap::bGetIsWater(short dX, short dY)
{
 class CTile * pTile;	
	
	if ((dX < 14) || (dX >= m_sSizeX - 16) || (dY < 12) || (dY >= m_sSizeY - 14)) return FALSE;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	
	if (pTile->m_bIsWater == FALSE) return FALSE;
	
	return TRUE;
}

//v2.19 2002-12-16 농사 스킬 관련
BOOL CMap::bRemoveCropsTotalSum()
{
	if(m_iTotalAgriculture < DEF_MAXAGRICULTURE)
	{
		m_iTotalAgriculture--;
		if(m_iTotalAgriculture < 0)
		{
			m_iTotalAgriculture = 0;
		}
		return TRUE;
	}
	return FALSE;
}

//v2.19 2002-12-16 농사 스킬 관련
BOOL CMap::bAddCropsTotalSum()
{
	if(m_iTotalAgriculture < DEF_MAXAGRICULTURE)
	{
		m_iTotalAgriculture++;
		return TRUE;
	}
	return FALSE;
}

BOOL CMap::bGetIsFarm(short tX, short tY)
{
 class CTile * pTile;	
	
	if ((tX < 14) || (tX >= m_sSizeX - 16) || (tY < 12) || (tY >= m_sSizeY - 14)) return FALSE;

	pTile = (class CTile *)(m_pTile + tX + tY*m_sSizeY);
	
	if (pTile->m_bIsFarm == FALSE) return FALSE;
	
	return TRUE;
}

int CMap::iAnalyze(char cType, int * pX, int * pY, int * pV1, int * pV2, int * pV3)
{

 
	// 현재 맵의 상황을 분석하여 쿼리에 맞는 위치를 반환한다. 
	switch (cType) {
	case 1:
		// 현재 교전이 벌어지고 있는 곳의 위치를 찾는다. 

		break;


	}

	return 0;
}

void CMap::SetTempMoveAllowedFlag(int dX, int dY, BOOL bFlag)
{
 class CTile * pTile;	
	
	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	pTile->m_bIsTempMoveAllowed = bFlag;
}

int CMap::iRegisterOccupyFlag(int dX, int dY, int iSide, int iEKNum, int iDOI)
{
 int i;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return -1;
	
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++) 
	if (m_pOccupyFlag[i] == NULL) {
		// 값을 할당하고 반환.
		m_pOccupyFlag[i] = new class COccupyFlag(dX, dY, iSide, iEKNum, iDOI);
		if (m_pOccupyFlag == NULL) return -1;
		else return i;
	}
	
	return -1;
}



void CMap::ClearSectorInfo()
{
 int ix, iy;

	for (ix = 0; ix < DEF_MAXSECTORS; ix++)
	for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
		m_stSectorInfo[ix][iy].iNeutralActivity = 0;
		m_stSectorInfo[ix][iy].iAresdenActivity = 0;
		m_stSectorInfo[ix][iy].iElvineActivity  = 0;
		m_stSectorInfo[ix][iy].iMonsterActivity = 0;
		m_stSectorInfo[ix][iy].iPlayerActivity  = 0;
	} 
}

void CMap::ClearTempSectorInfo()
{
 int ix, iy;

	for (ix = 0; ix < DEF_MAXSECTORS; ix++)
	for (iy = 0; iy < DEF_MAXSECTORS; iy++) {
		m_stTempSectorInfo[ix][iy].iNeutralActivity = 0;
		m_stTempSectorInfo[ix][iy].iAresdenActivity = 0;
		m_stTempSectorInfo[ix][iy].iElvineActivity  = 0;
		m_stTempSectorInfo[ix][iy].iMonsterActivity = 0;
		m_stTempSectorInfo[ix][iy].iPlayerActivity  = 0;
	} 
}

void CMap::_SetupNoAttackArea()
{
 int i, ix, iy;
 class CTile * pTile;	

	for (i = 0; i < DEF_MAXNMR; i++) {
		if ((m_rcNoAttackRect[i].top > 0)) {
			// No-Magic-Area가 정의되어 있다.
			for (ix = m_rcNoAttackRect[i].left; ix <= m_rcNoAttackRect[i].right; ix++)
			for (iy = m_rcNoAttackRect[i].top; iy <= m_rcNoAttackRect[i].bottom; iy++) {
				pTile = (class CTile *)(m_pTile + ix + iy*m_sSizeY);
				pTile->m_iAttribute = pTile->m_iAttribute | 0x00000004;		// 3번째 비트를 세팅한다. 안전지대 
			}
		}
		else if (m_rcNoAttackRect[i].top == -10) {
			// 전 영역 공격 금지다.	
			for (ix = 0; ix < m_sSizeX; ix++)
			for (iy = 0; iy < m_sSizeY; iy++) {
				pTile = (class CTile *)(m_pTile + ix + iy*m_sSizeY);
				pTile->m_iAttribute = pTile->m_iAttribute | 0x00000004;		// 3번째 비트를 세팅한다. 안전지대
			}
		}
	}
}

/*********************************************************************************************************************
**  int CMap::iGetAttribute(int dX, int dY, int iBitMask)															**
**  description			:: check if the tile contains a dead player													**
**  last updated		:: November 20, 2004; 9:55 PM; Hypnotoad													**
**	return value		:: int																						**
**********************************************************************************************************************/
int CMap::iGetAttribute(int dX, int dY, int iBitMask)
{
 class CTile * pTile;

	if ((dX < 20) || (dX >= m_sSizeX - 20) || (dY < 20) || (dY >= m_sSizeY - 20)) return -1;

	pTile = (class CTile *)(m_pTile + dX + dY*m_sSizeY);
	return (pTile->m_iAttribute & iBitMask);
}

BOOL CMap::bAddCrusadeStructureInfo(char cType, short sX, short sY, char cSide)
{
 register int i;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
	if (m_stCrusadeStructureInfo[i].cType == NULL) {
		m_stCrusadeStructureInfo[i].cType = cType;
		m_stCrusadeStructureInfo[i].cSide = cSide;
		m_stCrusadeStructureInfo[i].sX = sX;
		m_stCrusadeStructureInfo[i].sY = sY;

		m_iTotalCrusadeStructures++;
		return TRUE;
	}

	return FALSE;
}

/*BOOL CMap::bAddHeldenianTowerInfo(char cType, short sX, short sY, char cSide)
{
 register int i;

	for (i = 0; i < DEF_MAXHELDENIANTOWER; i++)
	if (m_stHeldenianTower[i].cType == NULL) {
	if (m_stHeldenianTower[i].cSide == 1) {
		m_stHeldenianTower[i].sTypeID = sTypeID;
		m_stHeldenianTower[i].cSide = cSide;
		m_stHeldenianTower[i].sX = sX;
		m_stHeldenianTower[i].sY = sY;
		m_iHeldenianAresdenLeftTower++;
		return TRUE;
	}
	else if (m_stHeldenianTower[i].cSide == 2) {
		m_stHeldenianTower[i].sTypeID = sTypeID;
		m_stHeldenianTower[i].cSide = cSide;
		m_stHeldenianTower[i].sX = sX;
		m_stHeldenianTower[i].sY = sY;
		m_iHeldenianElvineLeftTower++;
		return TRUE;
	}

	return FALSE;
}*/

BOOL CMap::bRemoveCrusadeStructureInfo(short sX, short sY)
{
 register int i;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
	if ((m_stCrusadeStructureInfo[i].sX == sX) && (m_stCrusadeStructureInfo[i].sY == sY)) {
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
		goto RCSI_REARRANGE;
	}

	return FALSE;

RCSI_REARRANGE:;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES-1; i++)
	if ((m_stCrusadeStructureInfo[i].cType == NULL) && (m_stCrusadeStructureInfo[i+1].cType != NULL)) {
		m_stCrusadeStructureInfo[i].cType = m_stCrusadeStructureInfo[i+1].cType;
		m_stCrusadeStructureInfo[i].cSide = m_stCrusadeStructureInfo[i+1].cSide;
		m_stCrusadeStructureInfo[i].sX = m_stCrusadeStructureInfo[i+1].sX;
		m_stCrusadeStructureInfo[i].sY = m_stCrusadeStructureInfo[i+1].sY;

		m_stCrusadeStructureInfo[i+1].cType = NULL;
		m_stCrusadeStructureInfo[i+1].cSide = NULL;
		m_stCrusadeStructureInfo[i+1].sX = NULL;
		m_stCrusadeStructureInfo[i+1].sY = NULL;
	}

	m_iTotalCrusadeStructures--;
	return TRUE;
}



void CMap::RestoreStrikePoints()
{
 int i;

	for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) {
		m_stStrikePoint[i].iInitHP = m_stStrikePoint[i].iHP;
	}
}

