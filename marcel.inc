#include <a_samp>
#include <regex>
#include <foreach>

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
	if(integer == 1)
	{
	    boolean = true;
	}
	else if(integer == 0)
	{
	    boolean = false;
	}
	/*
		Since PAWN interprets everything thats non-zero as true (1) thats why the boolean will be set to true
	*/
	else
	{
		boolean = true;
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
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
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
	new carName[20];
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
	new carName[20];
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
switchPlayersVehicle(playerid, modelId, bool:hasToBeDriver = true)
{
	if(isPlayerInVehicle(playerid))
	{
		new playerVehicle = GetPlayerVehicleID(playerid);
		if(GetVehicleModel(playerVehicle) != modelId)
		{
		    if(hasToBeDriver)
		    {
		        if(GetPlayerVehicleSeat(playerid) != 0)
		        {
					return false;
			    }
    		}
    		
    		new Float:X, Float:Y, Float:Z;
			new Float:zAngle;
			new Float:velX, Float:velY, Float:velZ;
			GetVehicleVelocity(playerVehicle, velX, velY, velZ);
			GetVehiclePos(playerVehicle, X, Y, Z);
			GetVehicleZAngle(playerVehicle, zAngle);

			DestroyVehicle(playerVehicle);
			new newVehicle = CreateVehicle(modelId, X, Y, Z, zAngle, -1, -1, -1);
			SetVehicleVirtualWorld(newVehicle, GetPlayerVirtualWorld(playerid));
			PutPlayerInVehicle(playerid, newVehicle, 0);

	    	SetVehiclePos(playerVehicle, X, Y, Z);
	    	SetVehicleZAngle(playerVehicle, zAngle);
	    	SetVehicleVelocity(playerVehicle, velX, velY, velZ);
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

addNitroToPlayersVehicle(playerid)
{
	if(isPlayerInVehicle(playerid))
    {
    	AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
    }
    return 1;
}

repairPlayersVehicle(playerid)
{
	if(isPlayerInVehicle(playerid))
    {
	    RepairVehicle(GetPlayerVehicleID(playerid));
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
isPasswordValid(password[])
{
	//TODO(MSC): Add more validation steps
	if(strlen(password) >= 7)
	{
	    return true;
	}
	return false;
}

/*
Description:
Validates an E-Mail address.

Parameters
eMail	the E-Mail address to validate
Return values:
'true' if the E-Mail address is valid and 'false' if not.
*/
isEmailValid(eMail[])
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
    foreach(Player,i)
    {
        if(IsPlayerInAnyVehicle(i))
        {
            if(GetPlayerVehicleID(i) == vehicleid)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
	return false;
}