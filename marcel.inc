#include <a_samp>
#include <regex>
#include <foreach>

#define MAX_PLAYER_NAME_EX MAX_PLAYER_NAME + 1

new vehicleNames[][] =
{
    "Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial", "Sentinel",
	"Dumper", "Firetruck", "Trashmaster", "Stretch", "Manana", "Infernus",
	"Voodoo", "Pony", "Mule", "Cheetah", "Ambulance", "Leviathan", "Moonbeam",
    "Esperanto", "Taxi", "Washington", "Bobcat", "Whoopee", "BF Injection",
	"Hunter", "Premier", "Enforcer", "Securicar", "Banshee", "Predator", "Bus",
	"Rhino", "Barracks", "Hotknife", "Trailer", "Previon", "Coach", "Cabbie",
	"Stallion", "Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral",
	"Squalo", "Seasparrow", "Pizzaboy", "Tram", "Trailer", "Turismo", "Speeder",
	"Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair", "Berkley's RC Van",
	"Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale",
	"Oceanic","Sanchez", "Sparrow", "Patriot", "Quad", "Coastguard", "Dinghy",
	"Hermes", "Sabre", "Rustler", "ZR-350", "Walton", "Regina", "Comet", "BMX",
	"Burrito", "Camper", "Marquis", "Baggage", "Dozer", "Maverick", "News Chopper",
	"Rancher", "FBI Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring", "Sandking",
	"Blista Compact", "Police Maverick", "Boxville", "Benson", "Mesa", "RC Goblin",
	"Hotring Racer A", "Hotring Racer B", "Bloodring Banger", "Rancher", "Super GT",
	"Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster", "Stunt",
 	"Tanker", "Roadtrain", "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra",
 	"FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck", "Fortune",
 	"Cadrona", "FBI Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer",
 	"Remington", "Slamvan", "Blade", "Freight", "Streak", "Vortex", "Vincent",
    "Bullet", "Clover", "Sadler", "Firetruck", "Hustler", "Intruder", "Primo",
	"Cargobob", "Tampa", "Sunrise", "Merit", "Utility", "Nevada", "Yosemite",
	"Windsor", "Monster", "Monster", "Uranus", "Jester", "Sultan", "Stratium",
	"Elegy", "Raindance", "RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito",
    "Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper",
	"Broadway", "Tornado", "AT-400", "DFT-30", "Huntley", "Stafford", "BF-400",
	"News Van", "Tug", "Trailer", "Emperor", "Wayfarer", "Euros", "Hotdog", "Club",
	"Freight Box", "Trailer", "Andromada", "Dodo", "RC Cam", "Launch", "Police Car",
 	"Police Car", "Police Car", "Police Ranger", "Picador", "S.W.A.T", "Alpha",
 	"Phoenix", "Glendale", "Sadler", "Luggage", "Luggage", "Stairs", "Boxville",
 	"Tiller", "Utility Trailer"
};

/**
 * Turns an integer value into a boolean(non-zero = true, 0 = false)
 *
 * @param integer the integer to put into the boolean
 * @param boolean the boolean that will be set
 *
 * @return returns 1 if it worked, it can only work so it always returns 1;
**/
stock putIntegerValueIntoBoolean(integer, &bool:boolean)
{
	/*
	Since PAWN interprets everything thats non-zero as true (1) thats why the boolean will be set to true
	*/
	if(integer >= 1)
	{
	    boolean = true;
	}
	else if(integer == 0)
	{
	    boolean = false;
	}
	return 1;
}

/*
Description:
Returns a players name

Parameters:
playerid	the players id that you want to get the name from

Return Values:
Playername
*/
stock getName(playerid)
{
	new name[MAX_PLAYER_NAME_EX];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME_EX);
	return name;
}

/*
Description:
Returns a vehicles name

Parameters:
vehicleid the id of the vehicle(not the model)

Return Values:
defined Vehiclename
*/
stock getVehicleNameByID(vehicleid)
{
	new carName[17];
	format(carName, sizeof(carName),"%s", vehicleNames[GetVehicleModel(vehicleid) - 400]);
	return carName;
}

/*
Description:
Returns a vehicles name

Parameters:
vehicleid the model id of the vehicle

Return Values:
defined Vehiclename
*/
stock getVehicleNameByModelID(modelId)
{
	new carName[17];
	format(carName, sizeof(carName),"%s", vehicleNames[modelId - 400]);
	return carName;
}

/*
Description:
Checks if the player is in any vehicle.

Parameters:
playerid	the playerid to check

Returns:
true if he is inside of a vehicle and otherwise false
*/
stock isPlayerInVehicle(playerid)
{
	return GetPlayerVehicleID(playerid) == 0 ? false : true;
}


/*
Description:
Replaces the players actual vehicle with another one. If he isn't isnide of an vehicle, the vehicle won't be replaced.

Parameters:
playerid			the players is
modelId				the new vehicles model id
hasToBeDriver(true)	Does he have to be the driver to make his vehicle get replaces
*/	
stock switchPlayersVehicle(playerid, modelId, bool:keepVelocity = true, Float:addToX = 0.0, Float:addToY = 0.0, Float:addToZ = 0.0, bool:hasToBeDriver = true)
{
	new playerVehicle = GetPlayerVehicleID(playerid);

	if(playerVehicle != 0)
	{
		if(GetVehicleModel(playerVehicle) != modelId)
		{
		    if(hasToBeDriver && GetPlayerVehicleSeat(playerid) != 0)
		    {
				return false;
    		}
    		
    		new Float:X, Float:Y, Float:Z;
			new Float:zAngle;
			new Float:velX, Float:velY, Float:velZ;
			if(keepVelocity)
			{
				GetVehicleVelocity(playerVehicle, velX, velY, velZ);
			}
			GetVehiclePos(playerVehicle, X, Y, Z);
			GetVehicleZAngle(playerVehicle, zAngle);

			X += addToX;
			Y += addToY;
			Z += addToZ;

			DestroyVehicle(playerVehicle);
			new newVehicle = CreateVehicle(modelId, X, Y, Z, zAngle, -1, -1, -1);
			SetVehicleVirtualWorld(newVehicle, GetPlayerVirtualWorld(playerid));
			PutPlayerInVehicle(playerid, newVehicle, 0);

	    	SetVehiclePos(playerVehicle, X, Y, Z);
	    	SetVehicleZAngle(playerVehicle, zAngle);
			if(keepVelocity)
			{
		    	SetVehicleVelocity(playerVehicle, velX, velY, velZ);
		    }
	    	return true;
		}
	}
	return false;
}

stock removePlayerVehicle(playerid)
{
	new vehicleId = GetPlayerVehicleID(playerid);
	if(vehicleId != 0)
	{
		new Float:X, Float:Y, Float:Z;
		GetPlayerPos(playerid, X, Y ,Z);
		SetPlayerPos(playerid, X, Y, Z);
		DestroyVehicle(vehicleId);
	}
}

stock addNitroToPlayersVehicle(playerid, bool:playsound = true)
{
	new vehicleId = GetPlayerVehicleID(playerid);

	if(vehicleId != 0)
	{
		RemoveVehicleComponent(vehicleId, 1010);
    	AddVehicleComponent(vehicleId, 1010);

    	if(playsound)
		{
			PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
		}
	}
    return 1;
}

stock repairPlayersVehicle(playerid, bool:playsound = true)
{
	new vehicleId = GetPlayerVehicleID(playerid);

	if(vehicleId != 0)
	{
	    RepairVehicle(vehicleId);
		if(playsound)
		{
			PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
		}
	}
	return 1;
}

#define BYTES_PER_CELL 				4

stock SendClientMessageFormatted(playerid, color, fstring[], {Float, _}:...)
{
    static const STATIC_ARGS = 3;
    new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;
    if(n)
    {
        new message[144],arg_start,arg_end;
        #emit CONST.alt        fstring
        #emit LCTRL          5
        #emit ADD
        #emit STOR.S.pri        arg_start

        #emit LOAD.S.alt        n
        #emit ADD
        #emit STOR.S.pri        arg_end
        do
        {
            #emit LOAD.I
            #emit PUSH.pri
            arg_end -= BYTES_PER_CELL;
            #emit LOAD.S.pri      arg_end
        }
        while(arg_end > arg_start);

        #emit PUSH.S          fstring
        #emit PUSH.C          144
        #emit PUSH.ADR         message

        n += BYTES_PER_CELL * 3;
        #emit PUSH.S          n
        #emit SYSREQ.C         format

        n += BYTES_PER_CELL;
        #emit LCTRL          4
        #emit LOAD.S.alt        n
        #emit ADD
        #emit SCTRL          4

        if(playerid == INVALID_PLAYER_ID)
        {
            #pragma unused playerid
            return SendClientMessageToAll(color, message);
        } else {
            return SendClientMessage(playerid, color, message);
        }
    } else {
        if(playerid == INVALID_PLAYER_ID)
        {
            #pragma unused playerid
            return SendClientMessageToAll(color, fstring);
        } else {
            return SendClientMessage(playerid, color, fstring);
        }
    }
}

/*
Description:
Validates a players password.

Parameters
password    the password to validate

Return values:
'true' if the password is valid and 'false' if not.
*/
stock isPasswordValid(password[])
{
	//TODO(MSC): Add more validation steps
	if(strlen(password) >= 7)
	{
	    return false;
	}
	return true;
}

/*
Description:
Validates an E-Mail address.

Parameters
eMail	the E-Mail address to validate
Return values:
'true' if the E-Mail address is valid and 'false' if not.
*/
stock isEmailValid(eMail[])
{
	static RegEx:rEmail;

	if ( !rEmail )
	{
		rEmail = regex_build("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?");
	}

	return regex_match_exid(eMail, rEmail);
}

stock isVehicleOccupied(vehicleid)
{
	#warning The Method 'isVehicleOccupied(vehicleid)' is deprecated
    foreach(Player,i)
    {
		new playerVehicle = GetPlayerVehicleID(i);

		if(playerVehicle != 0)
		{
            if(playerVehicle == vehicleid)
            {
                return true;
            }
        }
    }
	return false;
}

stock kick(playerid, color, message[])
{
	SendClientMessage(playerid, color, message);
	Kick(playerid);
	return true;
}

stock getTickDiff(newtick, oldtick) 
{ 
    if (oldtick < 0 && newtick >= 0) { 
        return newtick - oldtick; 
    } else if (oldtick >= 0 && newtick < 0 || oldtick > newtick) { 
        return (cellmax - oldtick + 1) - (cellmin - newtick); 
    } 
    return newtick - oldtick; 
}

stock getTimeAsString(time)
{
	new milliseconds = time % 1000;
	new secondsAll = time / 1000;
	new seconds = secondsAll % 60;
	new minutes = secondsAll / 60;

	new timeStr[12];
	format(timeStr, sizeof(timeStr), "%02d:%02d:%03d", minutes, seconds, milliseconds);
	return timeStr;
}

stock reshowTextDrawForAll(Text:text)
{
	TextDrawHideForAll(text);
	TextDrawShowForAll(text);
}

stock reshowTextDrawForPlayer(playerid, Text:text)
{
	TextDrawHideForPlayer(playerid, text);
	TextDrawShowForPlayer(playerid, text);
}

stock generateSalt()
{
	new salt[11];

	for(new i; i < 10; i++)
	{
		salt[i] = random(79) + 47;
	}
	salt[10] = 0;
	
	return salt;
}

stock isCharacterInRange(character, char1, char2)
{
	return (character <= char2 && character >= char1);
}

stock deleteAllVehicles(bool:onlyIfUnoccupied = false)
{
	for(new i = 0, vehiclePoolSize = GetVehiclePoolSize(); i < vehiclePoolSize; i++)
	{
		if(onlyIfUnoccupied)
		{
			if(IsVehicleOccupied(i))
			{
				continue;
			}
		}
		DestroyVehicle(i);
	}
	return 1;
}
