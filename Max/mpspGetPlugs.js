/*

getplugs.js

Retrieve audio plugin names from the Max database and populate a umenu with them

Andrew Pask
andrew@cycling74.com
Tim Place
tim@cycling74.com

*/

var sqlite = new SQLite;
var result = new SQLResult;

NUM_WINDOWS_BOGUS_DLLs = 6; // hack to avoid returning .dlls installed on Windows by Max which are not VSTs

result_filter = ((max.os === "windows") * NUM_WINDOWS_BOGUS_DLLs);

//Query string for getting plug-in names from the Max database

var query = "SELECT _name FROM _things WHERE _kind = 'audioplugin' AND _status = ''";

////////////////////////////////////

/* By default, the Max database is not used by the Max runtime or in standalones.

If you wish to enable the database for use in a standalone,  check the "Database" attribute in the inspector for the standalone object

If you wish to enable the database in the Max runtime application, create a text file in the c74:/init folder and add the following line to it:

    max db.enable 1;
*/

var task = new Task(open_db, this); 
task.interval = 500;
task.repeat();

function open_db()
{
	/*
	var status = sqlite.open("__maxdb__",1);

	if (status == 0)
	{
		var err = sqlite.exec(query, result);

		if (!err){
			if (result.numrecords()> result_filter)
			{
				formatResultForUmenu();
				arguments.callee.task.cancel();
			}
		}
	}
	*/
}

function formatResultForUmenu()
{

	var numrecords = result.numrecords();

	outlet(0, "clear");
	outlet(0, "append","no-plugin");
	
	switch(max.os)
	{
		case "windows":
			for(var i=NUM_WINDOWS_BOGUS_DLLs; i<numrecords; i++)
			{
				outlet(0,"append",result.value(0, i));
			}
			break;
	
		case "macintosh":	
			for(var i=0; i<numrecords; i++)
			{
				outlet(0,"append",result.value(0, i));
			}	
			break;
	}
}
