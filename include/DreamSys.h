#ifndef CLASS_DREAMSYS
#define CLASS_DREAMSYS

//.sbss values (defined in DreamSys.c)
extern s8 (*gpNavChallengesComplete)[30];
extern s32 *gpDinamicLinkPenalty;
extern s32 D_8008ACBC;
extern s32 D_8008ACC0;
extern s32 D_8008ACC4;
extern s32 D_8008ACC8;

typedef struct CinematicCall{
	s16 bank;
	s16 entry;
} CinematicCall;

typedef struct {
	MoodGraphPoint lastMood;
	struct sumAxis{
		s32 dynamic;
		s32 upper;
	} sumMoods;
	s32 amountMoods;
} MoodGraphContributor;

typedef struct PlayerSpawnPoint {
	struct MapChunk {
		u8 col;
		u8 row;
	} chunk;
	struct MapTile {
		u8 col;
		u8 row;
	} tile;
	struct RelativePos {
		s16 x;
		s16 y;
		s16 z;
	} position;
} PlayerSpawnPoint;

typedef struct {
	s32 stageID;
	PlayerSpawnPoint position;
	struct Angle{
		s16 angle;
		s16 one;
	} pitch; //Does not do what you think it does
	struct Angle heading;
	struct Angle roll; //Ditto
	s16 timeLimit;
	s32 unknown_value_0x1c;
	s32 day;
} FlashbackEntry;

// FIXME: this struct is a mess. half the fields are unknown_values placeholder arrays
// because I keep finding new stuff the game reads. total size is ~0x88c and counting.
// also "Flasback" is misspelled in 400 places and at this point I'm keeping it as a tribute.
typedef struct DreamSys {
	struct vtable_DreamSys *vt;
	s8 unknown_values_0x4[32];

	s32 dreamTimer;
	s8 unknown_values_0x28[28]; // another padding hole I can't identify

	s32 unknwon_int_0x44;
	s8 unknown_values_0x48[32];

	bool isFlashbackSession;
	s8 unknown_values_0x6c[200];

	s32 dreamTimeLimit;
	s8 unknown_values_0x138[12];

	MoodGraphContributor areaMoods;
	MoodGraphContributor entityMoods;
	s32 currentLocation;
	CinematicCall nextCinematic;
	PlayerSpawnPoint linkCoordinates;
	s32 unknown_sdata_0x178;
	s32 currentYear;
	s32 currentDay;
	s32 totalFlasbackUnlockScore;
	s32 navigationFlasbackUnlockScore;
	s32 instanceFlasbackUnlockScore;
	MoodGraphPoint moodPreviousDays[365];
	s32 amountFlashbacksAvailable;
	FlashbackEntry storedFlasbacks[10];

	s8 unknown_values_0x5d8[8];
	s8 navChallengesArray[30];
	s32 amountDynamicLinksDone;
	s8 unknown_values_0x604[116]; // more mystery padding

	bool screenShakeOn;
	s32 unknown_word_0x67c;
	s32 unknown_word_0x680;
	s8 unknown_values_0x684[500]; // half a kilobyte of ???

	s32 unk_0x878;
	s32 currentFlashbackIndex;
	s8 unknown_values_0x880[12];
	s32 storedDay;
} DreamSys;

struct vtable_DreamSys{
	u32 unknown_int;
	void *extfunc_17eb0;
	void *Constructor;
	u32 unknown_functions_0xc[13];
	void * func_800588ec ;
	u32 unknown_functions_0x44[2];
	void *func_58968;
	u32 unknown_functions_0x50[18];
	void *TimerTick;
	u32 unknown_functions_0x9c[17];
	void *LinkWall;
	u32 unknown_functions_0xe4[8];
	s32 (*GetSetDreamTimeLimit)(DreamSys *this, s32 time);
	u32 unknown_functions_0x104[36];
	void (*InitNewGame)(DreamSys *this);
	void (*GetSetScreenShake)(DreamSys *this, bool *value);
	void * func_8005a2e4 ;
	s32 (*AdvanceDay)(DreamSys *this);
	void * func_8005a33c ;
	void * func_8005a344 ;
	void * func_8005a350 ;
	s32 (*StartDay)(DreamSys *this);
	s32 (*EndDay)(DreamSys *this, s32 arg1);
	CinematicCall (*GetCinematic)(DreamSys *this);
	void (*InitSpawnLoc)(DreamSys *this);
	void (*DynamicLink)(DreamSys *this);
	bool (*StaticWallLink)(DreamSys *this, PlayerSpawnPoint *currentPos);
	bool (*LoadNextFlashback)(DreamSys *this, bool unknown);
	u32 unknown_functions_0x1d0[5];
	void *ProcessChunkChange;
	void (*InstanceEffectsOnPlayer)(DreamSys *this, void *entity, int effect);
	void (*GetPreviousDayMood)(DreamSys *this, MoodGraphPoint *target, bool unknown);
	void (*InitMoodContibutors)(DreamSys *this, MoodGraphPoint *special);
	void (*LogChunkMood)(DreamSys *this, PlayerSpawnPoint *currentPos);
	void (*LogInstanceMood)(DreamSys *this,MoodGraphPoint *source);
	void (*UpdateDreamChart)(DreamSys *this, MoodGraphPoint *ret);
	s32 (*GetDreamColor)(DreamSys *this);
	void (*ClearMoodGraph)(DreamSys *this, MoodGraphContributor *contributor);
	void (*LogMood)(DreamSys* this, MoodGraphContributor* layer, MoodGraphPoint* mood);
	void (*GetMoodAverage)(DreamSys *this, MoodGraphContributor *layer, MoodGraphPoint *ret);
	void (*CalcUnlockScore)(DreamSys *this);
	void *GameManager__AddFlashback;
	void *GameManager__FlashbackSaving;
};

typedef enum DreamColors{
	DREAM_COLOR_BLACK, DREAM_COLOR_BLUE,
	DREAM_COLOR_GREEN, DREAM_COLOR_CYAN,
	DREAM_COLOR_RED, DREAM_COLOR_PINK,
	DREAM_COLOR_YELLOW, DREAM_COLOR_WHITE,
}DreamColors;

typedef struct LocationSpawn{
	struct MapChunk chunk;
	struct MapTile tile;
	s8 adjustment;
	s8 extra;
}LocationSpawn;

typedef struct StaticLinkTrigger{
	struct MapChunk chunk;
	union TriggerTile{
		struct MapTile axis;
		s16 value;
	} tile;
	s8 stage;
	s8 spawnpointIndex;
}StaticLinkTrigger;

// Jumptable holding all of DreamSys's "virtual" methods
extern struct vtable_DreamSys DREAMSYS_METHODS;

extern s16 LOCATION_TIME_LIMITS[];
extern struct RelativePos SPAWN_POS_ADJUST[];
extern LocationSpawn* LOCATION_SPAWNPOINTS[];
extern s8 LEN_LOCATION_SPAWNPOINTS[];
extern LocationSpawn* LOCATION_PERMALINK_SPAWNS[];
extern StaticLinkTrigger* LOCATION_PERMALINK_TRIGGERS[];
extern s8 LEN_LOCATION_PERMALINK_TRIGGERS[];
extern s16 SPECIAL_DAYS[];
extern s8 SPECIAL_COLORS[];

/* 
 * Called when the player hits a wall.
 * Tries static link first, then falls back to dynamic.
 */
void DreamSys__WallLink(DreamSys *this, void* unk_class_86aa0, int arg2);

// Returns previous time limit. Negative values store as-is (pass-through for "no change").
s32 DreamSys__GetSetDreamTimeLimit(DreamSys *this, s32 value);

void DreamSys__InitNewGame(DreamSys *this);
void DreamSys__GetSetScreenShake(DreamSys *this, bool *value);

// Advances the day counter, loops on new year. Returns 0-364.
s32 DreamSys__AdvanceDay(DreamSys *this);

// Figures out what kind of dream comes next. Returns stage ID, -1 for cinematic.
s32 DreamSys__StartDay(DreamSys *this);

s32 DreamSys__EndDay(DreamSys *this, s32 arg1);

// Next cinematic to play. entry=-1 means none scheduled.
CinematicCall DreamSys__GetCinematic(DreamSys *this);

void DreamSys__InitSpawnLoc(DreamSys *this);

// Handles dynamic or instance links depending on currentLocation
void DreamSys__DynamicLink(DreamSys *this);

/* 
 * Tests whether the player's position triggers a static wall link.
 * Returns true if a valid link was found.
 */
bool DreamSys__StaticWallLink(DreamSys *this, PlayerSpawnPoint *currentPos);

// Loads the next flashback. Returns false at end of session.
bool DreamSys__LoadNextFlashback(DreamSys *this, bool unknown);

// Called during some links. No idea what it does.
bool ExecuteLink(DreamSys *system, s32 stage, s32 unk1, s32 unk2);

// Instance linking + flashback logging
void DreamSys__InstanceEffectsOnJournal(DreamSys *this, void *entity, s32 effect);

void DreamSys__GetPreviousDayMood(DreamSys *this, MoodGraphPoint *target, bool unknown);

/* (Re)inits mood contributors for a new day.
 * If special is not NULL, both start with this point logged. */
void DreamSys__InitMoodContibutors(DreamSys *this, MoodGraphPoint *special);

void DreamSys__LogChunkMood(DreamSys *this, PlayerSpawnPoint *currentPos);
void DreamSys__LogInstanceMood(DreamSys *this, MoodGraphPoint *source);
void DreamSys__UpdateDreamChart(DreamSys *this, MoodGraphPoint *ret);

DreamColors DreamSys__GetDreamColor(DreamSys *this);
DreamColors CalcDreamColor(MoodGraphPoint *mood);

void DreamSys__ClearMoodGraph(DreamSys *this, MoodGraphContributor *contributor);
void DreamSys__LogMood(DreamSys* this, MoodGraphContributor* layer, MoodGraphPoint* mood);

// Averages one contributor into a (-9,-9) - (9,9) mood point
void DreamSys__GetMoodAverage(DreamSys* this, MoodGraphContributor* layer, MoodGraphPoint* ret);

// Combines contributor axis values into -9..9. Last logged gets a boost.
s32 CalcMoodAxis(s32 lank, s32 sum, s32 amount);

void DreamSys__CalcUnlockScore(DreamSys *this);

// Saves a flashback spawn into the player's session
void DreamSys__AddFlashback(DreamSys *this, s32 stage, PlayerSpawnPoint* pos, s32 *angles, s32 unknown, s32 time, s32 day);

// Grey Man calls this to wipe your flashback log
void DreamSys__ResetFlashbackList(DreamSys *this);

struct vtable_DreamSys *Get_vtable_DreamSys(void);

void InitNavChallengesArray(s8 (*arrayMem)[30], s32 *linkCounter);

// Score 0..50,000,000 based on navigation challenges completed
s32 CalcNavigationScore(void);

// Resolves a mood graph point into stage, spawn, and time limit
s32 GenerateInitialSpawn(PlayerSpawnPoint *target, s32 *timeLimit, MoodGraphPoint *mood, s32 day);

/* Gets a random spawn on or away from a stage.
 * stg >= 0: dynamic link away from given stage
 * stg < 0:  semi-static link on stage (used by instances)
 * Both count toward the dynamic link penalty. */
s32 GetRandomSpawnFromLocation(PlayerSpawnPoint *target, s32 stg, s32 unused);

// Checks for special days and loads a random cinematic if it is.
// Returns mood graph contribution, or NULL if not special.
MoodGraphPoint *IsDaySpecial(CinematicCall *cinematic, int day);

#endif
