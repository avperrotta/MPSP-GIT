inlets = 2;
outlets = 1;

setinletassist(0,"MPSP.app path");
setinletassist(1,"MPSP.app path");
setoutletassist(0,"applescript to open mpspEditor.app");


var pathEnd = "editor/mpspEditor.app"; //empty string

function anything()
{
	if(inlet == 0){
	
		var a = arrayfromargs(messagename,arguments);
		var b = a + pathEnd;
		var c = "\"" + b + "\"";
		var d = "osascript -e 'tell application " + c + "to activate'"
	
		
	}
	else{
		var a = arrayfromargs(messagename,arguments);
		var b = a + pathEnd;
		var c = "\"" + b + "\"";
		var d = "osascript -e 'tell application " + c + "to quit'"
	}
	
	outlet(0, d);
}

