

// Constants for cheat detection types
#define AIMBOT_DETECTION    1
#define SPEEDHACK_DETECTION 2
#define RAPIDFIRE_DETECTION 3
#define HEALTHHACK_DETECTION 4
#define WALLHACK_DETECTION  5
#define FLYHACK_DETECTION   6

// Constants for the anticheat
#define MAX_AIM_DISTANCE 3.0       // Maximum distance (in units) for aimbot detection
#define MAX_PLAYER_SPEED 15.0      // Maximum speed (in units per second) for speedhack detection
#define MAX_DETECTION_COUNT 5      // Number of detections before taking action
#define MAX_HEALTH_ARMOR 100       // The maximum health and armor allowed for non-cheating players
#define MAX_HEALTH_ARMOR_DIFF 50   // Maximum difference allowed between reported health/armor and actual health/armor for non-cheating players
#define MAX_RAPID_FIRE_INTERVAL 500 // 500 milliseconds (adjust as needed)
#define INVALID_PLAYER_SPEED -1.0
#define MAX_FLY_DISTANCE 10.0
#define MAX_WALLHACK_DISTANCE 200.0 // Adjust as needed
#define WALLHACK_CHECK_INTERVAL 1000 // Check interval in milliseconds (adjust as needed)

// Important
#define PI 3.1415926535897932384626433832795

// Player data
static
    g_AimbotDetection[MAX_PLAYERS],
    g_SpeedhackDetection[MAX_PLAYERS],
    g_RapidFireDetection[MAX_PLAYERS],
    g_HealthHackDetection[MAX_PLAYERS],
    g_WallhackDetection[MAX_PLAYERS],
    g_FlyHackDetection[MAX_PLAYERS],
    g_WallhackDetectionTime[MAX_PLAYERS];
    
// Variables to store previous position and timestamp
static Float:g_PlayerPrevPosX[MAX_PLAYERS], Float:g_PlayerPrevPosY[MAX_PLAYERS], Float:g_PlayerPrevPosZ[MAX_PLAYERS];
static g_PlayerPrevTimestamp[MAX_PLAYERS];
new bool:g_CheatDetectionEnable[7];

// Function prototypes
forward GetPlayerAimPos(playerid, &Float:x, &Float:y, &Float:z);
forward Float:GetPlayerSpeed(playerid);
forward Float:TAnticheat_GetVectorMagnitude(Float:x, Float:y, Float:z);
forward TAnticheat_CheckAimbot(playerid);
forward TAnticheat_CheckSpeedhack(playerid);
forward TAnticheat_CheckRapidFire(playerid);
forward TAnticheat_CheckHealthHack(playerid);
forward TAnticheat_CheckWallhack(playerid);
forward TAnticheat_CheckFlyHack(playerid);
forward TAnticheat_Update(playerid);
forward OnPlayerCheatDetected(playerid, const reason[]);
forward Float:abs(Float:value);

stock TAnticheat_Init()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        g_AimbotDetection[i] = 0;
        g_SpeedhackDetection[i] = 0;
        g_RapidFireDetection[i] = 0;
        g_HealthHackDetection[i] = 0;
        g_WallhackDetection[i] = 0;
    }
}

// Player update hook
public TAnticheat_Update(playerid)
{
    TAnticheat_CheckAimbot(playerid);
    TAnticheat_CheckSpeedhack(playerid);
    TAnticheat_CheckRapidFire(playerid);
    TAnticheat_CheckHealthHack(playerid);
    TAnticheat_CheckWallhack(playerid);
    return 1;
}

// Function to configure cheat detection for all players
stock TAnticheat_Enable(type, bool:statement)
{
    // Validate the cheat detection type
    if (type < 1 || type > FLYHACK_DETECTION)
        return false;

    // Update the cheat detection configuration for the type
    g_CheatDetectionEnabled[type] = statement;

    return true;
}

// Aimbot detection
forward Float:Distance(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2);

public TAnticheat_CheckAimbot(playerid)
{
    if(g_CheatDetectionEnable[AIMBOT_DETECTION] == false)
        return 0;
        
    new Float:distance;
    new Float:player_pos[3], Float:aim_pos[3];
    
    GetPlayerPos(playerid, player_pos[0], player_pos[1], player_pos[2]);
    GetPlayerAimPos(playerid, aim_pos[0], aim_pos[1], aim_pos[2]);
    
    distance = Distance(player_pos[0], player_pos[1], player_pos[2], aim_pos[0], aim_pos[1], aim_pos[2]);
    
    if (distance > MAX_AIM_DISTANCE)
    {
        g_AimbotDetection[playerid]++;
        
        if (g_AimbotDetection[playerid] >= MAX_DETECTION_COUNT)
        {
            // Kick or ban the player for suspected aimbot
            CallLocalFunction("OnPlayerCheatDetected", "is", playerid, "Aimbot");
        }
    }
    else
    {
        g_AimbotDetection[playerid] = 0;
    }
    return 1;
}

// Speedhack detection
public TAnticheat_CheckSpeedhack(playerid)
{
    if(g_CheatDetectionEnable[SPEEDHACK_DETECTION] == false)
        return 0;
        
    new Float:speed;
    
    speed = GetPlayerSpeed(playerid);
    
    if (speed > MAX_PLAYER_SPEED)
    {
        g_SpeedhackDetection[playerid]++;
        
        if (g_SpeedhackDetection[playerid] >= MAX_DETECTION_COUNT)
        {
            CallLocalFunction("OnPlayerCheatDetected", "is", playerid, "Speedhack");
        }
    }
    else
    {
        g_SpeedhackDetection[playerid] = 0;
    }
    return 1;
}

// Rapid fire detection
public TAnticheat_CheckRapidFire(playerid)
{
    if(g_CheatDetectionEnable[RAPIDFIRE_DETECTION] == false)
        return 0;
        
    new last_fire_time, current_time;
    last_fire_time = g_RapidFireDetection[playerid];
    current_time = gettime();

    if (current_time - last_fire_time < MAX_RAPID_FIRE_INTERVAL)
    {
        g_RapidFireDetection[playerid]++;
        
        if (g_RapidFireDetection[playerid] >= MAX_DETECTION_COUNT)
        {
            CallLocalFunction("OnPlayerCheatDetected", "is", playerid, "Rapidfire");
        }
    }
    else
    {
        g_RapidFireDetection[playerid] = current_time;
    }
    return 1;
}

// Health/Armor hack detection
public TAnticheat_CheckHealthHack(playerid)
{
    if(g_CheatDetectionEnable[HEALTHHACK_DETECTION] == false)
        return 0;
        
    new Float:health, Float:armor;
    health = GetPlayerHealth(playerid, health);
    armor = GetPlayerArmour(playerid, armor);
    
    if (health > MAX_HEALTH_ARMOR || armor > MAX_HEALTH_ARMOR)
    {
        g_HealthHackDetection[playerid]++;
        
        if (g_HealthHackDetection[playerid] >= MAX_DETECTION_COUNT)
        {
            CallLocalFunction("OnPlayerCheatDetected", "is", playerid, "HealthHacks");
        }
    }
    else
    {
        // Reset the detection counter
        g_HealthHackDetection[playerid] = 0;
    }
    
    // Check for large health/armor difference
    new Float:reported_health, Float:reported_armor;
    GetPlayerHealth(playerid, reported_health);
    GetPlayerArmour(playerid, reported_armor);
    
    if (abs(reported_health - health) > MAX_HEALTH_ARMOR_DIFF || abs(reported_armor - armor) > MAX_HEALTH_ARMOR_DIFF)
    {
        // Increase detection counter for large difference
        g_HealthHackDetection[playerid]++;
        
        if (g_HealthHackDetection[playerid] >= MAX_DETECTION_COUNT)
        {
            CallLocalFunction("OnPlayerCheatDetected", "is", playerid, "ArmorHacks");
        }
    }
    else
    {
        // Reset the detection counter
        g_HealthHackDetection[playerid] = 0;
    }
    return 1;
}

// Wallhack detection
public TAnticheat_CheckWallhack(playerid)
{
    if(g_CheatDetectionEnable[WALLHACK_DETECTION] == false)
        return 0;
        
    // Check the wallhack cheat detection every WALLHACK_CHECK_INTERVAL milliseconds
    if (gettime() - g_WallhackDetectionTime[playerid] < WALLHACK_CHECK_INTERVAL)
        return false;
        
    new Float:distance;
    new Float:player_pos[3], Float:target_pos[3];

    GetPlayerPos(playerid, player_pos[0], player_pos[1], player_pos[2]);
    // Set the expected position based on your game mode logic or map layout
    target_pos[0] = 100.0;
    target_pos[1] = 200.0;
    target_pos[2] = 10.0;

    distance = Distance(player_pos[0], player_pos[1], player_pos[2], target_pos[0], target_pos[1], target_pos[2]);

    if (distance > MAX_WALLHACK_DISTANCE)
    {
        g_WallhackDetection[playerid]++;

        if (g_WallhackDetection[playerid] >= MAX_DETECTION_COUNT)
        {
            // Kick or ban the player for suspected wallhack
            CallLocalFunction("OnPlayerCheatDetected", "is", playerid, "Wallhacks");
        }
    }
    else
    {
        g_WallhackDetection[playerid] = 0;
    }
    return 1;
}

public TAnticheat_CheckFlyHack(playerid)
{
    if(g_CheatDetectionEnable[FLYHACK_DETECTION] == false)
        return 0;
        
    new Float:distance;
    new Float:player_pos[3], Float:player_velocity[3];

    GetPlayerPos(playerid, player_pos[0], player_pos[1], player_pos[2]);
    GetPlayerVelocity(playerid, player_velocity[0], player_velocity[1], player_velocity[2]);

    // Calculate the distance traveled by the player based on their velocity
    distance = TAnticheat_GetVectorMagnitude(player_velocity[0], player_velocity[1], player_velocity[2]);

    if (distance > MAX_FLY_DISTANCE)
    {
        // Increase fly hack detection count for the player
        g_FlyHackDetection[playerid]++;

        if (g_FlyHackDetection[playerid] >= MAX_DETECTION_COUNT)
        {
            // Player has been detected as a potential fly hacker
            OnPlayerCheatDetected(playerid, "Fly Hack");

            // You may choose to kick or ban the player here
            // Kick(playerid);
        }
    }
    else
    {
        g_FlyHackDetection[playerid] = 0;
    }
    return 1;
}

// Utility function to get the magnitude of a vector
public Float:TAnticheat_GetVectorMagnitude(Float:x, Float:y, Float:z)
{
    return floatsqroot(x * x + y * y + z * z);
}

// Utility function to calculate the aim position of a player
public GetPlayerAimPos(playerid, &Float:x, &Float:y, &Float:z)
{
    new Float:pos[3], Float:angle[3];
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerCameraPos(playerid, angle[0], angle[1], angle[2]);
    
    // Calculate the aim vector based on camera angle
    new Float:distance = 50.0; // Adjust this distance as needed
    x = pos[0] + distance * floatcos(angle[0] * (PI / 180.0)) * floatcos(angle[1] * (PI / 180.0));
    y = pos[1] + distance * floatcos(angle[0] * (PI / 180.0)) * floatsin(angle[1] * (PI / 180.0));
    z = pos[2] - distance * floatsin(angle[0] * (PI / 180.0));
}

// Function to calculate the player's speed (approximation)
public Float:GetPlayerSpeed(playerid)
{
    new Float:currentPosX, Float:currentPosY, Float:currentPosZ;
    GetPlayerPos(playerid, currentPosX, currentPosY, currentPosZ);

    // Check if we have valid previous position data for the player
    if (g_PlayerPrevPosX[playerid] == 0.0 && g_PlayerPrevPosY[playerid] == 0.0 && g_PlayerPrevPosZ[playerid] == 0.0)
    {
        // First call, store the current position and timestamp
        g_PlayerPrevPosX[playerid] = currentPosX;
        g_PlayerPrevPosY[playerid] = currentPosY;
        g_PlayerPrevPosZ[playerid] = currentPosZ;
        g_PlayerPrevTimestamp[playerid] = gettime();
        return INVALID_PLAYER_SPEED; // Speed is not yet available
    }

    // Calculate time difference between the current and previous positions
    new Float:timeDiff = (gettime() - g_PlayerPrevTimestamp[playerid]) / 1000.0; // Convert to seconds

    // Calculate distance moved
    new Float:distance = Distance(currentPosX, currentPosY, currentPosZ, g_PlayerPrevPosX[playerid], g_PlayerPrevPosY[playerid], g_PlayerPrevPosZ[playerid]);

    // Calculate speed as distance divided by time
    new Float:speed = distance / timeDiff;

    // Update previous position and timestamp for the next iteration
    g_PlayerPrevPosX[playerid] = currentPosX;
    g_PlayerPrevPosY[playerid] = currentPosY;
    g_PlayerPrevPosZ[playerid] = currentPosZ;
    g_PlayerPrevTimestamp[playerid] = gettime();

    return Float:speed;
}

// Utility function to calculate distance between two points
public Float:Distance(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2)
{
    new Float:dx = x1 - x2;
    new Float:dy = y1 - y2;
    new Float:dz = z1 - z2;

    return floatsqroot(dx * dx + dy * dy + dz * dz);
}

public Float:abs(Float:value)
{
    if (value >= 0.0)
        return Float:value;
    else
        return Float:-value;
}
